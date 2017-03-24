/*
 *  Debug utility header
 *  Author: Maarten Vandersteege
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#define error(fmt, ...)     fprintf(stderr, fmt, ##__VA_ARGS__)

#ifdef DEBUG
#define debug printf
#else
#define debug(...)
#endif

#endif /* DEBUG_H */
