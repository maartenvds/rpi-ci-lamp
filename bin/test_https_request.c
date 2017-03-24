#include "https_request.h"
#include "response_parser.h"
#include "logging.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
    int i;
    int passed;
    struct HttpsRequest https;
    const char *request = "GET /repos/travis-ci/travis-api/branches/meat-job-log-id HTTP/1.1\r\n" \
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
            error("Request failed\n");
            https_request_deinit(&https);
            return -1;
        }

        if (response_parser_build_result(response, &passed) == -1) {
            error("Failed parsing request\n");
            https_request_deinit(&https);
            return -1;
        }
        printf("result = %s\n", response);
        sleep(2);
    }

    https_request_deinit(&https);
    return 0;
}
