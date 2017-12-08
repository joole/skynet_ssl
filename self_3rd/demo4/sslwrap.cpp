#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#ifdef __cplusplus
}
#endif // __cplusplus

#include <openssl/ssl.h>
#include <openssl/err.h>

typedef struct ssl_context {
    SSL_CTX *ctx;
    SSL *client_ssl;
}SSL_CONTEXT;

static SSL_CONTEXT g_context;

SSL_CTX* init_server_ctx()
{
    SSL_CTX *ctx = nullptr;
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(SSLv23_server_method());
    if (ctx == nullptr) {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}

void load_certificates(SSL_CTX* ctx, const char* cert_file, const char* key_file)
{
    if(SSL_CTX_load_verify_locations(ctx, cert_file, key_file) != 1)
        ERR_print_errors_fp(stderr);
    if (SSL_CTX_set_default_verify_paths(ctx) != 1)
        ERR_print_errors_fp(stderr);

    // set the local certificate from certfile
    if(SSL_CTX_use_certificate_file(ctx, cert_file, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        abort();
    }

    // set private key from keyfile(may be the same as certfile)
    if(SSL_CTX_use_PrivateKey_file(ctx, key_file, SSL_FILETYPE_PEM) <= 0){
        ERR_print_errors_fp(stderr);
        abort();
    }

    // verify private key
    if(!SSL_CTX_check_private_key(ctx)) {
        fprintf(stderr, "private key does not math the public certification");
        abort();
    }

    printf("load certificates complete successfully ...\n");
}


static int lndssl_init(lua_State *L)
{
    size_t len = 0;
    const char *cert_file_path = luaL_checklstring(L, 1, &len);
    const char *prvkey_file_path = luaL_checklstring(L, 2, &len);

    g_context.ctx = init_server_ctx();


    load_certificates(g_context.ctx, cert_file_path, prvkey_file_path);

    return 1;
}

static int lndssl_register_client(lua_State *L)
{
    int client_sock = luaL_checkint(L, 1);
    SSL *client_ssl = ssl_new(g_context.ctx);

    g_context.register_new_client(client_sock, client_ssl);

    SSL_accept(client_ssl);

    SSL_set_fd(client_ssl, client_sock);

    return 1;
}

static int lndssl_read(lua_State *L)
{
    int client_sock = luaL_checkint(L, 1);

    g_context.read_sock(client_sock, )

    return 1;
}

static int lndssl_write(lua_State *L)
{
    int client_sock = luaL_checkint(L, 1);
    g_context.write_sock(client_sock, )

    return 1;
}

static int lndssl_close(lua_State *L)
{
    int client_sock = luaL_checkint(L, 1);
    g_context.close_ssl(client_sock);

    return 1;
}




extern "C" int luaopen_sslwrap(lua_State *L)
{
    luaReg func_array[] = {
        {"ndssl_init", lndssl_init},
        {"ndssl_accept", lndssl_accept},
        {NULL, NULL},
    };

    luaL_newlib(L, func_array);
    return 1;
}