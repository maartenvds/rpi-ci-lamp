/*
 *  Red/green lamp IO implementation
 *  Author: Maarten Vandersteegen
 */

#include "logging.h"
#include "lamp_io_red_green.h"
#include <wiringPi.h>
#include <stdio.h>

#ifndef LAMP_IO_RED_PIN
#define LAMP_IO_RED_PIN     (int)23
#endif

#ifndef LAMP_IO_GREEN_PIN
#define LAMP_IO_GREEN_PIN   (int)21
#endif

#ifdef  TESTING
#define DIGITAL_WRITE(...)
#define PIN_MODE(...)
#define PIN_SETUP(...)      (int)0
#else
#define DIGITAL_WRITE       digitalWrite
#define PIN_MODE            pinMode
#define PIN_SETUP           wiringPiSetup
#endif

/*
 *  Thread for supporting a blinking lamp
 */
static void *lamp_io_blink_thread(void *obj)
{
    struct LampIoRedGreen *self = obj;

    while (self->thread_is_running) {
        if (self->blinking_lamp >= 0) {
            DIGITAL_WRITE(self->blinking_lamp, HIGH);
        }
        delay(500);
        if (self->blinking_lamp >= 0) {
            DIGITAL_WRITE(self->blinking_lamp, LOW);
        }
        delay(500);
    }

    return NULL;
}

int lamp_io_red_green_init(struct LampIoRedGreen *self)
{
    if (PIN_SETUP() != 0) {
        error("Failed to setup GPIO\n");
        return -1;
    }

    PIN_MODE(LAMP_IO_RED_PIN, OUTPUT);
    PIN_MODE(LAMP_IO_GREEN_PIN, OUTPUT);

    self->thread_is_running = 1;
    if (pthread_create(&self->thread_handle, NULL, lamp_io_blink_thread, &self->blinking_lamp)) {
        error("Error creating lamp thread\n");
        return -1;
    }

    return 0;
}

void lamp_io_red_green_deinit(struct LampIoRedGreen *self)
{
    self->thread_is_running = 0;
    (void) pthread_join(self->thread_handle, NULL);
}

void lamp_io_red_green_set_state(struct LampIoRedGreen *self, enum LampStateRedGreen lamp_state)
{
    self->blinking_lamp = -1;

    switch (lamp_state) {
        case LAMP_STATE_OFF :
            DIGITAL_WRITE(LAMP_IO_RED_PIN, LOW);
            DIGITAL_WRITE(LAMP_IO_GREEN_PIN, LOW);
            debug("lamp = off\n");
            break;

        case LAMP_STATE_GREEN :
            DIGITAL_WRITE(LAMP_IO_RED_PIN, LOW);
            DIGITAL_WRITE(LAMP_IO_GREEN_PIN, HIGH);
            debug("lamp = green\n");
            break;
        case LAMP_STATE_RED :
            DIGITAL_WRITE(LAMP_IO_RED_PIN, HIGH);
            DIGITAL_WRITE(LAMP_IO_GREEN_PIN, LOW);
            debug("lamp = red\n");
            break;
        case LAMP_STATE_BLINK_GREEN :
            DIGITAL_WRITE(LAMP_IO_RED_PIN, LOW);
            self->blinking_lamp = LAMP_IO_GREEN_PIN;
            debug("lamp = blink green\n");
            break;
        case LAMP_STATE_BLINK_RED :
            DIGITAL_WRITE(LAMP_IO_GREEN_PIN, LOW);
            self->blinking_lamp = LAMP_IO_RED_PIN;
            debug("lamp = blink red\n");
            break;
        case LAMP_STATE_ERROR :
        default:
            DIGITAL_WRITE(LAMP_IO_RED_PIN, HIGH);
            DIGITAL_WRITE(LAMP_IO_GREEN_PIN, HIGH);
            debug("lamp = error\n");
    };
}
