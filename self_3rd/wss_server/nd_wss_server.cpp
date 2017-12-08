#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#ifdef __cplusplus
}
#endif //__cplusplus

static int lstart(lua_State *L)
{
    size_t len = 0;
    const char*  addr = lua_tostring(L, -1);
    int port = lua_tonumber(L, -2);
    const char * cert = luaL_checklstring(L, 1, &len);
    const char*  pkey = luaL_checklstring(L, 2, &len);
    short port = lua_tonumber(L, 3);

    NdWssServer::getInstance()->start(port, cert, pkey);
    return 1;
}

static int lstop(lua_State *L)
{
    NdWssServer::get_instance()->stop();
    return 1;
}

static int lset_onmessage(lua_State *L)
{

}

static int lset_on_opened(lua_State *L)
{

}

static int lset_on_pong(lua_State *L)
{



}

static int lset_on_close(lua_State *L)
{

}

extern "C" luaopen_nd_wsserver(lua_State *L)
{
    luaL_Reg myApi = {
        {"start", lstart},
        {"stop", lstop},
        {NULL, NULL}
    };
    luaL_newlib(L, myApi);
    return 1;
}