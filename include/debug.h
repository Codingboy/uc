#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG

#include "typedefs.h"

/**
 * Initialises the debug LED.
 */
void initDebug(void);

/**
 * Sets a debugmode.
 * Depending on the mode, the debug LED will blink other.
 */
void setDebug(u8 debugLevel);

/**
 * This function handles the debugled and must be called each 10 ms.
 */
void handleDebug(void);

/**
 * Halts the system until BREAKPOINT is cleared by an ISR.
 */
void breakpointDebug(void);

#endif

#endif
