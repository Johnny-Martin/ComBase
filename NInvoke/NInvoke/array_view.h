/*
 *    array_view.h:
 *    
 *    Copyright (C) 2014 By limhiaoing <blog.poxiao.me> All Rights Reserved.
 *
 */

#ifndef NINVOKE_ARRARY_VIEW_H_
#define NINVOKE_ARRARY_VIEW_H_
#include "nctypes.h"

struct lua_State;

class array_view {
    typedef array_view this_type;
    typedef struct array_view_impl impl_type;
public:
    static const char* name;
public:
    // for lua
    static int create(lua_State* L);
    static int index(lua_State* L);
    static int newindex(lua_State* L);
    static int length(lua_State* L);
    static int to_string(lua_State* L);
    static int get_pointer(lua_State* L);
private:
    // for c
    static bool is_valid_value_type(ctypes type);
public:
    static bool registe(lua_State* L);
};

#endif
