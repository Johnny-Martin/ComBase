/*
 *    byte_array.h:
 *    
 *    Copyright (C) 2014 By limhiaoing <blog.poxiao.me> All Rights Reserved.
 *
 */

#ifndef NINVOKE_BYTE_ARRAY_H_
#define NINVOKE_BYTE_ARRAY_H_
#include <cstddef>

struct lua_State;

class byte_array {
private:
    typedef byte_array this_type;
    typedef struct byte_array_data impl_type;
public:
    static const char* name;
    typedef unsigned char native_value;
public:
    // for lua
    static int create(lua_State* L);
    static int str_to_byte_array(lua_State* L);
    static int byte_array_to_str(lua_State* L);
    static int index(lua_State* L);
    static int newindex(lua_State* L);
    static int to_string(lua_State* L);
    static int concat_operation(lua_State* L);
    static int get_pointer(lua_State* L);
    static int length(lua_State* L);
private:
    // for c
    static int get(lua_State* L);
public:
    static void push(lua_State* L, std::size_t n, native_value value = 0);
    static void push(lua_State* L, const native_value* p, std::size_t n);
    static bool registe(lua_State* L);
};

#endif
