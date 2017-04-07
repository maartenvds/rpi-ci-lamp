/*
 *  Response parser
 *  Author: Maarten Vandersteege
 */

#include "logging.h"
#include "response_parser.h"
#include <string.h>

int response_parser_build_result(const char *in, enum BuildState *state)
{
    char *token_p;

    /* search for 'state' token in HTTP body */
    const char *state_token = "\"state\":\"";
    token_p = strstr(in, state_token);
    if (!token_p) {
        error("No 'state' token found in HTTP body\n");
        return -1;
    }

    token_p += strlen(state_token);

    if (strncmp(token_p, "created", 7) == 0 ||
        strncmp(token_p, "started", 7) == 0)
        *state = BUILD_STATE_RUNNING;
    else if (strncmp(token_p, "passed", 6) == 0)
        *state = BUILD_STATE_PASSED;
    else
        *state = BUILD_STATE_FAILED;

    return 0;
}
