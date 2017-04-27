/*
 *  HTTPS client
 *  Author: Maarten Vandersteegen
 */

#ifndef HTTPS_CLIENT_H
#define HTTPS_CLIENT_H

#include <curl/curl.h>

struct HttpsClient
{
    CURL *curl;
    char *response;
    size_t response_size;
    size_t bytes_read;
};

/*
 *  self:                   https_client instance
 *  disable_cert_verify:    disable server certificate verification
 *  returns 0 on success, -1 on failure
 */
int https_client_init(struct HttpsClient *self, int disable_cert_verify);

/*
 *  self:               https_client instance
 *  uri:                Uri + path to target
 *  request_headers:    optional GET request headers
 *  response:           HTTP response body (no headers) from the server
 *  response_size:      buffer size of 'response'
 *  returns 0 on success, -1 on failure or if the HTTP response code was not 200
 */
int https_client_get(struct HttpsClient *self, const char *uri, const struct curl_slist *request_headers,
                                                                char *response, size_t response_size);

/*
 *  self:   https_client instance
 */
void https_client_deinit(struct HttpsClient *self);

#endif /* HTTPS_CLIENT_H */
