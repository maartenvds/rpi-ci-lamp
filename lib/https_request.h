/*
 *  HTTPS request handler
 *  Author: Maarten Vandersteege
 */

#ifndef HTTPS_REQUEST_H
#define HTTPS_REQUEST_H

#include <curl/curl.h>

struct HttpsRequest
{
    CURL *curl;
    char *response;
    size_t response_size;
    size_t bytes_read;
};

/*
 *  self:                   https_request instance
 *  disable_cert_verify:    disable server certificate verification
 *  returns 0 on success, -1 on failure
 */
int https_request_init(struct HttpsRequest *self, int disable_cert_verify);

/*
 *  self:               https_request instance
 *  uri:                server uri, format = scheme://host:port/path
 *  request_headers:    Optional HTTP request header list, use NULL if not used
 *  response:           HTTP response body (no headers) from the server
 *  response_size:      buffer size of 'response'
 *  returns 0 on success, -1 on failure or if the HTTP response code was not 200
 */
int https_request_get(struct HttpsRequest *self, const char *uri, struct curl_slist *request_headers,
                                                                char *response, size_t response_size);

/*
 *  self:   https_request instance
 */
void https_request_deinit(struct HttpsRequest *self);

#endif /* HTTPS_REQUEST_H */
