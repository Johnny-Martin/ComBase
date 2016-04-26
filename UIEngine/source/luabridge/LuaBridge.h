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
//Register function "SetGlobalObject"��"GetGlobalObject" to lua EVN
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
	typedef int (T::*mfp)(lua_State *L);//C++������Luaע��ĺ�������
	typedef struct { const char *name; mfp mfunc; } RegType;//��Lua��ע��ĺ��������Լ���Ӧ�ĺ�����ַ

	//����ע��C++�������,��Luaʹ��
	static void Register(lua_State *L) 
	{
		lua_newtable(L);//����method table����table keyΪ��������
		//valueΪ������ַ������������C++�ж�����ķ�����Ҳ������Lua�ж��庯����
		//��Lua�У���table��keyΪ��������������Ӧ�ķ�����
		int methods = lua_gettop(L);

		luaL_newmetatable(L, T::className);//����userdata��Ԫ��
		int metatable = lua_gettop(L);

		lua_pushvalue(L, methods);//��method tableע�ᵽȫ�֣�����Lua�п���ֱ��ʹ�á����Ӻ�����Luaʵ��
		set(L, LUA_GLOBALSINDEX, T::className);

		//����userdata��ʵ�ʵ�Ԫ��Ҳ����˵��Lua��
		//����getmetatable(userdata)�õ�����methods table��������metatable table
		lua_pushvalue(L, methods);
		set(L, metatable, "__metatable");

		lua_pushvalue(L, methods);//����metatable table��Ԫ����
		set(L, metatable, "__index");

		lua_pushcfunction(L, tostring_T);
		set(L, metatable, "__tostring");

		lua_pushcfunction(L, gc_T); //����__gcԪ��������Lua����userdataʱ��������һЩ���������������ͷ�����Ӧ�Ķ���
		set(L, metatable, "__gc");

		lua_newtable(L); //����methods��Ԫ��mt
		lua_pushcfunction(L, new_T);
		lua_pushvalue(L, -1); // ��new_T�ٴ�ѹ��ջ��
		set(L, methods, "new"); // ��new_T�������뵽methods�У������ű���ͨ������T:new()������C++����
		set(L, -3, "__call"); // mt.__call = mt�������ű�����ͨ������T()������C++����
		lua_setmetatable(L, methods); //����methods��Ԫ��Ϊmt

		//����T�еķ������浽method table�У���Lua�ű�ʹ��
		for (RegType *l = T::methods; l->name; l++) 
		{
			lua_pushstring(L, l->name);
			lua_pushlightuserdata(L, (void*)l); //��ע�ắ���������λ����Ϊcclosure��upvalue
			lua_pushcclosure(L, thunk, 1); //��Lua���õ��෽�������õĶ���c closure thunk��thunkͨ��upvalue��ȡʵ�ʵ��õĺ�����ַ
			lua_settable(L, methods);
		}

		lua_pop(L, 2); //����methods��metatable table����֤Register���ú�ջ�Ŀռ��С����
	}

	//���ñ�����method table�еĺ���
	//�ڵ���call֮ǰ����Ҫ��ջ��ѹ��userdata�Ͳ�����
	//�������ĵ��ý��ѹ��ջ�У�����method����Ҫ���õĺ�����
	static int call(lua_State *L, const char *method, int nargs=0, int nresults=LUA_MULTRET, int errfunc=0)
	{
		int base = lua_gettop(L) - nargs; //��ȡuserdata��ջ�е�����
		if (!luaL_checkudata(L, base, T::className)) 
		{
			//����ô�������͵�����Ӧ�ķ��������ջ�е���userdata�Ͳ���
			//����ѹ����Ӧ�Ĵ�����Ϣ
			lua_settop(L, base-1);
			lua_pushfstring(L, "not a valid %s userdata", T::className);
			return -1;
		}

		lua_pushstring(L, method); //ѹ�뷽������ͨ����������userdata method table�л�ȡʵ��Ҫ���õ�c closure

		//��ȡ��Ӧ�ĺ�����ַ���������Ǵ�userdata��Ԫ��metatable���ң�
		//��metatable.__index=methods����methods��ͨ������������ȡ��Ӧ�ķ���
		lua_gettable(L, base);
		if (lua_isnil(L, -1))//����������Ӧ�ķ��� 
		{
			lua_settop(L, base-1);
			lua_pushfstring(L, "%s missing method '%s'", T::className, method);
			return -1;
		}
		lua_insert(L, base); // �ѷ����Ƶ�userdata��args����

		int status = lua_pcall(L, 1+nargs, nresults, errfunc); // ���÷���
		if (status) 
		{
			const char *msg = lua_tostring(L, -1);
			if (msg == NULL) msg = "(error with no message)";
			lua_pushfstring(L, "%s:%s status = %d\n%s",
				T::className, method, status, msg);
			lua_remove(L, base); //remove old message
			return -1;
		}
		return lua_gettop(L) - base + 1; // ���õķ���������ֵ�ĸ���
	}

	//��ջ��ѹ��userdata����userdata����һ��ָ�룬��ָ��ָ��һ������ΪT�Ķ���
	//����objΪָ������ָ�룬����gcĬ��Ϊfalse����Lua�ڻ���userdataʱ�������������ͷ�obj��Ӧ�Ķ��󣬴�ʱӦ�ó�������Ӧ�����ͷ�
	//��Ϊtrue����Lua�ڻ���userdataʱ�����ͷ���Ӧ�Ķ���
	static int push(lua_State *L, T *obj, bool gc=false) 
	{
		if (!obj){ lua_pushnil(L); return 0; }
		luaL_getmetatable(L, T::className); //��ע����л�ȡ�����Ķ�Ӧ��table mt������Ϊ����userdata��Ԫ��
		if (lua_isnil(L, -1)) luaL_error(L, "%s missing metatable", T::className);
		int mt = lua_gettop(L);

		//����mt["userdata"] = lookup������ջ��ѹ��lookup��lookup��һ��modeΪ"v"��weak table����������������Ӧ��userdata
		//key�Ƕ����ַ��value��userdata
		subtable(L, mt, "userdata", "v");
		userdataType *ud =
			static_cast(pushuserdata(L, obj, sizeof(userdataType))); //��ջ��ѹ��һ��userdata
		if (ud) 
		{
			ud->pT = obj; //�Ѷ���ĵ�ַobj���浽userdata��
			lua_pushvalue(L, mt); //ѹ��ע�����������Ӧ��table mt
			lua_setmetatable(L, -2); //����userdata��Ԫ��
			if (gc == false) 
			{
				//gcΪfalse��Lua�ڻ���userdataʱ�������������ͷ�obj��Ӧ�Ķ��󣬴�ʱӦ�ó�������Ӧ�����ͷ�
				lua_checkstack(L, 3);

				//mt["do not trash"] = nottrash��nottrash��һ��modeΪ"k"��weak table���������в�����userdata��������Ӧ����Ҳ�ͷŵ�userdata
				//key��userdata��value��true����ջ��ѹ��nottrash
				subtable(L, mt, "do not trash", "k");
				lua_pushvalue(L, -2); //�ٴ�ѹ��userdata
				lua_pushboolean(L, 1);
				lua_settable(L, -3); //nottrash[userdata] = true
				lua_pop(L, 1); //��nottrash��ջ�е���
			}
		}
		lua_replace(L, mt); //������mt��Ԫ��ֵ�滻Ϊuserdata
		lua_settop(L, mt); //����ջ�Ĵ�С����ͨ������push()���ã�ջ��Ԫ��Ϊuserdata����userdata����ָ������ָ��
		return mt; //����userdata��ջ�е�����
	}

	//�������narg����ֵ�Ƿ�Ϊ��Ӧ��userdata�������򷵻�һ��ָ�룬��ָ��ָ������T�Ķ���
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
	Lunar(); //����Ĭ�ϵĹ��캯��

	//Lua�е�����ĳ�Ա����������ͨ�����øú�����Ȼ��ʹ��userdataType��upvalue������ʵ�ʵĳ�Ա����
	static int thunk(lua_State *L) 
	{
		//��ʱջ��Ԫ����userdata�Ͳ���
		T *obj = check(L, 1); //����Ƿ�����Ӧ��userdata�����ǣ�����ָ��T�����ָ��
		lua_remove(L, 1); //��ջ��ɾ��userdata���Ա��Ա�����Ĳ�����������1��ʼ
		RegType *l = static_cast(lua_touserdata(L, lua_upvalueindex(1)));//����upvalue��ȡ��Ӧ�ĳ�Ա����
		return (obj->*(l->mfunc))(L); //����ʵ�ʵĳ�Ա����
	}

	//����һ���µĶ���T���ڽű��е���T()��T:new()��ʵ�ʵ��õĶ��Ǹú���
	//���ú�ջ��Ԫ��Ϊuserdata����userdata����ָ������ָ��
	static int new_T(lua_State *L) 
	{
		lua_remove(L, 1); // Ҫ���ڽű���ʹ��T:new()����������T.new()
		T *obj = new T(L); // ������T�Ĺ��캯��
		push(L, obj, true); // ����true������Lua����userdataʱ����Ӧ�Ķ���Ҳ��ɾ��
		return 1;
	}

	//Lua�ڻ���userdataʱ����Ӧ��Ҳ����øú���
	//����userdata�Ƿ񱣴���nottrash����mt["do not trash"]��mtΪע�����T:classname��Ӧ��table����������
	//�Ƿ��ͷ���Ӧ�Ķ������ڣ����ͷ���Ӧ�Ķ�����ҪӦ�ó����Լ�ɾ��������ɾ����Ӧ�Ķ���
	static int gc_T(lua_State *L) 
	{
		if (luaL_getmetafield(L, 1, "do not trash")) 
		{
			lua_pushvalue(L, 1); //�ٴ�ѹ��userdata
			lua_gettable(L, -2); //��ջ��ѹ��nottrash[userdata]
			if (!lua_isnil(L, -1)) return 0; //��nottrash�У���ɾ����Ӧ�Ķ���
		}
		userdataType *ud = static_cast(lua_touserdata(L, 1));
		T *obj = ud->pT;
		if (obj) delete obj; //ɾ����Ӧ�Ķ���
		return 0;
	}

	//��Lua�е���tostring(object)ʱ������øú���
	static int tostring_T (lua_State *L) 
	{
		char buff[32];
		userdataType *ud = static_cast(lua_touserdata(L, 1));
		T *obj = ud->pT;
		sprintf(buff, "%p", (void*)obj);
		lua_pushfstring(L, "%s (%s)", T::className, buff);

		return 1;
	}

	//����t[key]=value��t������Ϊtable_index��Ӧ��ֵ��valueΪջ��Ԫ��
	static void set(lua_State *L, int table_index, const char *key) 
	{
		lua_pushstring(L, key);
		lua_insert(L, -2); //����key��value
		lua_settable(L, table_index);//��Ч��t[key]=value��tλ��table_index����ջ����value��ջ��֮����key
	}

	//��ջ��ѹ��һ��ģʽΪmode��weak table
	static void weaktable(lua_State *L, const char *mode) 
	{
		lua_newtable(L);
		lua_pushvalue(L, -1);
		lua_setmetatable(L, -2); //������weak table��������ΪԪ��
		lua_pushliteral(L, "__mode");
		lua_pushstring(L, mode);
		lua_settable(L, -3); // metatable.__mode = mode
	}

	//�ú�����ջ��ѹ��ֵt[name]��t�Ǹ���������tindex��ֵ��
	//��ԭ��t[name]ֵ�����ڣ��򴴽�һ��ģʽΪmode��weak table wt�����Ҹ�ֵt[name] = wt
	//���ջ����ѹ�����weak table
	static void subtable(lua_State *L, int tindex, const char *name, const char *mode) 
	{
		lua_pushstring(L, name);
		lua_gettable(L, tindex);
		if (lua_isnil(L, -1)) 
		{
			lua_pop(L, 1); //����nil
			lua_checkstack(L, 3); //���ջ�Ŀռ䣬�Ƿ��㹻
			weaktable(L, mode); //ջ��ѹ��һ��ָ��ģʽ��weak table
			lua_pushstring(L, name);
			lua_pushvalue(L, -2); //�ٴ�ѹ�봴����weak table
			lua_settable(L, tindex); //t[name] = wt
		}
	}

	//��ջ��ѹ��lookup[key]��lookup��һ��weak table����������������Ӧ��userdata
	//key�Ƕ����ַ��value��userdata�����������򴴽�һ��userdata������ֵlookup[key] = userdata
	//����ʹ���ڽű������ù��Ķ��󣬾Ͳ��ᴴ���µ�userdata��
	static void *pushuserdata(lua_State *L, void *key, size_t sz) 
	{
		void *ud = 0;
		lua_pushlightuserdata(L, key); //����һ��light userdata
		lua_gettable(L, -2); // ����lookup[key]�Ƿ����
		if (lua_isnil(L, -1)) 
		{
			lua_pop(L, 1); //����nil
			lua_checkstack(L, 3); //���ջ�Ŀռ�
			ud = lua_newuserdata(L, sz); //����һ��userdata
			lua_pushlightuserdata(L, key);
			lua_pushvalue(L, -2); //�ٴ�ѹ��userdata
			lua_settable(L, -4); //lookup[key] = userdata
		}
		return ud;
	}
};
#endif