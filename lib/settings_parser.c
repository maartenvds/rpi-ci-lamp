/*
 *  Settings parser
 *  Author: Maarten Vandersteege
 */

#include "logging.h"
#include "settings_parser.h"

static int settings_parse_server_config(struct BuildInfo *build_info, config_t *cf, const char *name)
{
    int count, n;
    const char *header;
    struct curl_slist *chunk = NULL;
    config_setting_t *server, *headers;

    server = config_lookup(cf, name);
    if (!server) {
        error("Settings file: failed reading server setting %s\n", name);
        return -1;
    }
    if (!config_setting_lookup_string(server, "regex_passed", &build_info->regex_passed)) {
        error("Settings file: failed reading 'regex_passed' setting from '%s'\n", name);
        return -1;
    }
    if (!config_setting_lookup_string(server, "regex_running", &build_info->regex_running)) {
        error("Settings file: failed reading 'regex_running' setting from '%s'\n", name);
        return -1;
    }
    if (!config_setting_lookup_string(server, "regex_failed", &build_info->regex_failed)) {
        error("Settings file: failed reading 'regex_failed' setting from '%s'\n", name);
        return -1;
    }

    headers = config_setting_get_member(server, "headers");
    if (!headers) {
        error("Settings file: failed reading 'headers' setting from '%s'\n", name);
        return -1;
    }
    count = config_setting_length(headers);
    for (n = 0; n < count; n++) {
        header = config_setting_get_string_elem(headers, n);
        if (!header) {
            error("Settings file: server setting 'headers' must contain only strings\n");
            if (chunk)
                curl_slist_free_all(chunk);
            return -1;
        }
        chunk = curl_slist_append(chunk, header);
    }
    build_info->headers = chunk;
    if (!chunk && count > 0) {
        error("Settings file: failed to create header list\n");
        return -1;
    }

    return 0;
}

int settings_parser_parse_config(const char *filename, struct Settings *settings)
{
    config_t *cf = &settings->cf;
    const config_setting_t *builds, *build;
    const char *servername;
    int count, n;

    config_init(cf);

    if (!config_read_file(cf, filename)) {
        error("Settings file parser: %s:%d - %s\n", config_error_file(cf),
            config_error_line(cf), config_error_text(cf));
        config_destroy(cf);
        return -1;
    }
    if (!config_lookup_int(cf, "interval", &settings->interval)) {
        error("Settings file: failed reading 'interval' setting\n");
        config_destroy(cf);
        return -1;
    }
    builds = config_lookup(cf, "build-info");
    if (!builds) {
        error("Settings file: failed reading 'build-info' setting\n");
        config_destroy(cf);
        return -1;
    }
    count = config_setting_length(builds);
    if (count > SETTINGS_PARSER_BUILDS_SIZE) {
        error("Settings file: max number of 'build-info' entries (%d) exceeded\n",
                SETTINGS_PARSER_BUILDS_SIZE);
        config_destroy(cf);
        return -1;
    }
    settings->builds_count = count;
    for (n = 0; n < count; n++) {
        build = config_setting_get_elem(builds, n);
        if (!build) {
            /* sanity check, but can normally not occure */
            error("Settings file: unknown error\n");
            config_destroy(cf);
            return -1;
        }
        if (!config_setting_lookup_string(build, "url", &settings->builds[n].url)) {
            error("Settings file: failed reading 'url' setting in entry %d\n", n);
            config_destroy(cf);
            return -1;
        }
        if (!config_setting_lookup_string(build, "server", &servername)) {
            error("Settings file: failed reading 'server' setting in entry %d\n", n);
            config_destroy(cf);
            return -1;
        }
        if (settings_parse_server_config(&settings->builds[n], cf, servername) == -1) {
            config_destroy(cf);
            return -1;
        }
    }

    return 0;
}

void settings_parser_destroy(struct Settings *settings)
{
    int n;

    for (n=0; n<settings->builds_count; n++) {
        if (settings->builds[n].headers) {
            curl_slist_free_all(settings->builds[n].headers);
            settings->builds[n].headers = NULL;
        }
    }
    config_destroy(&settings->cf);
}
