/*
 *  Main application code
 *  Author: Maarten Vandersteege
 */
#include "application.h"
#include "response_parser.h"
#include "read_file.h"
#include "lamp_io.h"
#include "logging.h"
#include <string.h>
#include <stdlib.h>

#define MAX_REPONSE_SIZE    4000
#define REQUEST_TEMPLATE   "GET /repos/%s/branches/%s HTTP/1.1\r\n" \
                            "User-Agent: MyClient/1.0.0\r\n" \
                            "Accept: application/vnd.travis-ci.2+json\r\n" \
                            "Host: api.travis-ci.org\r\n\r\n"

int application_init(struct Application *self, const char *settings_filename, const char *uri)
{
    self->settings_filename = settings_filename;
    self->settings.interval = 1;
    self->settings.repo_count = 0;
    self->lamp_state = LAMP_OFF;

    if (https_request_init(&self->https, uri) == -1) {
        https_request_deinit(&self->https);
        return -1;
    }

    return 0;
}

void application_deinit(struct Application *self)
{
    https_request_deinit(&self->https);
}

static int application_routine(struct Application *self)
{
    int i, res;
    int passed;
    const char *request_fmt = REQUEST_TEMPLATE;
    char *request, *settings_str;
    char response[MAX_REPONSE_SIZE];

    /* read settings */
    settings_str = read_file(self->settings_filename);
    if (!settings_str) {
        error("Failed to read settings file %s\n", self->settings_filename);
        return -1;
    }

    /* parse settings */
    res = settings_parser_get_settings(settings_str, &self->settings);
    free(settings_str);
    if (res == -1) {
        error("Failed to parse settings file %s\n", self->settings_filename);
        return -1;
    }

    /* loop over all repository entries */
    for (i=0; i<self->settings.repo_count; i++) {
        char *name = self->settings.repos[i].name;
        char *branch = self->settings.repos[i].branch;

        /* construct HTTP request */
        res = asprintf(&request, request_fmt, name, branch);
        if (res <= 0) {
            error("Failed constructing HTTP request\n");
            return -1;
        }

        /* perform HTTP request */
        res = https_request_get(&self->https, request, response, MAX_REPONSE_SIZE);
        free(request);
        if (res == -1) {
            error("Request for '%s','%s' failed\n", name, branch);
            return -1;
        }

        /* parse HTTP response */
        if (response_parser_build_result(response, &passed) == -1) {
            error("Failed parsing response from '%s','%s'\n", name, branch);
            return -1;
        }

        /* calculate lamp state */
        if (!passed) {
            self->lamp_state = LAMP_RED;
            break;                      /* some build failed, no use to check other builds */
        }
    }

    return 0;
}

int application_run(struct Application *self, enum LampIoState *lamp_state)
{
    self->lamp_state = LAMP_GREEN;

    if (application_routine(self) == -1) {
        self->lamp_state = LAMP_ERROR;
    }

    *lamp_state = self->lamp_state;

    return self->settings.interval;
}
