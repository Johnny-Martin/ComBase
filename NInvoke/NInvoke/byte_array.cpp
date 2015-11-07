/*
 *    byte_array.cpp:
 *    
 *    Copyright (C) 2014 By limhiaoing <blog.poxiao.me> All Rights Reserved.
 *
 */

#include "byte_array.h"
#include <cstring>
#include <cassert>
#include <lua.hpp>
#include <Windows.h>
#include "nctypes.h"

const char* byte_array::name = "NInvoke.byte_array";

struct byte_array_data {
    std::size_t size;
    byte_array::native_value buffer[1];
};

int byte_array::create(lua_State* L)
{
    int size = luaL_checkint(L, 1);
    luaL_argcheck(L, size >= 0, 1, "invalid array size");
    native_value value = 0;
    if(!lua_isnoneornil(L, 2)) {
        value = static_cast<native_value>(luaL_checkint(L, 2));
    }
    this_type::push(L, size, value);
    return 1;
}

enum text_encoding {
    te_ansi,
    te_utf8,
    te_utf16le
};

int byte_array::str_to_byte_array(lua_State* L)
{
    std::size_t size = 0;
    const char* str = luaL_checklstring(L, 1, &size);
    if(size == 0) {
        this_type::push(L, 0);
        return 1;
    }
    text_encoding text_enc = te_utf8;
    if(!lua_isnone(L, 2)) {
        const char* encoding = luaL_checkstring(L, 2);
        if(std::strcmp(encoding, "utf8") == 0) {
            text_enc = te_utf8;
        }
        else if (std::strcmp(encoding, "utf16le") == 0) {
            text_enc = te_utf16le;
        }
        else if (std::strcmp(encoding, "ansi") == 0){
            text_enc = te_ansi;
        }
        else {
            luaL_argerror(L, 2, "unsuported encoding");
        }
    }
    if(text_enc == te_utf8) {
        this_type::push(L, reinterpret_cast<const native_value*>(str), size);
    }
    else if(text_enc == te_utf16le) {
        int length = ::MultiByteToWideChar(CP_UTF8, 0, str, static_cast<int>(size), NULL, 0); 
        if(length == 0) {
            lua_pushliteral(L, "text encoding error");
            lua_error(L);
        }
        wchar_t* utf16_buf = reinterpret_cast<wchar_t*>(lua_newuserdata(L, length * (sizeof(wchar_t) / sizeof(char))));
        ::MultiByteToWideChar(CP_UTF8, 0, str, static_cast<int>(size), utf16_buf, length);
        this_type::push(L, reinterpret_cast<const native_value*>(utf16_buf), length * (sizeof(wchar_t) / sizeof(char)));
    }
    else if(text_enc == te_ansi) {
        int utf16_length = ::MultiByteToWideChar(CP_UTF8, 0, str, static_cast<int>(size), NULL, 0); 
        if(utf16_length == 0) {
            lua_pushliteral(L, "text encoding error");
            lua_error(L);
        }
        wchar_t* utf16_buf = reinterpret_cast<wchar_t*>(lua_newuserdata(L, utf16_length * (sizeof(wchar_t) / sizeof(char))));
        ::MultiByteToWideChar(CP_UTF8, 0, str, static_cast<int>(size), utf16_buf, utf16_length);
        int ansi_length = ::WideCharToMultiByte(CP_ACP, 0, utf16_buf, utf16_length, NULL, 0, NULL, NULL);
        if(ansi_length == 0) {
            lua_pushliteral(L, "text encoding error");
            lua_error(L);
        }
        char* ansi_buf = reinterpret_cast<char*>(lua_newuserdata(L, ansi_length));
        ::WideCharToMultiByte(CP_ACP, 0, utf16_buf, utf16_length, ansi_buf, ansi_length, NULL, NULL);
        this_type::push(L, reinterpret_cast<const native_value*>(ansi_buf), ansi_length);
    }
    else {
        return 0;
    }
    return 1;
}

int byte_array::byte_array_to_str(lua_State* L)
{
    impl_type* impl = reinterpret_cast<impl_type*>(luaL_checkudata(L, 1, name));
    if(impl->size == 0) {
        lua_pushliteral(L, "");
        return 1;
    }
    text_encoding text_enc = te_utf8;
    if(!lua_isnone(L, 2)) {
        const char* encoding = luaL_checkstring(L, 2);
        if(std::strcmp(encoding, "utf8") == 0) {
            text_enc = te_utf8;
        }
        else if (std::strcmp(encoding, "utf16le") == 0) {
            text_enc = te_utf16le;
        }
        else if (std::strcmp(encoding, "ansi") == 0){
            text_enc = te_ansi;
        }
        else {
            luaL_argerror(L, 2, "unsuported encoding");
        }
    }
    if(text_enc == te_utf16le) {
        if(impl->size % 2 != 0) {
            lua_pushliteral(L, "text encoding error");
            lua_error(L);
        }
        int utf8_length = ::WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<wchar_t*>(impl->buffer), static_cast<int>(impl->size / 2), NULL, 0, NULL, NULL);
        if(utf8_length == 0) {
            lua_pushliteral(L, "text encoding error");
            lua_error(L);
        }
        char* utf8_buf = reinterpret_cast<char*>(lua_newuserdata(L, utf8_length));
        ::WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<wchar_t*>(impl->buffer), static_cast<int>(impl->size / 2), utf8_buf, utf8_length, NULL, NULL);
        lua_pushlstring(L, reinterpret_cast<const char*>(utf8_buf), utf8_length);
    }
    else if(text_enc == te_ansi) {
        int utf16_length = ::MultiByteToWideChar(CP_ACP, 0, reinterpret_cast<const char*>(impl->buffer), static_cast<int>(impl->size), NULL, 0); 
        if(utf16_length == 0) {
            lua_pushliteral(L, "text encoding error");
            lua_error(L);
        }
        wchar_t* utf16_buf = reinterpret_cast<wchar_t*>(lua_newuserdata(L, utf16_length * (sizeof(wchar_t) / sizeof(char))));
        ::MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(impl->buffer), static_cast<int>(impl->size), utf16_buf, utf16_length);
        int utf8_length = ::WideCharToMultiByte(CP_UTF8, 0, utf16_buf, utf16_length, NULL, 0, NULL, NULL);
        if(utf8_length == 0) {
            lua_pushliteral(L, "text encoding error");
            lua_error(L);
        }
        char* utf8_buf = reinterpret_cast<char*>(lua_newuserdata(L, utf8_length));
        ::WideCharToMultiByte(CP_ACP, 0, utf16_buf, utf16_length, utf8_buf, utf8_length, NULL, NULL);
        lua_pushlstring(L, utf8_buf, utf8_length);
        return 1;
    }
    else {
        assert(text_enc == te_utf8);
        lua_pushlstring(L, reinterpret_cast<const char*>(impl->buffer), impl->size);
    }
    return 1;
}

