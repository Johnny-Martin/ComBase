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
			static const _LuaCFunctionInfo functions[] = {

#define REGISTER_CFUNCTION(fun)\
				{#fun, fun},

#define END_LUACFUNCTION_DECLARE()\
			};m_initCFunctionArraySize = sizeof(functions);\
			return functions;}




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


	lua_State*					m_luaState;
	static int					m_initCFunctionArraySize;
};

#endif