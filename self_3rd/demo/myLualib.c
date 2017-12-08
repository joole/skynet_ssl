#include <lua.h>
#include <lauxlib.h>
#include <stdio.h>
#include <unistd.h>

static int ltest1(lua_State *L) {
    int num = luaL_checkinteger(L, 1);
    printf("--- ltest1, num:%d\n", num);
    return 0;
}

static int ltest2(lua_State *L) {
    size_t len = 0;
    const char * msg = luaL_checklstring(L, 1, &len);
    printf("--- ltest2, msg:%s, len:%d\n", msg, len);
    return 0;
}

static int ltest3(lua_State *L) {
    size_t len = 0;
    int num = luaL_checkinteger(L, 1);
    const char * msg = luaL_checklstring(L, 2, &len);
    printf("--- ltest3, num:%d, msg:%s, len:%d\n", num, msg, len);
    return 0;
}

static int lloop1(lua_State * L)
{

    int num = luaL_checkinteger(L, 1);
    printf("--- lloop1 loop begin\n");
    return 0;
}

int luaopen_myLualib(lua_State *L) {

    luaL_Reg l[] = {
        { "test1", ltest1 },
        { "test2", ltest2 },
        { "test3", ltest3 },
        { "loop1", lloop1 },
        { NULL, NULL },
    };
    luaL_newlib(L, l);

    return 1;
}