int byte_array::index(lua_State* L)
{
    if(lua_isnumber(L, 2)) {
        return get(L);
    }
    else {
        const char* key = luaL_checkstring(L, 2);
        if(std::strcmp(key, "get_pointer") == 0) {
            lua_pushcfunction(L, &byte_array::get_pointer);
        }
        else {
            return 0;
        }
        return 1;
    }
}

int byte_array::newindex(lua_State* L)
{
    impl_type* impl = reinterpret_cast<impl_type*>(luaL_checkudata(L, 1, name));
    int index = luaL_checkint(L, 2);
    luaL_argcheck(L, index >= 0 && static_cast<std::size_t>(index) < impl->size, 2, "out of range");
    *(reinterpret_cast<native_value*>(impl->buffer) + index) = static_cast<native_value>(luaL_checknumber(L, 3));
    return 0;
}

int byte_array::to_string(lua_State* L)
{
    impl_type* impl = reinterpret_cast<impl_type*>(luaL_checkudata(L, 1, name));
    lua_pushfstring(L, "%s: %p", name, impl->buffer);
    return 1;
}

int byte_array::concat_operation(lua_State* L)
{
    impl_type* lhs = reinterpret_cast<impl_type*>(luaL_checkudata(L, 1, name));
    impl_type* rhs = reinterpret_cast<impl_type*>(luaL_checkudata(L, 2, name));
    impl_type* impl = reinterpret_cast<impl_type*>(lua_newuserdata(L, lhs->size + rhs->size + sizeof(std::size_t)));
    impl->size = lhs->size + rhs->size;
    if(lhs->size > 0) {
        std::memcpy(impl->buffer, lhs->buffer, lhs->size);
    }
    if(rhs->size > 0) {
        std::memcpy(impl->buffer + lhs->size, rhs->buffer, rhs->size);
    }
    luaL_getmetatable(L, name);
    lua_setmetatable(L, -2);
    return 1;
}

int byte_array::get_pointer(lua_State* L)
{
    impl_type* impl = reinterpret_cast<impl_type*>(luaL_checkudata(L, 1, name));
    ctype_pointer::push(L, impl->buffer);
    return 1;
}

int byte_array::length(lua_State* L)
{
    impl_type* impl = reinterpret_cast<impl_type*>(luaL_checkudata(L, 1, name));
    lua_pushnumber(L, static_cast<lua_Number>(impl->size));
    return 1;
}

int byte_array::get(lua_State* L)
{
    impl_type* impl = reinterpret_cast<impl_type*>(luaL_checkudata(L, 1, name));
    int index = luaL_checkint(L, 2);
    luaL_argcheck(L, index >= 0 && static_cast<std::size_t>(index) < impl->size, 2, "out of range");
    lua_pushnumber(L, *(reinterpret_cast<native_value*>(impl->buffer) + index));
    return 1;
}

void byte_array::push(lua_State* L, std::size_t n, native_value value)
{
    impl_type* impl = reinterpret_cast<impl_type*>(lua_newuserdata(L, n + sizeof(std::size_t)));
    impl->size = n;
    if(n > 0) {
        std::memset(impl->buffer, value, impl->size);
    }
    luaL_getmetatable(L, name);
    lua_setmetatable(L, -2);
}

void byte_array::push(lua_State* L, const native_value* p, std::size_t n)
{
    impl_type* impl = reinterpret_cast<impl_type*>(lua_newuserdata(L, n + sizeof(std::size_t)));
    impl->size = n;
    std::memcpy(impl->buffer, p, impl->size);
    luaL_getmetatable(L, name);
    lua_setmetatable(L, -2);
}

bool byte_array::registe(lua_State* L)
{
    int top = lua_gettop(L);
    lua_newtable(L);
    lua_setfield(L, -2, "byte_array");
    lua_getfield(L, -1, "byte_array");
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
    lua_pushcfunction(L, &this_type::concat_operation);
    lua_setfield(L, -2, "__concat");
    lua_setmetatable(L, -2);
    lua_settop(L, top);
    return true;
}
