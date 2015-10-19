
extern "C" {  
#include <lua.h>
#include <lualib.h>  
#include <lauxlib.h>  
} 
#pragma comment(lib, "lua53.lib") 

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
	lua_State * l = luaL_newstate() ;        //创建lua运行环境  
	if ( l == NULL )// err_return(-1,"luaL_newstat() failed");   
		return 1 ;  
	int ret = 0 ;  
	ret = luaL_loadfile(l,"test.lua") ;      //加载lua脚本文件  
	if ( ret != 0 ) //err_return(-1,"luaL_loadfile failed") ;  
		return 1 ;
	ret = lua_pcall(l,0,0,0) ;  
	if ( ret != 0 )// err_return(-1,"lua_pcall failed:%s",lua_tostring(l,-1)) ;  
		return 1 ;

	//luaL_newlib();
	lua_pushcfunction(l,CFun4Lua) ;         //注册在lua中使用的c函数  
	lua_setglobal(l,"CFun4Lua") ;           //绑定到lua中的名字csum  

	int a = 15 ;  
	int b = 12 ;
	lua_getglobal(l,"LuaFun4Cpp");           //调用lua中的mysum函数，该函数调用本程序中定义的csum函数实现加法  
	lua_pushinteger(l,a) ;  
	lua_pushinteger(l,b) ;  
	ret = lua_pcall(l,2,1,0) ;  
	
	if ( ret != 0 ) //err_return(-1,"lua_pcall failed:%s",lua_tostring(l,-1)) ; 
		return 1 ;
	printf("mysum:%d + %d = %ld\n",a,b,lua_tointeger(l,-1)) ;  
	lua_pop(l,1) ;  

	lua_close(l) ;                     //释放lua运行环境  
	return 0 ;  
}  

template<class T>
class ATL_NO_VTABLE CLuaBridge
{

};