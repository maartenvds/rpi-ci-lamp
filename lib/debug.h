/*
 *  Debug utility header
 *  Author: Maarten Vandersteege
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifdef DEBUG
#define debug printf
#else
#define debug(...)
#endif

#endif /* DEBUG_H */
