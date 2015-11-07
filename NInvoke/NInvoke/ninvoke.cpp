/*
 *    ninvoke.cpp:
 *    
 *    Copyright (C) 2014 By limhiaoing <blog.poxiao.me> All Rights Reserved.
 *
 */

#include "ninvoke.h"
#include "nctypes.h"
#include "array_view.h"
#include "byte_array.h"
#include "clibrary.h"
#include "dllimport.h"
#include "native_function_wrapper.h"
#include <lua.hpp>

int luaopen_ninvoke(lua_State* L)
{
    lua_newtable(L);
    int root = lua_gettop(L);

    lua_pushcfunction(L, dllimport);
    lua_setfield(L, -2, "dllimport");
    lua_pushcfunction(L, &byte_array::str_to_byte_array);
    lua_setfield(L, -2, "str_to_byte_array");
    lua_pushcfunction(L, &byte_array::byte_array_to_str);
    lua_setfield(L, -2, "byte_array_to_str");

    lua_newtable(L);
    lua_setfield(L, -2, "ctypes");
    lua_getfield(L, -1, "ctypes");
    
    // scalar types
    ctype_char::registe(L, "char");
    ctype_signed_char::registe(L, "signed_char");
    ctype_unsigned_char::registe(L, "unsigned_char");
    ctype_wchar_t::registe(L, "wchar_t");
    ctype_short::registe(L, "short");
    ctype_unsigned_short::registe(L, "unsigned_short");
    ctype_int::registe(L, "int");
    ctype_unsigned_int::registe(L, "unsigned_int");
    ctype_long::registe(L, "long");
    ctype_unsigned_long::registe(L, "unsigned_long");
    ctype_long_long::registe(L, "long_long");
    ctype_unsigned_long_long::registe(L, "unsigned_long_long");

    // pointer
    ctype_pointer::registe(L);

    lua_settop(L, root);
    byte_array::registe(L);
    array_view::registe(L);
    native_function_wrapper::registe(L);

    lua_newtable(L);
    lua_setfield(L, -2, "clib");
    lua_getfield(L, -1, "clib");
    clibrary_registe(L);
    lua_settop(L, root);
    return 1;
}
