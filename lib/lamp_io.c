/*
 *  Lamp IO control
 *  Author: Maarten Vandersteege
 */

#include "lamp_io.h"
#include <stdio.h>

void lamp_io_set_state(enum LampIoState state)
{
    switch(state) {
        case LAMP_OFF :
            printf("lamp = off\n");
            break;
        case LAMP_GREEN :
            printf("lamp = green\n");
            break;
        case LAMP_RED :
            printf("lamp = red\n");
            break;
        case LAMP_ERROR :
        default:
            printf("lamp = error\n");
    };
}
