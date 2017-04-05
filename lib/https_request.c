/*
 *  HTTPS request handler
 *  Author: Maarten Vandersteege
 */

#include "https_request.h"
#include "logging.h"

#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <openssl/err.h>

int https_request_init(struct HttpsRequest *self, const char* uri)
{
    struct hostent *host;

    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    (void)SSL_library_init();
    self->ssl_ctx = SSL_CTX_new (SSLv23_client_method());

    if (!self->ssl_ctx) {
        error("Failed to create new SSL context\n");
        return -1;
    }

    /* resolve uri */
    if ((host = gethostbyname(uri)) == NULL) {
        error("Failed to resolve hostname '%s'\n", uri);
        return -1;
    }

    memset(&self->server, 0, sizeof(self->server));
    self->server.sin_family = AF_INET;
    memcpy(&self->server.sin_addr, host->h_addr, host->h_length);
    self->server.sin_port = htons( 443 );

    return 0;
}

int https_request_get(struct HttpsRequest *self, const char *request, char *response, size_t response_size)
{
    size_t len;
    int res = 0;
    int sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error("Failed to create a socket: %s\n", strerror(errno));
        return -1;
    }

    if (connect(sock , (struct sockaddr *)&self->server, sizeof(self->server)) < 0) {
        error("Failed to open connection: %s\n", strerror(errno));
        res = -1;
        goto close_socket;
    }

    SSL *conn = SSL_new(self->ssl_ctx);

    if (!conn) {
        error("Failed to create new SSL socket\n");
        res = -1;
        goto shutdown_ssl_connection;
    }

    if (!SSL_set_fd(conn, sock)) {
        error("Failed to set SSL socket\n");
        res = -1;
        goto shutdown_ssl_connection;
    }

    if (!SSL_connect(conn)) {
        error("Failed to open SSL connection\n");
        res = -1;
        goto shutdown_ssl_connection;
    }

    if (SSL_write(conn, request, strlen(request)) <= 0) {
        error("Failed to send request\n");
        res = -1;
        goto shutdown_ssl_connection;
    }

    len = SSL_read(conn, response, response_size);
    if (len <= 0) {
        error("Failed to receive response\n");
        res = -1;
        goto shutdown_ssl_connection;

    } else if (len == response_size) {
        response[len-1] = '\0';
    } else {
        response[len] = '\0';
    }

shutdown_ssl_connection:
    (void)SSL_shutdown(conn);

    SSL_free(conn);

close_socket:
    close(sock);

    return res;
}

void https_request_deinit(struct HttpsRequest *self)
{
    if (self->ssl_ctx) {
        SSL_CTX_free(self->ssl_ctx);
        self->ssl_ctx = NULL;
    }
}
