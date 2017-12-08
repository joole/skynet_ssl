#pragma once

#ifndef SSSL_H
#define SSSL_H


#include "ringbuf.h"
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>


struct ssock;
struct sssl;
struct sssl* sssl_alloc(struct ssock *fd);
void sssl_free(struct sssl *self);
int sssl_connect(struct sssl* self);
int sssl_poll(struct sssl* self, const char *buf, int sz);
int sssl_send(struct sssl* self, const char *buf, int sz);
void sssl_set_state(struct sssl* self, int v);
int sssl_shutdown(struct sssl* self, int how);
int sssl_close(struct sssl *self);
int sssl_clear(struct sssl *self);

#endif // SSL_H