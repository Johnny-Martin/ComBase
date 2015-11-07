/*
 *    native_function_wrapper.h:
 *    
 *    Copyright (C) 2014 By limhiaoing <blog.poxiao.me> All Rights Reserved.
 *
 */

#ifndef NINVOKE_NATIVE_FUNCTION_WRAPPER_H_
#define NINVOKE_NATIVE_FUNCTION_WRAPPER_H_

#include "nctypes.h"
#include <vector>
#include <Windows.h>

struct lua_State;

struct invoke_policy {
    std::vector<ctypes> arg_type_list;
    ctypes return_type;
};

class native_function_wrapper {
    HMODULE module;
    FARPROC native_fun;
    invoke_policy policy;
    native_function_wrapper(HMODULE mod, FARPROC fun, const invoke_policy& pol);
public:
    static const char* name;
public:
    // for lua
    static int invoke(lua_State* L);
    static int lua_gc(lua_State* L);

    static int to_string(lua_State* L);
public:
    // for c
    static bool push(lua_State* L, const wchar_t* dll, const char* func_name, const invoke_policy& policy);
    static bool registe(lua_State* L);
};

#endif
