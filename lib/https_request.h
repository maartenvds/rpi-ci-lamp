/*
 *  HTTPS request handler
 *  Author: Maarten Vandersteege
 */

#ifndef HTTPS_REQUEST_H
#define HTTPS_REQUEST_H

#include "debug.h"
#include <openssl/ssl.h>
#include <sys/socket.h>
#include <arpa/inet.h>

struct HttpsRequest
{
    SSL_CTX *ssl_ctx;
    struct sockaddr_in server;
};

/*
 *  self:   https_request instance
 *  uri:    server uri to connect to
 */
int https_request_init(struct HttpsRequest *self, const char* uri);

/*
 *  self:           https_request instance
 *  request:        HTTP request header
 *  response:       HTTP response from the server
 *  response_size:  buffer size of 'response'
 */
int https_request_get(struct HttpsRequest *self, const char *request, char *response, size_t response_size);

/*
 *  self:   https_request instance
 */
void https_request_deinit(struct HttpsRequest *self);

#endif /* HTTPS_REQUEST_H */
