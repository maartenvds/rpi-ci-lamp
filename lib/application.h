/*
 *  Main application code
 *  Author: Maarten Vandersteege
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include "build_state.h"
#include "lamp_control.h"
#include "https_request.h"
#include "settings_parser.h"

struct Application
{
    struct HttpsRequest https;
    struct Settings settings;
    const char *settings_filename;
    struct LampControl lamp_control;
};

/*
 *  self:               application instance
 *  settings_filename:  filename of the settings file
 *  uri:                uri of the build server
 *  return: zero on success, non zero on failure
 */
int application_init(struct Application *self, const char *settings_filename, const char *uri);

/*
 *  self:   application instance
 */
void application_deinit(struct Application *self);

/*
 *  self:       application instance
 *  return:     sleep time in seconds
 */
int application_run(struct Application *self);

#endif /* APPLICATION_H */
