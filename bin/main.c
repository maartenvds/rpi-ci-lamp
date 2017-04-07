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
    application_deinit(&app);
    lamp_control_off(&app.lamp_control);
    exit(0);
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    int sleep_time;

    signal(SIGINT, int_handler);

    if (application_init(&app, "settings.json", "https://localhost:4443", 1) == -1) {
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
