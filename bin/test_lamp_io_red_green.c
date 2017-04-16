/*
 *  Main program CI-lamp
 *  Author: Maarten Vandersteege
 */

#include "logging.h"
#include "lamp_io_red_green.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    struct LampIoRedGreen lamp_control;

    (void) lamp_io_red_green_init(&lamp_control);
    lamp_io_red_green_set_state(&lamp_control, LAMP_STATE_GREEN);
    sleep(1);
    lamp_io_red_green_set_state(&lamp_control, LAMP_STATE_RED);
    sleep(1);
    lamp_io_red_green_set_state(&lamp_control, LAMP_STATE_BLINK_GREEN);
    sleep(5);
    lamp_io_red_green_set_state(&lamp_control, LAMP_STATE_BLINK_RED);
    sleep(5);
    lamp_io_red_green_set_state(&lamp_control, LAMP_STATE_ERROR);
    sleep(1);
    lamp_io_red_green_set_state(&lamp_control, LAMP_STATE_OFF);
    lamp_io_red_green_deinit(&lamp_control);

    return 0;
}
