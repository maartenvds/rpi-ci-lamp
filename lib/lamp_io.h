/*
 *  Lamp IO module header
 *  Author: Maarten Vandersteegen
 */

#ifndef LAMP_IO_H
#define LAMP_IO_H

/*
 *  Initialize lamp IO
 */
void lamp_io_init(void);

/*
 *  Set the state of the lamp
 *  The value of 'lamp_state' is lamp implementation specific
 */
void lamp_io_set_state(int lamp_state);

#endif /* LAMP_IO_H */
