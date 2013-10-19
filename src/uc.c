#include <stdbool.h>

#ifdef DEBUG
#include "debug.h"
#include "gpio.h"
#include "led.h"
#include <avr/interrupt.h>
#include <avr/delay.h>

	Led led1;
	Gpio gpio1;

ISR(TIMER0_OVF_vect)//each 10 ms
{
onLed(&led1);
_delay_ms(100);
offLed(&led1);
_delay_ms(100);
	handleDebug();
	TCNT0 = 256-157;//each 10 ms
}
#endif

int main()
{
	initLed(&led1, &gpio1, 1,0,1);
#ifdef DEBUG
	initDebug();
#endif
	while (true)
	{
	}
}
