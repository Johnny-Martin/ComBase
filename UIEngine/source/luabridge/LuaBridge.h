#ifndef LUABRIDGE_H
#define LUABRIDGE_H
#include "stdafx.h"

extern "C" {  
#include "lua.h"
#include "lualib.h"  
#include "lauxlib.h" 
}

#pragma comment(lib, "lua53.lib")

#include <string>
#include <map>
using namespace std;

int LuaCallC();
int CFun4Lua(lua_State* luaState);
int CCallLua();
int SetGlobalObject(lua_State* pLuaState);
int MsgBox(lua_State* luaState);
int GetHttpContentCacheFile(lua_State* luaState);
int LuaShellExecute(lua_State* pLuaState);
int ANSIToLuaString(lua_State* pLuaState);
//Register function "SetGlobalObject"、"GetGlobalObject" to lua EVN
/*LuaGlobalFunctions
{
	"SetGlobalObject",// Lua Object & cpp Object????
	"GetGlobalObject",

	"UnloadLuaFile",
	"LoadLuaFile",//	"SaveLuaTable",	"LoadLuaTable",

	"KillTimer",
	"SetTimer",
	"SetDelay",
	,
	"MsgBox",
	"Log",
}
*/
typedef struct 
{
	char* funcName;
	lua_CFunction func;
}_LuaCFunctionInfo;

