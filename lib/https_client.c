/*
 *  HTTPS client handler
 *  Author: Maarten Vandersteege
 */

#include "https_client.h"
#include "logging.h"
#include <string.h>

#define MIN(a,b) \
   ({   __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
        _a < _b ? _a : _b; })

static size_t write_function(void *in, size_t size, size_t nmemb, struct HttpsClient *self)
{
    size_t len = MIN(size * nmemb, self->response_size - self->bytes_read - 1);

    memcpy(&self->response[self->bytes_read], in, len);

    self->bytes_read += len;
    self->response[self->bytes_read] = '\0';

    /* actual number of copied bytes */
    return len;
}

int https_client_init(struct HttpsClient *self, int disable_cert_verify)
{
    CURLcode res;
    long verify = disable_cert_verify ? 0L : 1L;

    res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        error("Curl global init failed: %s\n", curl_easy_strerror(res));
        return -1;
    }

    self->curl = curl_easy_init();
    if (!self->curl) {
        error("Curl init failed\n");
        return -1;
    }

    res = curl_easy_setopt(self->curl, CURLOPT_WRITEFUNCTION, write_function);
    if (res != CURLE_OK) {
        error("Failed to register curl write function: %s\n", curl_easy_strerror(res));
        return -1;
    }

    res = curl_easy_setopt(self->curl, CURLOPT_WRITEDATA, self);
    if (res != CURLE_OK) {
        error("Failed to set write function arguments: %s\n", curl_easy_strerror(res));
        return -1;
    }

    res = curl_easy_setopt(self->curl, CURLOPT_SSL_VERIFYPEER, verify);
    if (res != CURLE_OK) {
        error("Failed to enable/disable SSL certificate verification: %s\n", curl_easy_strerror(res));
        return -1;
    }

    return 0;
}

int https_client_get(struct HttpsClient *self, const char *uri, const struct curl_slist *request_headers,
                                                                char *response, size_t response_size)
{
    CURLcode res;
    long http_code = 0;

    res = curl_easy_setopt(self->curl, CURLOPT_URL, uri);
    if (res != CURLE_OK) {
        error("Failed to set uri: %s\n", curl_easy_strerror(res));
        return -1;
    }

    /* construct header */
    if (request_headers) {
        res = curl_easy_setopt(self->curl, CURLOPT_HTTPHEADER, request_headers);
        if (res != CURLE_OK) {
            error("Failed constructing custom header: %s\n", curl_easy_strerror(res));
            return -1;
        }
    }

    /* perform request */
    self->response = response;
    self->response_size = response_size;
    self->bytes_read = 0;
    res = curl_easy_perform(self->curl);
    if (res != CURLE_OK) {
        error("Https request failed: %s\n", curl_easy_strerror(res));
        return -1;
    }

    /* check HTTP response code */
    res = curl_easy_getinfo(self->curl, CURLINFO_RESPONSE_CODE, &http_code);
    if (res != CURLE_OK) {
        error("Failed to get HTTP response code: %s\n", curl_easy_strerror(res));
        return -1;
    }

    if (http_code != 200) {
        error("HTTP response code is not 200, got %ld\n", http_code);
        return -1;
    }

    return 0;
}

void https_client_deinit(struct HttpsClient *self)
{
    if (self->curl) {
        curl_easy_cleanup(self->curl);
        self->curl = NULL;
        curl_global_cleanup();
    }
}
