/*
 *  Red/green lamp control header
 *  Author: Maarten Vandersteegen
 */

#ifndef LAMP_CONTROL_RED_GREEN_H
#define LAMP_CONTROL_RED_GREEN_H

#include "lamp_control.h"
#include "lamp_io_red_green.h"

struct LampControlRedGreen
{
    enum LampStateRedGreen lamp_state;
    struct LampIoRedGreen lamp_io;
};

#endif /* LAMP_CONTROL_RED_GREEN_H */
