/*
 *  Response parser
 *  Author: Maarten Vandersteege
 */

#include "logging.h"
#include "response_parser.h"
#include <string.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

int response_parser_build_result(const char *in, int *passed)
{
    const char *expected = "HTTP/1.1 200 OK";
    char *token_p, *header_end;
    size_t compare_len = MIN(strlen(expected), strlen(in));

    /* check if HTTP response is OK */
    if (strncmp(in, expected, compare_len) != 0) {
        error("Invalid HTTP response: %s\n", in);
        return -1;
    }

    /* find end of HTTP header */
    const char *end_of_http_header_token = "\r\n\r\n";
    header_end = strstr(in, end_of_http_header_token);
    if (!header_end) {
        error("No end of HTTP header token found\n");
        return -1;
    }

    /* search for 'state' token in HTTP body */
    const char *state_token = "\"state\":\"";
    token_p = strstr(header_end, state_token);
    if (!token_p) {
        error("No 'state' token found in HTTP body\n");
        return -1;
    }
    
    token_p += strlen(state_token);

    if (strncmp(token_p, "passed", 6) == 0)
        *passed = 1;
    else
        *passed = 0;

    return 0;
}
