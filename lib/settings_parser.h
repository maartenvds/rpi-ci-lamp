/*
 *  Settings parser
 *  Author: Maarten Vandersteege
 */

#ifndef SETTINGS_PARSER_H
#define SETTINGS_PARSER_H

#include <curl/curl.h>
#include <libconfig.h>

#define SETTINGS_PARSER_BUILDS_SIZE     5

struct BuildInfo
{
    const char *url;
    struct curl_slist *headers;
    const char *regex_passed;
    const char *regex_running;
    const char *regex_failed;
};

struct Settings
{
    config_t cf;    /* reference to complete parsed config, only used internally */
    int interval;
    int builds_count;
    struct BuildInfo builds[SETTINGS_PARSER_BUILDS_SIZE];
};

/*
 *  filename:   configuration file name
 *  settings:   parsed settings struct
 *  return: zero on success, -1 on failure
 */
int settings_parser_parse_config(const char *filename, struct Settings *settings);

/*
 *  Destroy created settings struct
 */
void settings_parser_destroy(struct Settings *settings);

#endif /* SETTINGS_PARSER_H */
