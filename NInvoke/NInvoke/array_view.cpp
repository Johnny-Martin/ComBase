/*
 *    array_view.cpp:
 *    
 *    Copyright (C) 2014 By limhiaoing <blog.poxiao.me> All Rights Reserved.
 *
 */

#include "array_view.h"
#include "nctypes.h"
#include <cassert>
#include <lua.hpp>

const char* array_view::name = "NInvoke.array_view";

struct array_view_impl {
    void* array_ptr;
    ctypes value_type;
    std::size_t size;
};

int array_view::create(lua_State* L)
{
    ctype_pointer* p_cpointer = reinterpret_cast<ctype_pointer*>(luaL_checkudata(L, 1, ctype_pointer::name));
    const char* value_type_name = luaL_checkstring(L, 2);
    ctypes value_type = type_name_to_ctype(value_type_name);
    luaL_argcheck(L, is_valid_value_type(value_type) ? 1 : 0, 2, "invalid value type");
    int length = luaL_checkint(L, 3);
    luaL_argcheck(L, length > 0, 3, "invalid array length");
    impl_type* impl = reinterpret_cast<impl_type*>(lua_newuserdata(L, sizeof(impl_type)));
    impl->array_ptr = p_cpointer->get_value();
    impl->size = static_cast<std::size_t>(length);
    impl->value_type = value_type;
    luaL_getmetatable(L, name);
    lua_setmetatable(L, -2);
    return 1;
}

#define ARRAY_VIEW_SWITCH_CASE_MACRO1(TYPE) \
case TYPE: \
    type_enum_to_ctype<TYPE>::ctype::push(L, *(reinterpret_cast<type_enum_to_ctype<TYPE>::ctype::native_type*>(impl->array_ptr) + index)); \
    break;

int array_view::index(lua_State* L)
{
    impl_type* impl = reinterpret_cast<impl_type*>(luaL_checkudata(L, 1, name));
    if(lua_isnumber(L, 2)) {
        int index = luaL_checkint(L, 2);
        luaL_argcheck(L, index >= 0 && static_cast<std::size_t>(index) < impl->size, 2, "out of range");
        switch(impl->value_type) {
        ARRAY_VIEW_SWITCH_CASE_MACRO1(type_char)
        ARRAY_VIEW_SWITCH_CASE_MACRO1(type_signed_char)
        ARRAY_VIEW_SWITCH_CASE_MACRO1(type_unsigned_char)
        ARRAY_VIEW_SWITCH_CASE_MACRO1(type_wchar_t)
        ARRAY_VIEW_SWITCH_CASE_MACRO1(type_short)
        ARRAY_VIEW_SWITCH_CASE_MACRO1(type_unsigned_short)
        ARRAY_VIEW_SWITCH_CASE_MACRO1(type_int)
        ARRAY_VIEW_SWITCH_CASE_MACRO1(type_unsigned_int)
        ARRAY_VIEW_SWITCH_CASE_MACRO1(type_long)
        ARRAY_VIEW_SWITCH_CASE_MACRO1(type_unsigned_long)
        ARRAY_VIEW_SWITCH_CASE_MACRO1(type_long_long)
        ARRAY_VIEW_SWITCH_CASE_MACRO1(type_unsigned_long_long)
        ARRAY_VIEW_SWITCH_CASE_MACRO1(type_pointer)
        default:
            assert(false);
            return 0;
        }
        return 1;
    }
    else {
        const char* key = luaL_checkstring(L, 2);
        if(std::strcmp(key, "get_pointer") == 0) {
            lua_pushcfunction(L, &this_type::get_pointer);
            return 1;
        }
        else {
            return 0;
        }
    }
    return 0;
}

#define ARRAY_VIEW_SWITCH_CASE_MACRO2(TYPE) \
case TYPE: \
    *(reinterpret_cast<type_enum_to_ctype<TYPE>::ctype::native_type*>(impl->array_ptr) + index) = reinterpret_cast<type_enum_to_ctype<TYPE>::ctype*>(luaL_checkudata(L, 3, type_enum_to_ctype<TYPE>::ctype::name))->get_value(); \
    break;

int array_view::newindex(lua_State* L)
{
    impl_type* impl = reinterpret_cast<impl_type*>(luaL_checkudata(L, 1, name));
    int index = luaL_checkint(L, 2);
    luaL_argcheck(L, index >= 0 && static_cast<std::size_t>(index) < impl->size, 2, "out of range");
    switch(impl->value_type) {
    ARRAY_VIEW_SWITCH_CASE_MACRO2(type_char)
    ARRAY_VIEW_SWITCH_CASE_MACRO2(type_signed_char)
    ARRAY_VIEW_SWITCH_CASE_MACRO2(type_unsigned_char)
    ARRAY_VIEW_SWITCH_CASE_MACRO2(type_wchar_t)
    ARRAY_VIEW_SWITCH_CASE_MACRO2(type_short)
    ARRAY_VIEW_SWITCH_CASE_MACRO2(type_unsigned_short)
    ARRAY_VIEW_SWITCH_CASE_MACRO2(type_int)
    ARRAY_VIEW_SWITCH_CASE_MACRO2(type_unsigned_int)
    ARRAY_VIEW_SWITCH_CASE_MACRO2(type_long)
    ARRAY_VIEW_SWITCH_CASE_MACRO2(type_unsigned_long)
    ARRAY_VIEW_SWITCH_CASE_MACRO2(type_long_long)
    ARRAY_VIEW_SWITCH_CASE_MACRO2(type_unsigned_long_long)
    ARRAY_VIEW_SWITCH_CASE_MACRO2(type_pointer)
    default:
        assert(false);
    }
    return 0;
}

int array_view::length(lua_State* L)
{
    impl_type* impl = reinterpret_cast<impl_type*>(luaL_checkudata(L, 1, name));
    lua_pushnumber(L, static_cast<lua_Number>(impl->size));
    return 1;
}

int array_view::to_string(lua_State* L)
{
    impl_type* impl = reinterpret_cast<impl_type*>(luaL_checkudata(L, 1, name));
    lua_pushfstring(L, "%s: %p", impl->array_ptr);
    return 1;
}

int array_view::get_pointer(lua_State* L)
{
    impl_type* impl = reinterpret_cast<impl_type*>(luaL_checkudata(L, 1, name));
    ctype_pointer::push(L, impl->array_ptr);
    return 1;
}

bool array_view::is_valid_value_type(ctypes type)
{
    switch (type) {
    case type_char:
    case type_signed_char:
    case type_unsigned_char:
    case type_wchar_t:
    case type_short:
    case type_unsigned_short:
    case type_int:
    case type_unsigned_int:
    case type_long:
    case type_unsigned_long:
    case type_long_long:
    case type_unsigned_long_long:
    case type_pointer:
        return true;
    default:
        return false;
    }
}

bool array_view::registe(lua_State* L)
{
    int top = lua_gettop(L);
    lua_newtable(L);
    lua_setfield(L, -2, "array_view");
    lua_getfield(L, -1, "array_view");
    lua_pushcfunction(L, &this_type::create);
    lua_setfield(L, -2, "new");
    luaL_newmetatable(L, name);
    lua_pushcfunction(L, &this_type::index);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, &this_type::newindex);
    lua_setfield(L, -2, "__newindex");
    lua_pushcfunction(L, &this_type::length);
    lua_setfield(L, -2, "__len");
    lua_pushcfunction(L, &this_type::to_string);
    lua_setfield(L, -2, "__tostring");
    lua_setmetatable(L, -2);
    lua_settop(L, top);
    return true;
}
