/*
 *    dllimport.cpp:
 *    
 *    Copyright (C) 2014 By limhiaoing <blog.poxiao.me> All Rights Reserved.
 *
 */

#include "dllimport.h"
#include "nctypes.h"
#include "native_function_wrapper.h"
#include <Windows.h>
#include <lua.hpp>

int dllimport(lua_State* L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "dll");
    const char* dll_utf8 = luaL_checkstring(L, -1);
    int dll_utf8_length = static_cast<int>(std::strlen(dll_utf8));
    wchar_t dll_utf16[MAX_PATH];
    int dll_utf16_length = ::MultiByteToWideChar(CP_UTF8, 0, dll_utf8, dll_utf8_length, dll_utf16, MAX_PATH);
    if(dll_utf16_length == 0 || dll_utf16_length == MAX_PATH) {
        return 0;
    }
    dll_utf16[dll_utf16_length] = L'\0';
    lua_getfield(L, 1, "name");
    const char* name = luaL_checkstring(L, -1);
#ifndef _WIN64
    lua_getfield(L, 1, "calling_convention");
    const char* cc = luaL_checkstring(L, -1);
    if(std::strcmp(cc, "stdcall") != 0) {
        lua_pushliteral(L, "unsuported calling convention");
        lua_error(L);
    }
#endif
    lua_getfield(L, 1, "arg_types");
    int arg_types_pos = lua_gettop(L);
    luaL_checktype(L, -1, LUA_TTABLE);
    int argc = static_cast<int>(lua_objlen(L, -1));
    for(int i = 1; i <= argc; ++i) {
        lua_pushnumber(L, i);
        lua_gettable(L, -2);
        const char* type_name = luaL_checkstring(L, -1);
        luaL_argcheck(L, type_name_to_ctype(type_name) != type_end, 1, "unknown type in args_types");
        lua_pop(L, 1);
    }
    
    lua_getfield(L, 1, "return_type");
    const char* return_type_name = luaL_checkstring(L, -1);
    ctypes return_type = type_name_to_ctype(return_type_name);
    luaL_argcheck(L, return_type != type_end, 1, "unknown type in return_type");

    invoke_policy policy;
    policy.return_type = return_type;

    lua_pushvalue(L, arg_types_pos);
    for(int i = 1; i <= argc; ++i) {
        lua_pushnumber(L, i);
        lua_gettable(L, -2);
        const char* type_name = luaL_checkstring(L, -1);
        policy.arg_type_list.push_back(type_name_to_ctype(type_name));
        lua_pop(L, 1);
    }

    if(native_function_wrapper::push(L, dll_utf16, name, policy)) {
        return 1;
    }
    return 0;
}
