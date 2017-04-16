/*
 *  Lamp control module header
 *  Author: Maarten Vandersteegen
 */

#ifndef LAMP_CONTROL_H
#define LAMP_CONTROL_H

#include "build_state.h"
#include "lamp_io_red_green.h"

struct LampControl
{
    enum LampStateRedGreen lamp_state;
    struct LampIoRedGreen lamp_io;
};

/*
 *  self:  lamp control instance
 */
int lamp_control_init(struct LampControl *self);

/*
 *  self:  lamp control instance
 */
void lamp_control_deinit(struct LampControl *self);

/*
 *  Set the state of the lamp
 *  self:  lamp control instance
 *  state:  aggregate build state to be displayed on the lamp
 */
void lamp_control_set_state(struct LampControl *self, enum BuildState build_state);

/*
 *  Signal program error on the lamp
 *  self:  lamp control instance
 */
void lamp_control_signal_error(struct LampControl *self);

/*
 *  Turn of the lamp
 *  self:  lamp control instance
 */
void lamp_control_off(struct LampControl *self);

#endif /* LAMP_CONTROL_H */
