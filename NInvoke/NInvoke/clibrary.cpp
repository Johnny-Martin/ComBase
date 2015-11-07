/*
 *    clibrary.cpp:
 *    
 *    Copyright (C) 2014 By limhiaoing <blog.poxiao.me> All Rights Reserved.
 *
 */

#include "clibrary.h"
#include "nctypes.h"
#include <cstring>
#include <cwchar>
#include <lua.hpp>

// void* memcpy( void* dest, const void* src, std::size_t count );
static int clib_memcpy(lua_State* L)
{
    ctype_pointer* dest_pointer = reinterpret_cast<ctype_pointer*>(luaL_checkudata(L, 1, ctype_pointer::name));
    ctype_pointer* src_pointer = reinterpret_cast<ctype_pointer*>(luaL_checkudata(L, 2, ctype_pointer::name));
    std::size_t count = static_cast<std::size_t>(luaL_checknumber(L, 3));
    std::memcpy(dest_pointer->get_value(), src_pointer->get_value(), count);
    lua_pushvalue(L, 1);
    return 1;
}

// char* strcpy(char* dest, const char* src);
static int clib_strcpy(lua_State* L)
{
    ctype_pointer* dest_pointer = reinterpret_cast<ctype_pointer*>(luaL_checkudata(L, 1, ctype_pointer::name));
    ctype_pointer* src_pointer = reinterpret_cast<ctype_pointer*>(luaL_checkudata(L, 2, ctype_pointer::name));
    std::strcpy(reinterpret_cast<char*>(dest_pointer->get_value()), reinterpret_cast<const char*>(src_pointer->get_value()));
    lua_pushvalue(L, 1);
    return 1;
}

// size_t strlen(const char *str);
static int clib_strlen(lua_State* L)
{
    ctype_pointer* str_pointer = reinterpret_cast<ctype_pointer*>(luaL_checkudata(L, 1, ctype_pointer::name));
    ctype_size_t::push(L, std::strlen(reinterpret_cast<const char*>(str_pointer->get_value())));
    return 1;
}

// wchar_t *wcscpy(wchar_t *dest, const wchar_t *src);
static int clib_wcscpy(lua_State* L)
{
    ctype_pointer* dest_pointer = reinterpret_cast<ctype_pointer*>(luaL_checkudata(L, 1, ctype_pointer::name));
    ctype_pointer* src_pointer = reinterpret_cast<ctype_pointer*>(luaL_checkudata(L, 2, ctype_pointer::name));
    std::wcscpy(reinterpret_cast<wchar_t*>(dest_pointer->get_value()), reinterpret_cast<const wchar_t*>(src_pointer->get_value()));
    lua_pushvalue(L, 1);
    return 1;
}

// size_t wcslen(const wchar_t *str);
static int clib_wcslen(lua_State* L)
{
    ctype_pointer* str_pointer = reinterpret_cast<ctype_pointer*>(luaL_checkudata(L, 1, ctype_pointer::name));
    ctype_size_t::push(L, std::wcslen(reinterpret_cast<const wchar_t*>(str_pointer->get_value())));
    return 1;
}

bool clibrary_registe(lua_State* L)
{
    int top = lua_gettop(L);
    lua_pushcfunction(L, &clib_memcpy);
    lua_setfield(L, -2, "memcpy");
    lua_pushcfunction(L, &clib_strcpy);
    lua_setfield(L, -2, "strcpy");
    lua_pushcfunction(L, &clib_strlen);
    lua_setfield(L, -2, "strlen");

    lua_pushcfunction(L, &clib_wcscpy);
    lua_setfield(L, -2, "wcscpy");
    lua_pushcfunction(L, &clib_wcslen);
    lua_setfield(L, -2, "wcslen");
    lua_settop(L, top);
    return true;
}
