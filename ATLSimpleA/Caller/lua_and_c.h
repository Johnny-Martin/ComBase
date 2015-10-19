
extern "C" {  
#include <lua.h>
#include <lualib.h>  
#include <lauxlib.h>  
} 
#pragma comment(lib, "lua53.lib") 

/*Lua������ָ��*/  
lua_State* L;  
extern "C" int CCallLua ()
{  
	/*��ʼ��Lua*/  
	L = luaL_newstate();  
	/*����Lua������*/  
	luaL_openlibs(L);  
	/*����lua�ű�*/  
	int loadRet = luaL_dofile(L, "test.lua");  
	//int iError = lua_pcall(L, 0, 0, 0);  
	if (loadRet)  
	{  
		lua_close(L);  
		return 1;  
	}  
	/*����lua�еĺ���sum*/  
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
	/* ���Lua */  
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
	lua_State * l = luaL_newstate() ;        //����lua���л���  
	if ( l == NULL )// err_return(-1,"luaL_newstat() failed");   
		return 1 ;  
	int ret = 0 ;  
	ret = luaL_loadfile(l,"test.lua") ;      //����lua�ű��ļ�  
	if ( ret != 0 ) //err_return(-1,"luaL_loadfile failed") ;  
		return 1 ;
	ret = lua_pcall(l,0,0,0) ;  
	if ( ret != 0 )// err_return(-1,"lua_pcall failed:%s",lua_tostring(l,-1)) ;  
		return 1 ;

	//luaL_newlib();
	lua_pushcfunction(l,CFun4Lua) ;         //ע����lua��ʹ�õ�c����  
	lua_setglobal(l,"CFun4Lua") ;           //�󶨵�lua�е�����csum  

	int a = 15 ;  
	int b = 12 ;
	lua_getglobal(l,"LuaFun4Cpp");           //����lua�е�mysum�������ú������ñ������ж����csum����ʵ�ּӷ�  
	lua_pushinteger(l,a) ;  
	lua_pushinteger(l,b) ;  
	ret = lua_pcall(l,2,1,0) ;  
	
	if ( ret != 0 ) //err_return(-1,"lua_pcall failed:%s",lua_tostring(l,-1)) ; 
		return 1 ;
	printf("mysum:%d + %d = %ld\n",a,b,lua_tointeger(l,-1)) ;  
	lua_pop(l,1) ;  

	lua_close(l) ;                     //�ͷ�lua���л���  
	return 0 ;  
}  

template<class T>
class ATL_NO_VTABLE CLuaBridge
{

};