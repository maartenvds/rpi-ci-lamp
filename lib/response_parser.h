/*
 *  Request parser
 *  Author: Maarten Vandersteege
 */

#ifndef RESPONSE_PARSER_H
#define RESPONSE_PARSER_H

/*
 *  Parse HTTP response
 *  in:     HTTP response string from server
 *  passed: 1 if build was successfull, 0 on failure
 *  return: 0 on success, -1 on failure
 */
int response_parser_build_result(const char *in, int *passed);

#endif /* RESPONSE_PARSER_H */
