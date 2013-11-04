#ifndef DEBUG_H
#define DEBUG_H

#include "typedefs.h"

/**
 * Initialises the debug LED.
 */
void initDebug(void);

/**
 * Sets a debugmode.
 * Depending on the mode, the debug LED will blink other.
state off on
0 1900 100
1 900 100
2 400 100
3 100 1900
4 100 900
5 100 400
6 1000 1000
7 500 500
8 250 250
9 100 100
10 50 50
 */
void setDebug(u8 debugLevel);

/**
 * This function handles the debugled and must be called each 10 ms.
 */
void handleDebug(void);

#endif
