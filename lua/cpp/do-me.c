#include <stdio.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

/* the Lua interpreter */
lua_State* L;

int main(int argc, char *argv[])
{
    /* initialize Lua */
    L = lua_open();

    /* load Lua base libraries */
    lua_baselibopen(L);

    /* run the script */
    lua_dofile(L, "do-me.lua");

    /* cleanup Lua */
    lua_close(L);

    return 0;
}
