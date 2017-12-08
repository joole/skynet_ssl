#include "socket_ssl.h"


SSL_CTX* socket_ssl_init()
{
    SSL_CTX *ctx = 0;
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(SSLv23_server_method());
    if (ctx == 0) {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}


void socket_ssl_loadcertification(SSL_CTX* ctx, const char* cert_file , const char* key_file)
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

void handleHanleShake()
{
    
}