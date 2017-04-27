/*
 *  Response parser
 *  Author: Maarten Vandersteegen
 */

#ifndef RESPONSE_PARSER_H
#define RESPONSE_PARSER_H

#include "build_state.h"

/*
 *  Parse HTTP response
 *  in:             HTTP response string from server
 *  regex_passed:   Regex that matches 'in' when the build has passed
 *  regex_running:  Regex that matches 'in' when the build is running
 *  regex_failed:   Regex that matches 'in' when the build has failed
 *  state:          The resulting build status
 *  return: 0 on success, -1 on failure
 */
int response_parser_get_result(const char *in, const char *regex_passed,
                                                const char *regex_running,
                                                const char *regex_failed,
                                                enum BuildState *state);

#endif /* RESPONSE_PARSER_H */
