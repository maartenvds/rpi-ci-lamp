/*
 *  Main application code
 *  Author: Maarten Vandersteege
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include "lamp_io.h"
#include "https_request.h"
#include "settings_parser.h"

struct Application
{
    struct HttpsRequest https;
    struct Settings settings;
    const char *settings_filename;
    enum LampIoState lamp_state;
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
 *  lamp_state: new state that the lamp should get
 *  return:     sleep time in seconds
 */
int application_run(struct Application *self, enum LampIoState *lamp_state);

#endif /* APPLICATION_H */
