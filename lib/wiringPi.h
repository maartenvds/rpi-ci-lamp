#ifndef WIRINGPI_H
#define WIRINGPI_H

#define	LOW     0
#define	HIGH    1
#define	OUTPUT  1

extern void digitalWrite(int pin, int value);
extern void pinMode(int pin, int mode);
extern int wiringPiSetup(void);
extern void delay(unsigned int howLong);

#endif
