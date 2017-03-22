#include "https_request.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int i;
    struct HttpsRequest https;
    const char *request = "GET /repos/travis-ci/travis-api/builds HTTP/1.1\r\n" \
                            "User-Agent: MyClient/1.0.0\r\n" \
                            "Accept: application/vnd.travis-ci.2+json\r\n" \
                            "Host: api.travis-ci.org\r\n\r\n";
    char response[4000];
    
    if (https_request_init(&https, "api.travis-ci.org") == -1) {
        debug("Init failed\n");
        https_request_deinit(&https);
        return -1;
    }

    for (i=0;i<3;i++) {
        if (https_request_get(&https, request, response, 4000) == -1) {
            debug("Request failed\n");
            https_request_deinit(&https);
            return -1;
        }
        printf("Response: %s\n", response);
        sleep(2);
    }

    https_request_deinit(&https);
    return 0;
}
