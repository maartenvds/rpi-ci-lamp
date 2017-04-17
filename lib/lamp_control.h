/*
 *  Lamp control module header
 *  Author: Maarten Vandersteegen
 */

#ifndef LAMP_CONTROL_H
#define LAMP_CONTROL_H

#include "build_state.h"

/*
 *  Returns a lamp control handle on success, returns NULL on failure
 */
void *lamp_control_init(void);

/*
 *  Clean up the lamp control instance
 *  self:  lamp control handle
 */
void lamp_control_deinit(void *self);

/*
 *  Set the state of the lamp
 *  self:   lamp control instance
 *  state:  aggregate build state to be displayed on the lamp
 */
void lamp_control_set_state(void *self, enum BuildState build_state);

/*
 *  Signal program error on the lamp
 *  self:  lamp control instance
 */
void lamp_control_signal_error(void *self);

/*
 *  Turn of the lamp
 *  self:  lamp control instance
 */
void lamp_control_off(void *self);

#endif /* LAMP_CONTROL_H */
