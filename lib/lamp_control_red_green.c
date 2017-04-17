/*
 *  Red/green lamp control implementation
 *  Translates a build state into a red/green signal
 *  Author: Maarten Vandersteegen
 */

#include "lamp_control_red_green.h"
#include <stdlib.h>

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

void *lamp_control_init(void)
{
    struct LampControlRedGreen *self = malloc(sizeof(struct LampControlRedGreen));
    if (!self)
        return NULL;

    if (lamp_io_red_green_init(&self->lamp_io) == -1)
       return NULL;

    lamp_control_off(self);
    return self;
}

void lamp_control_deinit(void *vself)
{
    struct LampControlRedGreen *self = vself;

    if (self) {
        lamp_io_red_green_deinit(&self->lamp_io);
        free(self);
    }
}

void lamp_control_set_state(void *vself, enum BuildState build_state)
{
    struct LampControlRedGreen *self = vself;

    build_state_to_lamp_state(&self->lamp_state, build_state);
    lamp_io_red_green_set_state(&self->lamp_io, self->lamp_state);
}

void lamp_control_signal_error(void *vself)
{
    struct LampControlRedGreen *self = vself;

    self->lamp_state = LAMP_STATE_ERROR;
    lamp_io_red_green_set_state(&self->lamp_io, self->lamp_state);
}

void lamp_control_off(void *vself)
{
    struct LampControlRedGreen *self = vself;

    self->lamp_state = LAMP_STATE_OFF;
    lamp_io_red_green_set_state(&self->lamp_io, self->lamp_state);
}
