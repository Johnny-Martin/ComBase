/*
 *    nctypes.cpp:
 *    
 *    Copyright (C) 2014 By limhiaoing <blog.poxiao.me> All Rights Reserved.
 *
 */

#include "nctypes.h"
#include <string>

const char* ctype_wchar_t::name = "NInvoke.ctypes.wchar_t";
const char* ctype_char::name = "NInvoke.ctypes.char";
const char* ctype_signed_char::name = "NInvoke.ctypes.signed_char";
const char* ctype_unsigned_char::name = "NInvoke.ctypes.unsigned_char";
const char* ctype_short::name = "NInvoke.ctypes.short";
const char* ctype_unsigned_short::name = "NInvoke.ctypes.unsigned_short";
const char* ctype_long::name = "NInvoke.ctypes.long";
const char* ctype_unsigned_long::name = "NInvoke.ctypes.unsigned_long";
const char* ctype_int::name = "NInvoke.ctypes.int";
const char* ctype_unsigned_int::name = "NInvoke.ctypes.unsigned_int";
const char* ctype_long_long::name = "NInvoke.ctypes.long_long";
const char* ctype_unsigned_long_long::name = "NInvoke.ctypes.unsigned_long_long";
const char* ctype_pointer::name = "NInvoke.ctypes.pointer";

ctypes type_name_to_ctype(const char* type_name)
{
    if(std::strcmp(type_name, "void") == 0) {
        return type_void;
    }
    else if(std::strcmp(type_name, "pointer") == 0) {
        return type_pointer;
    }
    else if(std::strcmp(type_name, "char") == 0) {
        return type_char;
    }
    else if(std::strcmp(type_name, "signed char") == 0) {
        return type_signed_char;
    }
    else if(std::strcmp(type_name, "unsigned char") == 0) {
        return type_unsigned_char;
    }
    else if(std::strcmp(type_name, "wchar_t") == 0) {
        return type_unsigned_char;
    }
    else if(std::strcmp(type_name, "short") == 0) {
        return type_short;
    }
    else if(std::strcmp(type_name, "unsigned short") == 0) {
        return type_unsigned_short;
    }
    else if(std::strcmp(type_name, "int") == 0) {
        return type_int;
    }
    else if(std::strcmp(type_name, "unsigned int") == 0) {
        return type_unsigned_int;
    }
    else if(std::strcmp(type_name, "long") == 0) {
        return type_long;
    }
    else if(std::strcmp(type_name, "unsigned long") == 0) {
        return type_unsigned_long;
    }
    else if(std::strcmp(type_name, "long long") == 0) {
        return type_long_long;
    }
    else if(std::strcmp(type_name, "unsigned long long") == 0) {
        return type_unsigned_long_long;
    }
    else {
        return type_end;
    }
}

int ctype_pointer::create(lua_State* L)
{
    native_type value = reinterpret_cast<native_type>(static_cast<std::size_t>(luaL_checknumber(L, 1)));
    this_type::push(L, value);
    return 1;
}

int ctype_pointer::type_size(lua_State* L)
{
    lua_pushnumber(L, static_cast<lua_Number>(sizeof(native_type)));
    return 1;
}

int ctype_pointer::type_name(lua_State* L)
{
    lua_pushstring(L, name);
    return 1;
}

int ctype_pointer::to_number(lua_State* L)
{
    this_type* self = reinterpret_cast<this_type*>(luaL_checkudata(L, 1, name));
    lua_pushnumber(L, static_cast<lua_Number>(reinterpret_cast<std::size_t>(self->_value)));
    return 1;
}

int ctype_pointer::to_string(lua_State* L)
{
    this_type* self = reinterpret_cast<this_type*>(luaL_checkudata(L, 1, name));
    lua_pushfstring(L, "%p", self->_value);
    return 1;
}

int ctype_pointer::get_pointer(lua_State* L)
{
    this_type* self = reinterpret_cast<this_type*>(luaL_checkudata(L, 1, name));
    ctype_pointer::push(L, &self->_value);
    return 1;
}

int ctype_pointer::fetch_increase(lua_State* L)
{
    this_type* self = reinterpret_cast<this_type*>(luaL_checkudata(L, 1, name));
    int dis = luaL_checkint(L, 2);
    luaL_argcheck(L, dis >= 0, 2, "invalid value");
    ctype_pointer::push(L, reinterpret_cast<char*>(self->_value) + dis);
    return 1;
}

int ctype_pointer::fetch_decrease(lua_State* L)
{
    this_type* self = reinterpret_cast<this_type*>(luaL_checkudata(L, 1, name));
    int dis = luaL_checkint(L, 2);
    luaL_argcheck(L, dis >= 0, 2, "invalid value");
    ctype_pointer::push(L, reinterpret_cast<char*>(self->_value) - dis);
    return 0;
}

