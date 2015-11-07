/*
 *    native_function_wrapper.cpp:
 *    
 *    Copyright (C) 2014 By limhiaoing <blog.poxiao.me> All Rights Reserved.
 *
 */

#include "native_function_wrapper.h"
#include <algorithm>
#include <cassert>
#include <lua.hpp>

const char* native_function_wrapper::name = "NInvoke.native_function";

native_function_wrapper::native_function_wrapper(HMODULE mod, FARPROC fun, const invoke_policy& pol) : module(mod), native_fun(fun), policy(pol)
{
}

#ifdef max
#undef max
#endif max
#ifdef _WIN64
#define SWITCH_CASE_MACRO1(TYPE) \
case TYPE: \
    luaL_checkudata(L, arg_index, type_enum_to_ctype<TYPE>::ctype::name); \
    if(i < 4) { \
        continue; \
    } \
    arg_stack_size += std::max(sizeof(void*), type_enum_to_ctype<TYPE>::ctype::native_type_size()); \
    break;
#else
#define SWITCH_CASE_MACRO1(TYPE) \
case TYPE: \
    luaL_checkudata(L, arg_index, type_enum_to_ctype<TYPE>::ctype::name); \
    arg_stack_size += std::max(sizeof(void*), type_enum_to_ctype<TYPE>::ctype::native_type_size()); \
    break;
#endif

#ifdef _WIN64
#define SWITCH_CASE_MACRO2(TYPE) \
case TYPE: \
    *reinterpret_cast<type_enum_to_ctype<TYPE>::ctype::native_type*>(arg_ptr) = reinterpret_cast<type_enum_to_ctype<TYPE>::ctype*>(luaL_checkudata(L, arg_index, type_enum_to_ctype<TYPE>::ctype::name))->get_value(); \
    break;
#endif

#ifdef _WIN64
#define SWITCH_CASE_MACRO3(TYPE) \
case TYPE: \
    *reinterpret_cast<type_enum_to_ctype<TYPE>::ctype::native_type*>(args_buffer + arg_offset) = reinterpret_cast<type_enum_to_ctype<TYPE>::ctype*>(luaL_checkudata(L, arg_index, type_enum_to_ctype<TYPE>::ctype::name))->get_value(); \
    arg_offset += std::max(sizeof(void*), type_enum_to_ctype<TYPE>::ctype::native_type_size()); \
    break;
#else
#define SWITCH_CASE_MACRO3(TYPE) \
case TYPE: \
    *reinterpret_cast<type_enum_to_ctype<TYPE>::ctype::native_type*>(args_buffer + arg_offset) = reinterpret_cast<type_enum_to_ctype<TYPE>::ctype*>(luaL_checkudata(L, arg_index, type_enum_to_ctype<TYPE>::ctype::name))->get_value(); \
    arg_offset += std::max(sizeof(void*), type_enum_to_ctype<TYPE>::ctype::native_type_size()); \
    break;
#endif

#define SWITCH_CASE_MACRO4(TYPE) \
case TYPE: \
    type_enum_to_ctype<TYPE>::ctype::push(L, static_cast<type_enum_to_ctype<TYPE>::ctype::native_type>(ull_result)); \
    return 1;

