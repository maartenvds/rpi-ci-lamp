/*
 *  Red/green lamp torch implementation
 *  Author: Maarten Vandersteegen
 */

#include "lamp_io.h"
#include <stdio.h>

enum LampState
{
    LAMP_STATE_OFF = 0,
    LAMP_STATE_GREEN,
    LAMP_STATE_RED,
    LAMP_STATE_BLINK_GREEN,
    LAMP_STATE_BLINK_RED,
    LAMP_STATE_ERROR
};

static void lamp_io_set(enum LampState lamp_state)
{
    switch(lamp_state) {
        case LAMP_STATE_OFF :
            printf("lamp = off\n");
            break;
        case LAMP_STATE_GREEN :
            printf("lamp = green\n");
            break;
        case LAMP_STATE_RED :
            printf("lamp = red\n");
            break;
        case LAMP_STATE_BLINK_GREEN :
            printf("lamp = blink green\n");
            break;
        case LAMP_STATE_BLINK_RED :
            printf("lamp = blink red\n");
            break;
        case LAMP_STATE_ERROR :
        default:
            printf("lamp = error\n");
    };
}

static void build_state_to_lamp_state(enum LampState *lamp_state, enum BuildState build_state)
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

void lamp_io_set_state(int *lamp_state, enum BuildState build_state)
{
    build_state_to_lamp_state((enum BuildState *)lamp_state, build_state);
    lamp_io_set(*lamp_state);
}

void lamp_io_error(int *lamp_state)
{
    *lamp_state = LAMP_STATE_ERROR;
    lamp_io_set(*lamp_state);
}

void lamp_io_off(int *lamp_state)
{
    *lamp_state = LAMP_STATE_OFF;
    lamp_io_set(*lamp_state);
}

