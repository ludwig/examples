#include <stdio.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

/* the Lua interpreter */
lua_State* L;

int main(int argc, char *argv[])
{
    int sum;

    /* initialize Lua */
    L = lua_open();

    /* load Lua base libraries */
    lua_baselibopen(L);

    /* load the script */
    lua_dofile(L, "add.lua");

    /* the function name */
    lua_getglobal(L, "add");

    /* the first argument */
    lua_pushnumber(L, 41);

    /* the second argument */
    lua_pushnumber(L, 22);

    /* call the function with 2 arguments, return 1 result */
    lua_call(L, 2, 1);

    /* get the result */
    sum = (int) lua_tonumber(L, -1);
    lua_pop(L, 1);

    /* print the result */
    printf("The result is %d\n", sum);

    /* cleanup Lua */
    lua_close(L);

    return 0;
}
