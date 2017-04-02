/*
 *  Request parser
 *  Author: Maarten Vandersteegen
 */

#ifndef RESPONSE_PARSER_H
#define RESPONSE_PARSER_H

#include "build_state.h"

/*
 *  Parse HTTP response
 *  in:     HTTP response string from server
 *  passed: 1 if build was successfull, 0 on failure
 *  return: 0 on success, -1 on failure
 */
int response_parser_build_result(const char *in, enum BuildState *state);

#endif /* RESPONSE_PARSER_H */
