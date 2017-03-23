/*
 *  Settings parser
 *  Author: Maarten Vandersteege
 */

#ifndef SETTINGS_PARSER_H
#define SETTINGS_PARSER_H

#define SETTINGS_PARSER_STRING_SIZE     100
#define SETTINGS_PARSER_REPO_SIZE       5

struct SettingsRepo
{
    char name[SETTINGS_PARSER_STRING_SIZE];
    char branch[SETTINGS_PARSER_STRING_SIZE];
};

struct Settings
{
    int interval;                                               /* poll interval in seconds */
    int repo_count;
    struct SettingsRepo repos[SETTINGS_PARSER_STRING_SIZE];     /* repos to track */
};

/*
 *  in:     settings text string
 *  out:    parsed settings
 *  return: zero on success, non zero on failure
 */
int settings_parser_get_settings(const char *in, struct Settings *out);

#endif /* SETTINGS_PARSER_H */
