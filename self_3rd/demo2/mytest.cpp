#ifdef __cplusplus
extern "C" {
#endif

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#ifdef __cplusplus
}
#endif // __cplusplus

#include <thread>
#include <functional>
#include <chrono>
#include <sys/types.h>
#include <sys/socket.h>

static int ladd(lua_State  *L)
{
    double op1 = lua_tonumber(L, -1);
    double op2 = lua_tonumber(L, -2);
    lua_pushnumber(L, op1 + op2);

    return 1;
}

static int lloop(lua_State* L)
{
    printf("--- loop call\n");
    std::thread t1([](){
        while(true){
            printf("%ld interval call \n", std::this_thread::get_id());
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        }
    });

    t1.detach();
    return 1;
}

static int lstack_dump(lua_State *L)
{
    printf("---begin dump lua stack\n");
    int i = 0;
    int top = lua_gettop(L);
    for(i = 1; i <= top; ++i) {
        int t = lua_type(L, i);
        switch(t)
        {
            case LUA_TSTRING:
            {
                printf("string:%s\n", lua_tostring(L, i));
            }
            break;

            case LUA_TBOOLEAN:
            {
                printf(lua_toboolean(L, i) ? "boolean:true" : "boolean:false");
            }
            break;

            case LUA_TNUMBER:
            {
                printf("number :%g\n", lua_tonumber(L, i));
            }
            break;
            default:{
                printf("other:%s\n", lua_typename(L, t));
            }
            break;
        }
    }

    printf("--- end dump lua stack\n");
    return 1;
}

static int lcall_lua_func(lua_State* L)
{
    lua_getglobal(L, "lua_test");
    lua_call(L, 0, 0);
    return 1;
}

void OnTimer(lua_State *L, int callback)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, callback);
    lua_call(L, 0, 0);
    luaL_unref(L, LUA_REGISTRYINDEX, callback);
}

static int lcallback_test2(lua_State *L)
{
    int delay = luaL_checkinteger(L, 1);
    printf("user input delay :%d\n", delay);
    int param2 = luaL_checkinteger(L, 2);
    printf("user input params2 :%d\n", param2);

    int callback = luaL_ref(L, LUA_REGISTRYINDEX);
    int param4 = luaL_checkinteger(L, 4);
    printf("user input params2 :%d\n", param2);

    std::thread t1([](int da, lua_State *l, int cb){
        if(l == nullptr){
            printf("--- WAF lua VM lost\n");
            return;
        }
        std::this_thread::sleep_for(std::chrono::seconds(da));
        OnTimer(l, cb);
    }, delay, L, callback);
    t1.detach();
    return 1;
}

static int lsock_read(lua_State *L)
{
    int fd = luaL_checkinteger(L, 1);
    char buffer[2048] = {0};
    printf("--- input socket id :%d\n", fd);
    int ret = recv(fd, buffer, sizeof(buffer), 0);
    if(ret > 0) {
        printf("--- received msg :\n\t %s\n", buffer);
    }else{
        printf("--- received msg null\n");
    }
    return 1;
}



extern "C" int luaopen_mytestlib(lua_State* L)
{
    luaL_Reg myFunc[] = {
        {"add", ladd},
        {"loop", lloop},
        {"stack_dump", lstack_dump},
        {"call_lua_func", lcall_lua_func},
        {"callback_test2", lcallback_test2},
        {"sock_read", lsock_read},
        {NULL, NULL},
    };
    luaL_newlib(L, myFunc);
    return 1;
}