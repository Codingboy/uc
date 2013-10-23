#ifndef EZ3_H
#define EZ3_H

#include "gpio.h"

typedef struct
{
	Gpio* rx;
	Gpio* tx;
} Ez3;

/**
 * Initialises an EZ3 sensor.
 * When this function is called, the range between EZ3 sensor and objects in front of it shall be minimum 17.78 cm, optimum 35.56 cm or greater.
 * @post \p ez3 is initialised
 */
void initEz3(Ez3* ez3, Gpio* rx, u8 portRx, u8 bitRx, Gpio* tx, u8 portTx, u8 bitTx);

/**
 * Measures the range between EZ3 sensor and an object.
 * EZ3 sensor detects objects in between range from 15.24 up to 645 cm.
 * Objects nearer than 15.24 cm will be detected as if they were in a range of 15.24 cm.
 * @pre \p ez3 is initialised
 * @pre time is initialised
 * @return distance to object in cm or 0 if no object was detected
 */
u16 measureEz3(Ez3* ez3);

#endif
