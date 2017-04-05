/*
 *  Red/green lamp control implementation
 *  Translates a build state into a red/green signal
 *  Author: Maarten Vandersteegen
 */

#include "lamp_control.h"

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

int lamp_control_init(struct LampControl *self)
{
    if (lamp_io_red_green_init(&self->lamp_io) == -1)
       return -1;

    lamp_control_off(self);
    return 0;
}

void lamp_control_deinit(struct LampControl *self)
{
    lamp_io_red_green_deinit(&self->lamp_io);
}

void lamp_control_set_state(struct LampControl *self, enum BuildState build_state)
{
    build_state_to_lamp_state(&self->lamp_state, build_state);
    lamp_io_red_green_set_state(&self->lamp_io, self->lamp_state);
}

void lamp_control_signal_error(struct LampControl *self)
{
    self->lamp_state = LAMP_STATE_ERROR;
    lamp_io_red_green_set_state(&self->lamp_io, self->lamp_state);
}

void lamp_control_off(struct LampControl *self)
{
    self->lamp_state = LAMP_STATE_OFF;
    lamp_io_red_green_set_state(&self->lamp_io, self->lamp_state);
}
