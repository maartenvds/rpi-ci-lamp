/*
 *  Settings parser
 *  Author: Maarten Vandersteege
 */

#ifndef SETTINGS_PARSER_H
#define SETTINGS_PARSER_H

#define SETTINGS_PARSER_URI_SIZE   100

struct Settings
{
    char uri[SETTINGS_PARSER_URI_SIZE];     /* uri of the server to fetch the build result */
    int interval;                           /* poll interval in seconds */
};

/*
 *  in:     settings text string
 *  out:    parsed settings
 *  return: zero on success, non zero on failure
 */
int settings_parser_get_settings(const char *in, struct Settings *out);

#endif /* SETTINGS_PARSER_H */
