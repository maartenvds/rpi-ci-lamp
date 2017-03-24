/*
 *  Lamp IO control
 *  Author: Maarten Vandersteege
 */

#ifndef LAMP_IO_H
#define LAMP_IO_H

enum LampIoState
{
    LAMP_OFF = 0,
    LAMP_GREEN,
    LAMP_RED,
    LAMP_ERROR
};

void lamp_io_set_state(enum LampIoState state);

#endif /* LAMP_IO_H */
