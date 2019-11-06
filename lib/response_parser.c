/*
 *  Response parser
 *  Author: Maarten Vandersteege
 */

#include "logging.h"
#include "response_parser.h"
#include <regex.h>

static void regex_report_and_cleanup(int ret, regex_t *regex, const char *regex_name)
{
    char msgbuf[100];

    (void) regerror(ret, regex, msgbuf, sizeof(msgbuf));
    error("Response parser: regex '%s' failed: %s\n", regex_name, msgbuf);
    regfree(regex);
}

static int perform_regex(const char *string, const char *regex_string, const char *regex_name, int *match)
{
    int ret;
    regex_t regex;

    ret = regcomp(&regex, regex_string, REG_EXTENDED|REG_NOSUB);
    if (ret) {
        regex_report_and_cleanup(ret, &regex, regex_name);
        return -1;
    }

    ret = regexec(&regex, string, 0, NULL, 0);
    if (ret == 0) {
        *match = 1;
    } else if (ret == REG_NOMATCH) {
        *match = 0;
    } else {
        regex_report_and_cleanup(ret, &regex, regex_name);
        return -1;
    }

    regfree(&regex);
    return 0;
}

int response_parser_get_result(const char *in, const char *regex_passed,
                                                const char *regex_running,
                                                const char *regex_failed,
                                                enum BuildState *state)
{
    int match_passed, match_running, match_failed;

    if (perform_regex(in, regex_passed, "regex_passed", &match_passed) == -1)
        return -1;
    if (perform_regex(in, regex_running, "regex_running", &match_running) == -1)
        return -1;
    if (perform_regex(in, regex_failed, "regex_failed", &match_failed) == -1)
        return -1;

    if (match_passed)
        *state = BUILD_STATE_PASSED;
    else if (match_running)
        *state = BUILD_STATE_RUNNING;
    else if (match_failed)
        *state = BUILD_STATE_FAILED;
    else {
        error("No regex matched the following response: %s\n", in);
        return -1;
    }

    return 0;
}
