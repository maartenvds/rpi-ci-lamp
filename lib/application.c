/*
 *  Main application code
 *  Author: Maarten Vandersteegen
 */
#include "application.h"
#include "response_parser.h"
#include "read_file.h"
#include "logging.h"
#include <string.h>
#include <stdlib.h>

#define MAX_REPONSE_SIZE    4000
#define REQUEST_TEMPLATE   "GET /repos/%s/branches/%s HTTP/1.0\r\n" \
                            "User-Agent: MyClient/1.0.0\r\n" \
                            "Accept: application/vnd.travis-ci.2+json\r\n" \
                            "Host: api.travis-ci.org\r\n\r\n"

int application_init(struct Application *self, const char *settings_filename, const char *uri, unsigned short port)
{
    self->settings_filename = settings_filename;
    self->settings.interval = 1;
    self->settings.repo_count = 0;
    lamp_control_init(&self->lamp_control);

    if (https_request_init(&self->https, uri, port) == -1)
        return -1;

    return 0;
}

void application_deinit(struct Application *self)
{
    https_request_deinit(&self->https);
}

static void accumulate_build_state(enum BuildState *aggregate_build_state, enum BuildState build_state)
{
    const int map[3][3] = {
        /*  failed                  running                 passed  -> build_state */
        {   BUILD_STATE_FAILED,     BUILD_STATE_RUNNING,    BUILD_STATE_FAILED  },  /* failed */
        {   BUILD_STATE_RUNNING,    BUILD_STATE_RUNNING,    BUILD_STATE_RUNNING },  /* running */
        {   BUILD_STATE_FAILED,     BUILD_STATE_RUNNING,    BUILD_STATE_PASSED  }}; /* passed */

    *aggregate_build_state = map[*aggregate_build_state][build_state];
}

static int application_routine(struct Application *self, enum BuildState *aggregate_build_state)
{
    int i, res;
    enum BuildState build_state;
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

    *aggregate_build_state = BUILD_STATE_PASSED; /* initial state */

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
        if (response_parser_build_result(response, &build_state) == -1) {
            error("Failed parsing response from '%s','%s'\n", name, branch);
            return -1;
        }

        accumulate_build_state(aggregate_build_state, build_state);
    }

    return 0;
}

int application_run(struct Application *self)
{
    enum BuildState aggregate_build_state;

    if (application_routine(self, &aggregate_build_state) == 0)
        lamp_control_set_state(&self->lamp_control, aggregate_build_state);
    else
        lamp_control_signal_error(&self->lamp_control);

    return self->settings.interval;
}
