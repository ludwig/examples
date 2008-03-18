#include <stdio.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"


/* the Lua interpreter */
lua_State* L;


/* The function we'll call from the lua script */
static int average(lua_State *L)
{
    /* get number of arguments */
    int n = lua_gettop(L);
    double sum = 0;
    int i;

    /* loop through each argument */
    for (i = 1; i <= n; i++)
    {
        if (!lua_isnumber(L, i))
        {
            lua_pushstring(L, "Incorrect argument to 'average'");
            lua_error(L);
        }

        /* total the arguments */
        sum += lua_tonumber(L, i);
    }

    /* push the average */
    lua_pushnumber(L, sum/n);

    /* push the sum */
    lua_pushnumber(L, sum);

    /* return the number of results */
    return 2;
}


int main(int argc, char *argv[])
{
    /* initialize Lua */
    L = lua_open();

    /* load Lua base libraries */
    lua_baselibopen(L);

    /* register our function */
    lua_register(L, "average", average);

    /* run the script */
    lua_dofile(L, "average.lua");

    /* cleanup Lua */
    lua_close(L);

    return 0;
}

