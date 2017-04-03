/*
 *  Lamp control module header
 *  Author: Maarten Vandersteegen
 */

#ifndef LAMP_CONTROL_H
#define LAMP_CONTROL_H

#include "build_state.h"

/*
 *  lamp_state: state variable for this lamp
 */
void lamp_control_init(int *lamp_state);

/*
 *  Set the state of the lamp
 *  state:  aggregate build state to be displayed on the lamp
 */
void lamp_control_set_state(int *lamp_state, enum BuildState build_state);

/*
 *  Signal program error on the lamp
 */
void lamp_control_signal_error(int *lamp_state);

/*
 *  Turn of the lamp
 */
void lamp_control_off(int *lamp_state);

#endif /* LAMP_CONTROL_H */