int native_function_wrapper::invoke(lua_State* L)
{
    int argc = lua_gettop(L);
    native_function_wrapper** _self = reinterpret_cast<native_function_wrapper**>(luaL_checkudata(L, 1, name));
    native_function_wrapper* self = *_self;
    std::size_t arg_stack_size = 0;
    for(std::size_t i = 0; i < self->policy.arg_type_list.size(); ++i) {
        int arg_index = static_cast<int>(i + 2);
        switch(self->policy.arg_type_list[i]) {
        SWITCH_CASE_MACRO1(type_pointer)
        SWITCH_CASE_MACRO1(type_char)
        SWITCH_CASE_MACRO1(type_signed_char)
        SWITCH_CASE_MACRO1(type_unsigned_char)
        SWITCH_CASE_MACRO1(type_wchar_t)
        SWITCH_CASE_MACRO1(type_short)
        SWITCH_CASE_MACRO1(type_unsigned_short)
        SWITCH_CASE_MACRO1(type_int)
        SWITCH_CASE_MACRO1(type_unsigned_int)
        SWITCH_CASE_MACRO1(type_long)
        SWITCH_CASE_MACRO1(type_unsigned_long)
        SWITCH_CASE_MACRO1(type_long_long)
        SWITCH_CASE_MACRO1(type_unsigned_long_long)
        default:
            assert(false);
            return 0;
        }
    }
    unsigned long long ull_result = 0;
#ifdef _WIN64
    typedef unsigned long long (__fastcall *fun__fastcall)(unsigned long long, unsigned long long, unsigned long long, unsigned long long);
    fun__fastcall proxy_f = fun__fastcall(self->native_fun);
    unsigned long long ull_arg1 = 0;
    unsigned long long ull_arg2 = 0;
    unsigned long long ull_arg3 = 0;
    unsigned long long ull_arg4 = 0;
    for(std::size_t i = 0; i < 4 && i < self->policy.arg_type_list.size(); ++i) {
        int arg_index = static_cast<int>(i + 2);
        unsigned long long* arg_ptr = &ull_arg1;
        if(i == 1) {
            arg_ptr = &ull_arg2;
        }
        else if(i == 2) {
            arg_ptr = &ull_arg3;
        }
        else if(i == 3) {
            arg_ptr = &ull_arg4;
        }
        switch(self->policy.arg_type_list[i]) {
        SWITCH_CASE_MACRO2(type_pointer)
        SWITCH_CASE_MACRO2(type_char)
        SWITCH_CASE_MACRO2(type_signed_char)
        SWITCH_CASE_MACRO2(type_unsigned_char)
        SWITCH_CASE_MACRO2(type_wchar_t)
        SWITCH_CASE_MACRO2(type_short)
        SWITCH_CASE_MACRO2(type_unsigned_short)
        SWITCH_CASE_MACRO2(type_int)
        SWITCH_CASE_MACRO2(type_unsigned_int)
        SWITCH_CASE_MACRO2(type_long)
        SWITCH_CASE_MACRO2(type_unsigned_long)
        SWITCH_CASE_MACRO2(type_long_long)
        SWITCH_CASE_MACRO2(type_unsigned_long_long)
        default:
            assert(false);
            return 0;
        }
    }
#else
    typedef unsigned long long (__stdcall *fun__stdcall)();
    fun__stdcall proxy_f = fun__stdcall(self->native_fun);
#endif
    if(arg_stack_size == 0) {
#ifdef _WIN64
        ull_result = proxy_f(ull_arg1, ull_arg2, ull_arg3, ull_arg4);
#else
        ull_result = proxy_f();
#endif
    }
    else {
        char* args_buffer = NULL;
        try {
            args_buffer = new char[arg_stack_size];
        }
        catch(const std::bad_alloc&) {
            lua_pushliteral(L, "out of memery");
            lua_error(L);
        }
        std::size_t arg_offset = 0;
#ifdef _WIN64
        for(std::size_t i = 4; i < self->policy.arg_type_list.size(); ++i) {
#else
        for(std::size_t i = 0; i < self->policy.arg_type_list.size(); ++i) {
#endif
            int arg_index = static_cast<int>(i + 2);
            switch(self->policy.arg_type_list[i]) {
            SWITCH_CASE_MACRO3(type_pointer)
            SWITCH_CASE_MACRO3(type_char)
            SWITCH_CASE_MACRO3(type_signed_char)
            SWITCH_CASE_MACRO3(type_unsigned_char)
            SWITCH_CASE_MACRO3(type_wchar_t)
            SWITCH_CASE_MACRO3(type_short)
            SWITCH_CASE_MACRO3(type_unsigned_short)
            SWITCH_CASE_MACRO3(type_int)
            SWITCH_CASE_MACRO3(type_unsigned_int)
            SWITCH_CASE_MACRO3(type_long)
            SWITCH_CASE_MACRO3(type_unsigned_long)
            SWITCH_CASE_MACRO3(type_long_long)
            SWITCH_CASE_MACRO3(type_unsigned_long_long)
            default:
                assert(false);
                return 0;
            }
        }
        void* arg_stack = reinterpret_cast<char*>(alloca(arg_stack_size));
        std::memcpy(arg_stack, args_buffer, arg_stack_size);
        delete[] args_buffer;
#ifdef _WIN64
        ull_result = proxy_f(ull_arg1, ull_arg2, ull_arg3, ull_arg4);
#else
        ull_result = proxy_f();
#endif
    }
    assert(ctype_unsigned_long::native_type_size() <= 4 && ctype_unsigned_long_long::native_type_size() > 4);
    if(self->policy.return_type != type_unsigned_long_long || self->policy.return_type != type_long_long) {
        ull_result &= 0xffffffffffffffffull;
    }
    switch(self->policy.return_type) {
    case type_void:
        return 0;
    SWITCH_CASE_MACRO4(type_char)
    SWITCH_CASE_MACRO4(type_signed_char)
    SWITCH_CASE_MACRO4(type_unsigned_char)
    SWITCH_CASE_MACRO4(type_short)
    SWITCH_CASE_MACRO4(type_unsigned_short)
    SWITCH_CASE_MACRO4(type_int)
    SWITCH_CASE_MACRO4(type_unsigned_int)
    SWITCH_CASE_MACRO4(type_long)
    SWITCH_CASE_MACRO4(type_unsigned_long)
    SWITCH_CASE_MACRO4(type_long_long)
    SWITCH_CASE_MACRO4(type_unsigned_long_long)
    case type_pointer:
        ctype_pointer::push(L, reinterpret_cast<ctype_pointer::native_type>(ull_result));
        return 1;
    default:
        assert(false);
        return 0;
    }
    return 0;
}

int native_function_wrapper::lua_gc(lua_State* L)
{
    native_function_wrapper** _self = reinterpret_cast<native_function_wrapper**>(luaL_checkudata(L, 1, name));
    native_function_wrapper* self = *_self;
    if(self->module != NULL) {
        ::FreeLibrary(self->module);
    }
    self->module = NULL;
    self->native_fun = NULL;
    delete self;
    return 0;
}

int native_function_wrapper::to_string(lua_State* L)
{
    native_function_wrapper** _self = reinterpret_cast<native_function_wrapper**>(luaL_checkudata(L, 1, name));
    native_function_wrapper* self = *_self;
    lua_pushfstring(L, "%s: %p", name, self->native_fun);
    return 1;
}

bool native_function_wrapper::push(lua_State* L, const wchar_t* dll, const char* func_name, const invoke_policy& policy)
{
    HMODULE hModule = ::LoadLibrary(dll);
    if(hModule == NULL) {
        return false;
    }
    FARPROC pFunc = ::GetProcAddress(hModule, func_name);
    if(pFunc == NULL) {
        ::FreeLibrary(hModule);
        return false;
    }
    native_function_wrapper* nfw = NULL;
    try {
        nfw = new native_function_wrapper(hModule, pFunc, policy);
    }
    catch(const std::bad_alloc&) {
        ::FreeLibrary(hModule);
        return false;
    }
    native_function_wrapper** self = reinterpret_cast<native_function_wrapper**>(lua_newuserdata(L, sizeof(native_function_wrapper*)));
    *self = nfw;
    luaL_getmetatable(L, name);
    lua_setmetatable(L, -2);
    return true;
}

bool native_function_wrapper::registe(lua_State* L)
{
    int top = lua_gettop(L);
    lua_newtable(L);
    lua_setfield(L, -2, "native_function");
    lua_getfield(L, -1, "native_function");

    luaL_newmetatable(L, name);
    lua_pushcfunction(L, &native_function_wrapper::invoke);
    lua_setfield(L, -2, "__call");
    lua_pushcfunction(L, &native_function_wrapper::to_string);
    lua_setfield(L, -2, "__tostring");
    lua_pushcfunction(L, &native_function_wrapper::lua_gc);
    lua_setfield(L, -2, "__gc");
    lua_setmetatable(L, -2);
    lua_settop(L, top);
    return true;
}
