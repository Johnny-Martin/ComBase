#ifndef LUABRIDGE_H
#define LUABRIDGE_H

extern "C" {  
#include "lua.h"
#include "lualib.h"  
#include "lauxlib.h" 
} 
#pragma comment(lib, "lua53.lib")
 
#include <string>
#include <map>
using namespace std;

lua_State* L;  
extern "C" int CCallLua ()
{  
	/*Initialize Lua*/  
	L = luaL_newstate();  
	/*Load Lua Library*/  
	luaL_openlibs(L);  
	/*Load lua file*/  
	int loadRet = luaL_dofile(L, "test.lua");  
	//int iError = lua_pcall(L, 0, 0, 0);  
	if (loadRet)  
	{  
		lua_close(L);  
		return 1;  
	}  
	/*Invoke lua function*/  
	int a = 11 ;  
	int b = 12 ;  
	lua_getglobal(L,"LuaFun4Cpp");               
	lua_pushinteger(L,a) ;  
	lua_pushinteger(L,b) ;  
	int ret = lua_pcall(L,2,1,0) ;  
	if ( ret != 0 )  
		return 0;  
	printf("sum:%d + %d = %ld\n",a,b,lua_tointeger(L,-1)) ;  
	lua_pop(L,1);  
	/*Release Lua */  
	lua_close(L);  
	return 0;  
}  
int CFun4Lua(lua_State* luaState)  
{  
	lua_Integer a = lua_tointeger(luaState,1) ;  
	lua_Integer b = lua_tointeger(luaState,2) ;  
	lua_pushinteger(luaState,a+b) ; 

	return 1 ;  
}  

//#define err_exit(num,fmt,args)  \  
//do{printf("[%s:%d]",fmt,"\n",__FILE__,__LINE__,##args);exit(num);} while(0)
//
//#define err_return(num,fmt,args)  \  
//do{printf("[%s:%d]",fmt,"\n",__FILE__,__LINE__,##args);return(num);} while(0)  

int LuaCallC()  
{  
	lua_State * l = luaL_newstate() ; 
	if ( l == NULL )// err_return(-1,"luaL_newstat() failed");   
		return 1 ;  
	int ret = 0 ;  
	ret = luaL_loadfile(l,"test.lua") ;  
	if ( ret != 0 ) //err_return(-1,"luaL_loadfile failed") ;  
		return 1 ;
	ret = lua_pcall(l,0,0,0) ;  
	if ( ret != 0 )// err_return(-1,"lua_pcall failed:%s",lua_tostring(l,-1)) ;  
		return 1 ;

	//luaL_newlib();
	//Register the cpp function into lua
	lua_pushcfunction(l,CFun4Lua); 
	//Bind the global name to the function
	lua_setglobal(l,"CFun4Lua");

	//Now we can invoke function "CFun4Lua" in lua file
	int a = 15 ;  
	int b = 12 ;
	lua_getglobal(l,"LuaFun4Cpp"); 
	lua_pushinteger(l,a) ;  
	lua_pushinteger(l,b) ;  
	ret = lua_pcall(l,2,1,0) ;  

	if ( ret != 0 ) //err_return(-1,"lua_pcall failed:%s",lua_tostring(l,-1)) ; 
		return 1 ;
	printf("mysum:%d + %d = %ld\n",a,b,lua_tointeger(l,-1)) ;  
	lua_pop(l,1) ;  

	lua_close(l) ;
	return 0 ;  
}  

//Register function "SetGlobalObject"¡¢"GetGlobalObject" to lua EVN
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
			static const _LuaCFunctionInfo functions[] = {\

#define REGISTER_CFUNCTION(fun)\
				{#fun, fun},

#define END_LUACFUNCTION_DECLARE()\
			};m_initCFunctionArraySize = sizeof(functions);\
			return functions;}


int SetGlobalObject(lua_State* pLuaState)
{
	lua_Integer a = lua_tointeger(pLuaState,1);  
	lua_Integer b = lua_tointeger(pLuaState,2);  
	
	lua_pushinteger(pLuaState,a+b);
	return 1;
}

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
	END_LUACFUNCTION_DECLARE()
protected:
private:
	void						RegisterLuaGlobalFunctions(lua_State* pLuaStat);


	lua_State* m_luaState;
	static int m_initCFunctionArraySize;
};


int LuaBridge::m_initCFunctionArraySize = 0;

void LuaBridge::RegisterLuaGlobalFunctions(lua_State* pLuaStat)
{
	const _LuaCFunctionInfo* pArray = LuaBridge::_GetCFunctionArray();
	int iArrayLen = m_initCFunctionArraySize / sizeof(_LuaCFunctionInfo);
	for (int i=0; i<iArrayLen; ++i)
	{
		//Register the cpp function into lua
		lua_pushcfunction(m_luaState, pArray[i].func); 
		//Bind the global name to the function
		lua_setglobal(m_luaState, pArray[i].funcName);
	}
}

LuaBridge::LuaBridge():m_luaState(NULL)
{	

}
LUA_ENV_ERROR LuaBridge::InitLuaEnv()
{
	m_luaState = luaL_newstate(); 
	if ( m_luaState == NULL )
		return LUA_ENV_INITFAILED; 

	RegisterLuaGlobalFunctions(m_luaState);

	return LUA_ENV_SUCCESS;
}
LUA_ENV_ERROR LuaBridge::LoadLuaFile(const char* pszFilePath)
{
	if ( m_luaState == NULL )
		return LUA_ENV_INITFAILED;

	int ret = 0;  
	ret = luaL_loadfile(m_luaState, pszFilePath);  
	if ( ret != 0 )   
		return LUA_ENV_LOADFILEFAILED;

	ret = lua_pcall(m_luaState, 0, 0, 0);  
	if ( ret != 0 )
		return LUA_ENV_LOADFILEFAILED;

	return LUA_ENV_SUCCESS;
}
LUA_ENV_ERROR LuaBridge::InvokeLuaFunction(const char* funcName, int a, int b)
{
	lua_getglobal(m_luaState, funcName); 
	lua_pushinteger(m_luaState, a);  
	lua_pushinteger(m_luaState, b);  
	int ret = lua_pcall(m_luaState, 2, 1, 0);  
	if ( ret != 0 )  
		return LUA_ENV_INVOKEFAILED;

	int result = lua_tointeger(m_luaState,-1);
	printf("sum:%d + %d + 230 = %ld\n", a, b, result);  
	lua_pop(m_luaState,1);  

	return LUA_ENV_SUCCESS;
}
LUA_ENV_ERROR LuaBridge::UnInitLuaEnv()
{
	lua_close(m_luaState);
	return LUA_ENV_SUCCESS;
}

LuaBridge::~LuaBridge()
{
	UnInitLuaEnv();
}

#endif