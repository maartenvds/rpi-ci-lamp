/*
 *  Lamp IO control
 *  Author: Maarten Vandersteege
 */

#ifndef LAMP_IO_H
#define LAMP_IO_H

enum LampIoState
{
    LAMP_GREEN = 0,
    LAMP_RED
};

void lamp_io_set_state(enum LampIoState state);

#endif /* LAMP_IO_H */
