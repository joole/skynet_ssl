#include "ndsslmanager.h"
#include <iostream>
#include <sstream>

NdSSLManager::NdSSLManager()
{

}

NdSSLManager::~NdSSLManager()
{

}

SSL_CTX* NdSSLManager::init_server_ctx()
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

void NdSSLManager::load_certificates(SSL_CTX* ctx, const char* cert_file, const char* key_file)
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


bool NdSSLManager::ssl_server_init(const std::string& cert_fpath, const std::string& prvk_fpath)
{
    if(m_server_ctx != nullptr) {
        return false;
    }

    m_server_ctx = init_server_ctx();
    if(m_server_ctx == nullptr) {
        return false;
    }

    load_certificates(m_server_ctx, cert_fpath.c_str(), prvk_fpath.c_str());

    return true;
}

bool NdSSLMananger::ssl_client_new(int fd)
{
    if(!m_prepare_done)
        return false;

    SSL* client_ssl = SSL_new(m_server_ctx);
    if(client_ssl == nullptr) {
        return false;
    }
    SSL_set_fd(client_ssl, fd);
    if (SSL_accept(client_ssl) == -1) {
        return false;
    }
    else{
        show_cert();
    }
    register_to_mgr(fd, client_ssl);

    return true;
}

bool NdSSLManager::ssl_client_read(int sockid, char * buf, size_t buf_size)
{
    if(!m_prepare_done){
        return false;
    }
    auto client_ssl = get_ssl_by_sock(sockid);
    auto ret = SSL_read(client_ssl, buf, buf_size);
    if(ret <= 0) {
        return false;
    }
    return true;
}

bool NdSSLManager::ssl_client_write(int sockid, const char* buf, size_t buf_size)
{
    if(!m_prepare_done) {
        return false;
    }
    SSL* ssl = get_ssl_by_sock(sockid);
    if(ssl == nullptr) {
        return false;
    }
    SSL_write(ssl, buf, buf_size);
    return true;
}

bool NdSSLManager::register_to_mgr(int fd, SSL* ssl)
{
    return false;
}

SSL* NdSSLManager::get_ssl_by_sock(int fd)
{
    return nullptr;
}

void NdSSLManager::show_certs(SSL* ssl) {
    X509* cert;
    char* line;
    cert = SSL_get_peer_certificate(ssl); // get cerficates(if available)

    if(cert != nullptr) {
        printf("server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), nullptr, 0);
        printf("subject:%s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), nullptr, 0);
        printf("Issuer : %s\n", line);
        free(line);
        X509_free(cert);
    }
    else{
        printf("no certificates.\n");
    }
}
