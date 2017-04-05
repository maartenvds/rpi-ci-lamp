/*
 *  Red/green lamp IO implementation
 *  Author: Maarten Vandersteegen
 */

#include "logging.h"
#include "lamp_io_red_green.h"
#include <wiringPi.h>
#include <stdio.h>

#define LAMP_IO_RED_PIN     (int)23
#define LAMP_IO_GREEN_PIN   (int)21

/*
 *  Thread for supporting a blinking lamp
 */
static void *lamp_io_blink_thread(void *obj)
{
    struct LampIoRedGreen *self = obj;

    while (self->thread_is_running) {
        if (self->blinking_lamp >= 0)
            digitalWrite(self->blinking_lamp, HIGH);
        delay(500);
        if (self->blinking_lamp >= 0)
            digitalWrite(self->blinking_lamp, LOW);
        delay(500);
    }

    return NULL;
}

int lamp_io_red_green_init(struct LampIoRedGreen *self)
{
    if (wiringPiSetup() != 0) {
        error("Failed to setup GPIO\n");
        return -1;
    }

    pinMode(LAMP_IO_RED_PIN, OUTPUT);
    pinMode(LAMP_IO_GREEN_PIN, OUTPUT);

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
            digitalWrite(LAMP_IO_RED_PIN, LOW);
            digitalWrite(LAMP_IO_GREEN_PIN, LOW);
            debug("lamp = off\n");
            break;

        case LAMP_STATE_GREEN :
            digitalWrite(LAMP_IO_RED_PIN, LOW);
            digitalWrite(LAMP_IO_GREEN_PIN, HIGH);
            debug("lamp = green\n");
            break;
        case LAMP_STATE_RED :
            digitalWrite(LAMP_IO_RED_PIN, HIGH);
            digitalWrite(LAMP_IO_GREEN_PIN, LOW);
            printf("lamp = red\n");
            break;
        case LAMP_STATE_BLINK_GREEN :
            digitalWrite(LAMP_IO_RED_PIN, LOW);
            self->blinking_lamp = LAMP_IO_GREEN_PIN;
            printf("lamp = blink green\n");
            break;
        case LAMP_STATE_BLINK_RED :
            digitalWrite(LAMP_IO_GREEN_PIN, LOW);
            self->blinking_lamp = LAMP_IO_RED_PIN;
            printf("lamp = blink red\n");
            break;
        case LAMP_STATE_ERROR :
        default:
            digitalWrite(LAMP_IO_RED_PIN, HIGH);
            digitalWrite(LAMP_IO_GREEN_PIN, HIGH);
            printf("lamp = error\n");
    };
}
