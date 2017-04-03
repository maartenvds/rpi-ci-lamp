/*
 *  Lamp IO red/green header
 *  Author: Maarten Vandersteegen
 */

#ifndef LAMP_IO_RED_GREEN_H
#define LAMP_IO_RED_GREEN_H

#include "lamp_io.h"

enum LampStateRedGreen
{
    LAMP_STATE_OFF = 0,
    LAMP_STATE_GREEN,
    LAMP_STATE_RED,
    LAMP_STATE_BLINK_GREEN,
    LAMP_STATE_BLINK_RED,
    LAMP_STATE_ERROR
};

#endif /* LAMP_IO_RED_GREEN_H */
