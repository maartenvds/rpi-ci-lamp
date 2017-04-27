/*
 *  Main application code
 *  Author: Maarten Vandersteegen
 */
#include "application.h"
#include "response_parser.h"
#include "logging.h"
#include <string.h>
#include <stdlib.h>

#define MAX_RESPONSE_SIZE    4000

int application_init(struct Application *self, const char *settings_filename, int disable_cert_verify)
{
    self->settings_filename = settings_filename;
    self->settings.interval = 1;
    self->settings.builds_count = 0;

    self->lamp_control = lamp_control_init();
    if (!self->lamp_control)
        return -1;

    if (https_client_init(&self->client, disable_cert_verify) == -1)
        return -1;

    return 0;
}

void application_deinit(struct Application *self)
{
    if (self->lamp_control) {
        lamp_control_deinit(self->lamp_control);
        self->lamp_control = NULL;
    }

    https_client_deinit(&self->client);
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
    int i;
    enum BuildState build_state;
    char response[MAX_RESPONSE_SIZE];

    /* parse settings file */
    if (settings_parser_parse_config(self->settings_filename, &self->settings) == -1) {
        error("Failed to parse settings file %s\n", self->settings_filename);
        return -1;
    }

    *aggregate_build_state = BUILD_STATE_PASSED; /* initial state */

    /* loop over all build info entries */
    for (i=0; i<self->settings.builds_count; i++) {
        const struct BuildInfo *build_info = &self->settings.builds[i];

        if (https_client_get(&self->client, build_info->url, build_info->headers,
                response, MAX_RESPONSE_SIZE) == -1) {
            error("Request for '%s' failed\n", build_info->url);
            settings_parser_destroy(&self->settings);
            return -1;
        }

        if (response_parser_get_result(response, build_info->regex_passed,
                build_info->regex_running, build_info->regex_failed, &build_state) == -1) {
            error("Failed parsing response from '%s'\n", build_info->url);
            settings_parser_destroy(&self->settings);
            return -1;
        }

        accumulate_build_state(aggregate_build_state, build_state);
    }

    settings_parser_destroy(&self->settings);
    return 0;
}

int application_run(struct Application *self)
{
    enum BuildState aggregate_build_state;

    if (application_routine(self, &aggregate_build_state) == 0)
        lamp_control_set_state(self->lamp_control, aggregate_build_state);
    else
        lamp_control_signal_error(self->lamp_control);

    return self->settings.interval;
}
