#include <stdbool.h>

#ifdef DEBUG
#include "debug.h"
#include "gpio.h"
#include "led.h"
#include <avr/interrupt.h>
#include <avr/delay.h>

ISR(TIMER0_OVF_vect)//each 10 ms
{
	handleDebug();
	TCNT0 = 256-157;//each 10 ms
}
#endif

int main()
{
#ifdef DEBUG
	initDebug();
	setDebug(3);
#endif
	while (true)
	{
	}
}
