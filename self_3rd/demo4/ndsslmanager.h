#ifndef  _NDSSLMANANGER_H_
#define  _NDSSLMANANGER_H_


#include <openssl/ssl.h>
#include <openssl/err.h>
#include <map>
#include <string>

class NdSSLManager{
private:
    NdSSLManager();
    ~NdSSLManager();


    SSL_CTX* init_server_ctx();
    void load_certificates(SSL_CTX* ctx, const char* cert_file, const char* key_file);

public:
    bool ssl_server_init(const std::string& cert_fpath, const std::string& prvk_fpath);
    bool ssl_client_new(int fd);
    bool ssl_client_read(int fd, char *buf, int size);
    bool ssl_client_write(int fd, const char *buf, int size);

private:
    std::map<int, SSL*>     m_client_ssls;
    SSL_CTX                 *m_server_ctx;
    bool                    m_prepare_done;
    std::string             m_cert_file;
    std::string             m_prvk_file;
};



#endif //_NDSSLMANANGER_H_