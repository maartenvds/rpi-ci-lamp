/*
 *  Lamp IO control
 *  Author: Maarten Vandersteege
 */

#ifndef LAMP_IO_H
#define LAMP_IO_H

#include "build_state.h"

/*
 *  Set the state of the lamp
 *  state:  aggregate build state to be displayed on the lamp
 */
void lamp_io_set_state(int *lamp_state, enum BuildState build_state);

/*
 *  Signal program error on the lamp
 */
void lamp_io_error(int *lamp_state);

/*
 *  Turn of the lamp
 */
void lamp_io_off(int *lamp_state);

#endif /* LAMP_IO_H */
