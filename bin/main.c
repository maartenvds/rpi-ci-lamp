/*
 *  Main program CI-lamp
 *  Author: Maarten Vandersteege
 */

#include "logging.h"
#include "lamp_io.h"
#include "application.h"
#include <signal.h>
#include <unistd.h>

static struct Application app;

void int_handler(int dummy)
{
    application_deinit(&app);
    lamp_io_set_state(LAMP_OFF);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sleep_time;
    enum LampIoState lamp_state;

    signal(SIGINT, int_handler);

    if (application_init(&app, "settings.json", "api.travis-ci.org") == -1) {
        error("Initialization failed\n");
        return -1;
    }

    while (1) {
        sleep_time = application_run(&app, &lamp_state);
        lamp_io_set_state(lamp_state);
        sleep(sleep_time);
    }

    return 0;
}