enum LUA_ENV_ERROR
{
	LUA_ENV_SUCCESS = 30000,
	LUA_ENV_INITFAILED,
	LUA_ENV_LOADFILEFAILED,
	LUA_ENV_INVOKEFAILED,
};
#define BEGIN_LUACFUNCTION_DECLARE()\
	private:\
	static const _LuaCFunctionInfo* _GetCFunctionArray() {\
			static const _LuaCFunctionInfo functions[] = {

#define REGISTER_CFUNCTION(fun)\
				{#fun, fun},

#define END_LUACFUNCTION_DECLARE()\
			};m_initCFunctionArraySize = sizeof(functions);\
			return functions;}



//Register C_Function for Lua ENV;
//Invoke Lua function in Cpp code by using function name
class LuaBridge
{
public:
								LuaBridge();
								~LuaBridge();
	LUA_ENV_ERROR				InitLuaEnv();
	LUA_ENV_ERROR				UnInitLuaEnv();
	LUA_ENV_ERROR				LoadLuaFile(const char* pszFilePath);
	LUA_ENV_ERROR				InvokeLuaFunction(const char* funcName, int a, int b);

	BEGIN_LUACFUNCTION_DECLARE()
		REGISTER_CFUNCTION(SetGlobalObject)
		REGISTER_CFUNCTION(MsgBox)
		REGISTER_CFUNCTION(GetHttpContentCacheFile)
		REGISTER_CFUNCTION(LuaShellExecute)
		REGISTER_CFUNCTION(ANSIToLuaString)
	END_LUACFUNCTION_DECLARE()
protected:
private:
	void						InitLuaGlobalFunctions(lua_State* pLuaStat);
	LUA_ENV_ERROR				RegisterCFunction(lua_State* pLuaStat, const char* pszFuncName, lua_CFunction pFun);
	LUA_ENV_ERROR				RegisterCppObject(lua_State* pLuaStat, const char* pszObjectID, void* pObj);
	LUA_ENV_ERROR				RegisterCppClass(lua_State* pLuaStat, const char* pszClassID, void* pClass);

	lua_State*					m_luaState;
	static int					m_initCFunctionArraySize;
};

#define LUNAR_DECLARE_METHOD(Class, Name) {#Name, &Class::Name}

template <typename T> 
class Lunar 
{
public:
	typedef int (T::*mfp)(lua_State *L);//C++可以向Lua注册的函数类型
	typedef struct { const char *name; mfp mfunc; } RegType;//向Lua中注册的函数名字以及对应的函数地址

	//用来注册C++定义的类,供Lua使用
	static void Register(lua_State *L) 
	{
		lua_newtable(L);//创建method table，该table key为函数名，
		//value为函数地址（函数可以是C++中定义类的方法，也可以在Lua中定义函数）
		//在Lua中，以table的key为函数名，调用相应的方法。
		int methods = lua_gettop(L);

		luaL_newmetatable(L, T::className);//创建userdata的元表
		int metatable = lua_gettop(L);

		lua_pushvalue(L, methods);//把method table注册到全局，这样Lua中可以直接使用、增加函数的Lua实现
		set(L, LUA_GLOBALSINDEX, T::className);

		//隐藏userdata的实质的元表，也就是说在Lua中
		//调用getmetatable(userdata)得到的是methods table，而不是metatable table
		lua_pushvalue(L, methods);
		set(L, metatable, "__metatable");

		lua_pushvalue(L, methods);//设置metatable table的元方法
		set(L, metatable, "__index");

		lua_pushcfunction(L, tostring_T);
		set(L, metatable, "__tostring");

		lua_pushcfunction(L, gc_T); //设置__gc元方法，在Lua回收userdata时，可以做一些其他操作，比如释放其相应的对象
		set(L, metatable, "__gc");

		lua_newtable(L); //创建methods的元表mt
		lua_pushcfunction(L, new_T);
		lua_pushvalue(L, -1); // 把new_T再次压入栈顶
		set(L, methods, "new"); // 把new_T函数加入到methods中，这样脚本可通过调用T:new()来创建C++对象
		set(L, -3, "__call"); // mt.__call = mt，这样脚本可以通过调用T()来创建C++对象
		lua_setmetatable(L, methods); //设置methods的元表为mt

		//把类T中的方法保存到method table中，供Lua脚本使用
		for (RegType *l = T::methods; l->name; l++) 
		{
			lua_pushstring(L, l->name);
			lua_pushlightuserdata(L, (void*)l); //以注册函数在数组的位置作为cclosure的upvalue
			lua_pushcclosure(L, thunk, 1); //在Lua调用的类方法，调用的都是c closure thunk，thunk通过upvalue获取实质调用的函数地址
			lua_settable(L, methods);
		}

		lua_pop(L, 2); //弹出methods和metatable table，保证Register调用后，栈的空间大小不变
	}

	//调用保存在method table中的函数
	//在调用call之前，需要向栈中压入userdata和参数，
	//并把最后的调用结果压入栈中，参数method传入要调用的函数名
	static int call(lua_State *L, const char *method, int nargs=0, int nresults=LUA_MULTRET, int errfunc=0)
	{
		int base = lua_gettop(L) - nargs; //获取userdata在栈中的索引
		if (!luaL_checkudata(L, base, T::className)) 
		{
			//如果用错误的类型调用相应的方法，则从栈中弹出userdata和参数
			//并且压入相应的错误信息
			lua_settop(L, base-1);
			lua_pushfstring(L, "not a valid %s userdata", T::className);
			return -1;
		}

		lua_pushstring(L, method); //压入方法名，通过该名字在userdata method table中获取实质要调用的c closure

		//获取对应的函数地址，其流程是从userdata的元表metatable查找，
		//而metatable.__index=methods，在methods中通过方法名，获取相应的方法
		lua_gettable(L, base);
		if (lua_isnil(L, -1))//若不存在相应的方法 
		{
			lua_settop(L, base-1);
			lua_pushfstring(L, "%s missing method '%s'", T::className, method);
			return -1;
		}
		lua_insert(L, base); // 把方法移到userdata和args下面

		int status = lua_pcall(L, 1+nargs, nresults, errfunc); // 调用方法
		if (status) 
		{
			const char *msg = lua_tostring(L, -1);
			if (msg == NULL) msg = "(error with no message)";
			lua_pushfstring(L, "%s:%s status = %d\n%s",
				T::className, method, status, msg);
			lua_remove(L, base); //remove old message
			return -1;
		}
		return lua_gettop(L) - base + 1; // 调用的方法，返回值的个数
	}

	//向栈中压入userdata，该userdata包含一个指针，该指针指向一个类型为T的对象
	//参数obj为指向对象的指针，参数gc默认为false，即Lua在回收userdata时，不会主动是释放obj对应的对象，此时应用程序负责相应对象释放
	//若为true，则Lua在回收userdata时，会释放相应的对象
	static int push(lua_State *L, T *obj, bool gc=false) 
	{
		if (!obj){ lua_pushnil(L); return 0; }
		luaL_getmetatable(L, T::className); //在注册表中获取类名的对应的table mt，以作为下面userdata的元表
		if (lua_isnil(L, -1)) luaL_error(L, "%s missing metatable", T::className);
		int mt = lua_gettop(L);

		//设置mt["userdata"] = lookup，并向栈顶压入lookup，lookup是一个mode为"v"的weak table，保存所有类对象对应的userdata
		//key是对象地址，value是userdata
		subtable(L, mt, "userdata", "v");
		userdataType *ud =
			static_cast(pushuserdata(L, obj, sizeof(userdataType))); //向栈顶压入一个userdata
		if (ud) 
		{
			ud->pT = obj; //把对象的地址obj保存到userdata中
			lua_pushvalue(L, mt); //压入注册表中类名对应的table mt
			lua_setmetatable(L, -2); //设置userdata的元表
			if (gc == false) 
			{
				//gc为false，Lua在回收userdata时，不会主动是释放obj对应的对象，此时应用程序负责相应对象释放
				lua_checkstack(L, 3);

				//mt["do not trash"] = nottrash，nottrash是一个mode为"k"的weak table，保存所有不会随userdata回收其相应对象也释放的userdata
				//key是userdata，value是true，向栈顶压入nottrash
				subtable(L, mt, "do not trash", "k");
				lua_pushvalue(L, -2); //再次压入userdata
				lua_pushboolean(L, 1);
				lua_settable(L, -3); //nottrash[userdata] = true
				lua_pop(L, 1); //把nottrash从栈中弹出
			}
		}
		lua_replace(L, mt); //把索引mt出元表值替换为userdata
		lua_settop(L, mt); //设置栈的大小，即通过调用push()调用，栈顶元素为userdata，该userdata包含指向对象的指针
		return mt; //返回userdata在栈中的索引
	}

	//检测索引narg处的值是否为相应的userdata，若是则返回一个指针，该指针指向类型T的对象
	static T *check(lua_State *L, int narg) 
	{
		userdataType *ud = static_cast(luaL_checkudata(L, narg, T::className));
		if(!ud)
		{
			luaL_typerror(L, narg, T::className);
			return NULL;
		}
		return ud->pT;
	}

private:
	typedef struct { T *pT; } userdataType;
	Lunar(); //隐藏默认的构造函数

	//Lua中调用类的成员函数，都是通过调用该函数，然后使用userdataType的upvalue来调用实质的成员函数
	static int thunk(lua_State *L) 
	{
		//此时栈中元素是userdata和参数
		T *obj = check(L, 1); //检测是否是相应的userdata，若是，返回指向T对象的指针
		lua_remove(L, 1); //从栈中删除userdata，以便成员函数的参数的索引从1开始
		RegType *l = static_cast(lua_touserdata(L, lua_upvalueindex(1)));//利用upvalue获取相应的成员函数
		return (obj->*(l->mfunc))(L); //调用实质的成员函数
	}

	//创建一个新的对象T，在脚本中调用T()或T:new()，实质调用的都是该函数
	//调用后，栈顶元素为userdata，该userdata包含指向对象的指针
	static int new_T(lua_State *L) 
	{
		lua_remove(L, 1); // 要求在脚本中使用T:new()，而不能是T.new()
		T *obj = new T(L); // 调用类T的构造函数
		push(L, obj, true); // 传入true，表明Lua回收userdata时，相应的对象也会删除
		return 1;
	}

	//Lua在回收userdata时，相应的也会调用该函数
	//根据userdata是否保存在nottrash（即mt["do not trash"]，mt为注册表中T:classname对应的table）中来决定
	//是否释放相应的对象，若在，则不释放相应的对象，需要应用程序自己删除，否则删除相应的对象
	static int gc_T(lua_State *L) 
	{
		if (luaL_getmetafield(L, 1, "do not trash")) 
		{
			lua_pushvalue(L, 1); //再次压入userdata
			lua_gettable(L, -2); //向栈中压入nottrash[userdata]
			if (!lua_isnil(L, -1)) return 0; //在nottrash中，不删除相应的对象
		}
		userdataType *ud = static_cast(lua_touserdata(L, 1));
		T *obj = ud->pT;
		if (obj) delete obj; //删除相应的对象
		return 0;
	}

	//在Lua中调用tostring(object)时，会调用该函数
	static int tostring_T (lua_State *L) 
	{
		char buff[32];
		userdataType *ud = static_cast(lua_touserdata(L, 1));
		T *obj = ud->pT;
		sprintf(buff, "%p", (void*)obj);
		lua_pushfstring(L, "%s (%s)", T::className, buff);

		return 1;
	}

	//设置t[key]=value，t是索引为table_index对应的值，value为栈顶元素
	static void set(lua_State *L, int table_index, const char *key) 
	{
		lua_pushstring(L, key);
		lua_insert(L, -2); //交换key和value
		lua_settable(L, table_index);//等效于t[key]=value，t位于table_index处，栈顶是value，栈顶之下是key
	}

	//在栈顶压入一个模式为mode的weak table
	static void weaktable(lua_State *L, const char *mode) 
	{
		lua_newtable(L);
		lua_pushvalue(L, -1);
		lua_setmetatable(L, -2); //创建的weak table以自身作为元表
		lua_pushliteral(L, "__mode");
		lua_pushstring(L, mode);
		lua_settable(L, -3); // metatable.__mode = mode
	}

	//该函数向栈中压入值t[name]，t是给定索引的tindex的值，
	//若原来t[name]值不存在，则创建一个模式为mode的weak table wt，并且赋值t[name] = wt
	//最后栈顶中压入这个weak table
	static void subtable(lua_State *L, int tindex, const char *name, const char *mode) 
	{
		lua_pushstring(L, name);
		lua_gettable(L, tindex);
		if (lua_isnil(L, -1)) 
		{
			lua_pop(L, 1); //弹出nil
			lua_checkstack(L, 3); //检测栈的空间，是否足够
			weaktable(L, mode); //栈顶压入一个指定模式的weak table
			lua_pushstring(L, name);
			lua_pushvalue(L, -2); //再次压入创建的weak table
			lua_settable(L, tindex); //t[name] = wt
		}
	}

	//向栈顶压入lookup[key]，lookup是一个weak table，保存所有类对象对应的userdata
	//key是对象地址，value是userdata，若不存在则创建一个userdata，并赋值lookup[key] = userdata
	//这样使得在脚本中引用过的对象，就不会创建新的userdata了
	static void *pushuserdata(lua_State *L, void *key, size_t sz) 
	{
		void *ud = 0;
		lua_pushlightuserdata(L, key); //创建一个light userdata
		lua_gettable(L, -2); // 查找lookup[key]是否存在
		if (lua_isnil(L, -1)) 
		{
			lua_pop(L, 1); //弹出nil
			lua_checkstack(L, 3); //检测栈的空间
			ud = lua_newuserdata(L, sz); //创建一个userdata
			lua_pushlightuserdata(L, key);
			lua_pushvalue(L, -2); //再次压入userdata
			lua_settable(L, -4); //lookup[key] = userdata
		}
		return ud;
	}
};
#endif