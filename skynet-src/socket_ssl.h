#ifndef  __SOCKET_SSL_H__
#define  __SOCKET_SSL_H__

#include <openssl/ssl.h>
#include <openssl/err.h>


SSL_CTX* socket_ssl_init();

void socket_ssl_loadcertification(SSL_CTX* ctx, const char* cerm_key_file , const char* prv_key_file);



#endif // __SOCKET_SSL_H__