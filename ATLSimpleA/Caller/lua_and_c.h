
extern "C" {  
#include <lua.h>
#include <lualib.h>  
#include <lauxlib.h>  
} 
#pragma comment(lib, "lua5.1.lib") 

/*Lua解释器指针*/  
lua_State* L;  
extern "C" int CCallLua ()
{  
	/*初始化Lua*/  
	L = luaL_newstate();  
	/*载入Lua基本库*/  
	luaL_openlibs(L);  
	/*加载lua脚本*/  
	int loadRet = luaL_dofile(L, "test.lua");  
	//int iError = lua_pcall(L, 0, 0, 0);  
	if (loadRet)  
	{  
		lua_close(L);  
		return 1;  
	}  
	/*调用lua中的函数sum*/  
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
	/* 清除Lua */  
	lua_close(L);  
	return 0;  
}  
int CFun4Lua(lua_State* l)  
{  
	int a = lua_tointeger(l,1) ;  
	int b = lua_tointeger(l,2) ;  
	lua_pushinteger(l,a+b) ; 

	return 1 ;  
}  

template<class T>
class ATL_NO_VTABLE CLuaBridge
{

};