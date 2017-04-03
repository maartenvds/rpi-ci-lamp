/*
 *  Red/green lamp control implementation
 *  Translates a build state into a red/green signal
 *  Author: Maarten Vandersteegen
 */

#include "lamp_control.h"
#include "lamp_io_red_green.h"

static void build_state_to_lamp_state(enum LampStateRedGreen *lamp_state, enum BuildState build_state)
{
    if (build_state == BUILD_STATE_PASSED)
        *lamp_state = LAMP_STATE_GREEN;
    else if (build_state == BUILD_STATE_FAILED)
        *lamp_state = LAMP_STATE_RED;

    /* build_state is 'BUILD_STATE_RUNNING' */
    else if (*lamp_state == LAMP_STATE_OFF || *lamp_state == LAMP_STATE_GREEN)
        *lamp_state = LAMP_STATE_BLINK_GREEN;
    else if (*lamp_state == LAMP_STATE_RED || *lamp_state == LAMP_STATE_ERROR)
        *lamp_state = LAMP_STATE_BLINK_RED;
}

void lamp_control_init(int *lamp_state)
{
    lamp_io_init();
    lamp_control_off(lamp_state);
}

void lamp_control_set_state(int *lamp_state, enum BuildState build_state)
{
    build_state_to_lamp_state((enum LampStateRedGreen *)lamp_state, build_state);
    lamp_io_set_state(*lamp_state);
}

void lamp_control_signal_error(int *lamp_state)
{
    *lamp_state = LAMP_STATE_ERROR;
    lamp_io_set_state(*lamp_state);
}

void lamp_control_off(int *lamp_state)
{
    *lamp_state = LAMP_STATE_OFF;
    lamp_io_set_state(*lamp_state);
}
