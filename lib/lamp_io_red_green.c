/*
 *  Red/green lamp IO implementation
 *  Author: Maarten Vandersteegen
 */

#include "lamp_io_red_green.h"
#include <stdio.h>

void lamp_io_init()
{
}

void lamp_io_set_state(int lamp_state)
{
    switch((enum LampStateRedGreen)lamp_state) {
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
