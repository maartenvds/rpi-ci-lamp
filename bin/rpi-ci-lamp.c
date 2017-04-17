/*
 *  Main program CI-lamp
 *  Author: Maarten Vandersteege
 */

#include "logging.h"
#include "application.h"
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

static struct Application app;

void int_handler(int dummy)
{
    (void)dummy;
    lamp_control_off(app.lamp_control);
    application_deinit(&app);
    exit(0);
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    int sleep_time;

    signal(SIGINT, int_handler);

    if (application_init(&app, "settings.json", "https://api.travis-ci.org", 0) == -1) {
        application_deinit(&app);
        error("Initialization failed\n");
        return -1;
    }

    while (1) {
        sleep_time = application_run(&app);
        sleep(sleep_time);
    }

    return 0;
}
