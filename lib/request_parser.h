/*
 *  Request parser
 *  Author: Maarten Vandersteege
 */

#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

struct BuildResult
{
    int finished;                   /* non zero when build is finished */
    int outcome;                    /* zero on success, non zero on failure */
};

/*
 *  in:     response string from server
 *  out:    build result information
 *  return: 0 on success, -1 on failure
 */
int request_parser_build_result(const char *in, struct BuildResult *out);

#endif /* REQUEST_PARSER_H */
