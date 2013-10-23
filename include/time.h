#ifndef TIME_H
#define TIME_H

#include "typedefs.h"

/**
 * Initialises the time.
 * @post time is initialised
 */
void initTime();

/**
 * Increments the time in 100 µs steps.
 * @pre time is initialised
 */
void incrementTime();

/**
 * Gets the time.
 * @return time in µs
 * @pre time is initialised
 * @info threadsafe
 */
u32 getTime();

/**
 * Converts the time from µs to ms.
 * @param[in] time time in µs
 * @return time in ms
 */
u32 convertTimeMS(u32 time);

#endif
