/*
 *  Lamp IO module header
 *  Author: Maarten Vandersteegen
 */

#ifndef LAMP_IO_READ_GREEN_H
#define LAMP_IO_READ_GREEN_H

#include <pthread.h>

/* I/O pin definitions */

#ifndef LAMP_IO_RED_PIN
#define LAMP_IO_RED_PIN     (int)23
#endif

#ifndef LAMP_IO_GREEN_PIN
#define LAMP_IO_GREEN_PIN   (int)21
#endif

enum LampStateRedGreen
{
    LAMP_STATE_OFF = 0,
    LAMP_STATE_GREEN,
    LAMP_STATE_RED,
    LAMP_STATE_BLINK_GREEN,
    LAMP_STATE_BLINK_RED,
    LAMP_STATE_ERROR
};

struct LampIoRedGreen
{
    int blinking_lamp;
    int thread_is_running;
    pthread_t thread_handle;
};

/*
 *  Initialize lamp IO
 *  self:   lamp state instance
 *  return: 0 on success, -1 on failure
 */
int lamp_io_red_green_init(struct LampIoRedGreen *self);

/*
 *  Deinit lamp IO
 *  self:   lamp state instance
 */
void lamp_io_red_green_deinit(struct LampIoRedGreen *self);

/*
 *  Set the state of the lamp
 *  self:       lamp state instance
 *  lamp_state: state of the lamp
 */
void lamp_io_red_green_set_state(struct LampIoRedGreen *self, enum LampStateRedGreen lamp_state);

#endif /* LAMP_IO_RED_GREEN_H */
