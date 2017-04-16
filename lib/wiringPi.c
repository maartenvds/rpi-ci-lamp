#include "wiringPi.h"

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
    (void)howLong;
}
