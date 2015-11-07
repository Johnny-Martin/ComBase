/*
 *    main.cpp:
 *    
 *    Copyright (C) 2014 By limhiaoing <blog.poxiao.me> All Rights Reserved.
 *
 */

#include <iostream>
#include <lua.hpp>

#include "ninvoke.h"

int main(int argc, char* argv[])
{
    if(argc != 2) {
        std::cout << "usage: \n\tNInvoke.exe script_file" << std::endl;
        return 0;
    }
    const char* file = argv[1];
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    luaopen_ninvoke(L);
    lua_setglobal(L, "NInvoke");
    if(luaL_dofile(L, file) != 0) {
         std::cerr << lua_tostring(L, -1) << std::endl;
     }
    lua_close(L);
}
