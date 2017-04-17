#include "wiringPi.h"
#include <time.h>

void digitalWrite(int pin, int value)
{
    (void)pin;
    (void)value;
}

void pinMode(int pin, int mode)
{
    (void)pin;
    (void)mode;
}

int wiringPiSetup(void)
{
    return 0;
}

void delay(unsigned int howLong)
{
    struct timespec sleeper, dummy;

    sleeper.tv_sec = (time_t)(howLong / 1000);
    sleeper.tv_nsec = (long)(howLong % 1000) * 1000000;

    nanosleep (&sleeper, &dummy);
}