int ctype_pointer::eq_operation(lua_State* L)
{
    lua_pushboolean(L, reinterpret_cast<this_type*>(luaL_checkudata(L, 1, name))->_value == reinterpret_cast<this_type*>(luaL_checkudata(L, 2, name))->_value ? 1 : 0);
    return 1;
}

int ctype_pointer::lt_operation(lua_State* L)
{
    lua_pushboolean(L, reinterpret_cast<this_type*>(luaL_checkudata(L, 1, name))->_value < reinterpret_cast<this_type*>(luaL_checkudata(L, 2, name))->_value ? 1 : 0);
    return 1;
}

int ctype_pointer::le_operation(lua_State* L)
{
    lua_pushboolean(L, reinterpret_cast<this_type*>(luaL_checkudata(L, 1, name))->_value <= reinterpret_cast<this_type*>(luaL_checkudata(L, 2, name))->_value ? 1 : 0);
    return 1;
}

void ctype_pointer::push(lua_State* L, native_type value)
{
    this_type* self = reinterpret_cast<this_type*>(lua_newuserdata(L, sizeof(this_type)));
    self->_value = value;
    luaL_getmetatable(L, name);
    lua_setmetatable(L, -2);
}

std::size_t ctype_pointer::native_type_size()
{
    return sizeof(native_type);
}

bool ctype_pointer::registe(lua_State* L)
{
    int top = lua_gettop(L);
    lua_newtable(L);
    lua_setfield(L, -2, "pointer");
    lua_getfield(L, -1, "pointer");
    lua_pushcfunction(L, &this_type::create);
    lua_setfield(L, -2, "new");
    lua_pushcfunction(L, &this_type::type_size);
    lua_setfield(L, -2, "type_size");
    luaL_newmetatable(L, name);
    lua_pushvalue(L, -1);
    lua_pushcfunction(L, &this_type::to_number);
    lua_setfield(L, -2, "to_number");
    lua_pushcfunction(L, &this_type::get_pointer);
    lua_setfield(L, -2, "get_pointer");

    lua_pushcfunction(L, &this_type::type_name);
    lua_setfield(L, -2, "type_name");

    lua_pushcfunction(L, &this_type::fetch_increase);
    lua_setfield(L, -2, "fetch_increase");
    lua_pushcfunction(L, &this_type::fetch_decrease);
    lua_setfield(L, -2, "fetch_decrease");

    lua_pushcfunction(L, &this_type::deref_as<ctype_char>);
    lua_setfield(L, -2, "deref_as_char");
    lua_pushcfunction(L, &this_type::deref_as<ctype_signed_char>);
    lua_setfield(L, -2, "deref_as_signed_char");
    lua_pushcfunction(L, &this_type::deref_as<ctype_unsigned_char>);
    lua_setfield(L, -2, "deref_as_unsigned_char");
    lua_pushcfunction(L, &this_type::deref_as<ctype_wchar_t>);
    lua_setfield(L, -2, "deref_as_wchar_t");
    lua_pushcfunction(L, &this_type::deref_as<ctype_short>);
    lua_setfield(L, -2, "deref_as_short");
    lua_pushcfunction(L, &this_type::deref_as<ctype_unsigned_short>);
    lua_setfield(L, -2, "deref_as_unsigned_short");
    lua_pushcfunction(L, &this_type::deref_as<ctype_int>);
    lua_setfield(L, -2, "deref_as_int");
    lua_pushcfunction(L, &this_type::deref_as<ctype_unsigned_int>);
    lua_setfield(L, -2, "deref_as_unsigned_int");
    lua_pushcfunction(L, &this_type::deref_as<ctype_long>);
    lua_setfield(L, -2, "deref_as_long");
    lua_pushcfunction(L, &this_type::deref_as<ctype_unsigned_long>);
    lua_setfield(L, -2, "deref_as_unsigned_long");
    lua_pushcfunction(L, &this_type::deref_as<ctype_long_long>);
    lua_setfield(L, -2, "deref_as_long_long");
    lua_pushcfunction(L, &this_type::deref_as<ctype_unsigned_long_long>);
    lua_setfield(L, -2, "deref_as_unsigned_long_long");

    lua_pushcfunction(L, &this_type::deref_as<ctype_pointer>);
    lua_setfield(L, -2, "deref_as_pointer");

    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, &this_type::eq_operation);
    lua_setfield(L, -2, "__eq");
    lua_pushcfunction(L, &this_type::lt_operation);
    lua_setfield(L, -2, "__lt");
    lua_pushcfunction(L, &this_type::le_operation);
    lua_setfield(L, -2, "__le");
    lua_pushcfunction(L, &this_type::to_string);
    lua_setfield(L, -2, "__tostring");
    lua_setmetatable(L, -2);
    lua_settop(L, top);
    return true;
}

ctype_pointer::native_type ctype_pointer::get_value() const
{
    return this->_value;
}
