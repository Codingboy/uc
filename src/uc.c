#include <stdbool.h>

#ifdef DEBUG
#include "debug.h"
#include "gpio.h"
#include "led.h"
#include <avr/interrupt.h>
#include <avr/delay.h>

u8 DEBUGMODE;
u8 DEBUGSTATE;
Led* DEBUGLED;
Gpio* DEBUGGPIO;
	Led led1;
	Gpio gpio1;

ISR(TIMER0_OVF_vect)//each 10 ms
{
//toggleLed(&led1);
_delay_ms(100);
	DEBUGSTATE++;
	switch (DEBUGMODE)
	{
		case 0:
			switch (DEBUGSTATE)
			{
				case 190:
					onLed(DEBUGLED);
					break;
				case 200:
					offLed(DEBUGLED);
					DEBUGSTATE = 0;
					break;
			}
			break;
		case 1:
			switch (DEBUGSTATE)
			{
				case 90:
					onLed(DEBUGLED);
					break;
				case 100:
					offLed(DEBUGLED);
					DEBUGSTATE = 0;
					break;
			}
			break;
		case 2:
			switch (DEBUGSTATE)
			{
				case 40:
					onLed(DEBUGLED);
					break;
				case 50:
					offLed(DEBUGLED);
					DEBUGSTATE = 0;
					break;
			}
			break;
		case 3:
			switch (DEBUGSTATE)
			{
				case 190:
					offLed(DEBUGLED);
					break;
				case 200:
					onLed(DEBUGLED);
					DEBUGSTATE = 0;
					break;
			}
			break;
		case 4:
			switch (DEBUGSTATE)
			{
				case 90:
					offLed(DEBUGLED);
					break;
				case 100:
					onLed(DEBUGLED);
					DEBUGSTATE = 0;
					break;
			}
			break;
		case 5:
			switch (DEBUGSTATE)
			{
				case 40:
					offLed(DEBUGLED);
					break;
				case 50:
					onLed(DEBUGLED);
					DEBUGSTATE = 0;
					break;
			}
			break;
		case 6:
			switch (DEBUGSTATE)
			{
				case 100:
					onLed(DEBUGLED);
					break;
				case 200:
					offLed(DEBUGLED);
					DEBUGSTATE = 0;
					break;
			}
			break;
		case 7:
			switch (DEBUGSTATE)
			{
				case 50:
					onLed(DEBUGLED);
					break;
				case 100:
					offLed(DEBUGLED);
					DEBUGSTATE = 0;
					break;
			}
			break;
		case 8:
			switch (DEBUGSTATE)
			{
				case 25:
					onLed(DEBUGLED);
					break;
				case 50:
					offLed(DEBUGLED);
					DEBUGSTATE = 0;
					break;
			}
			break;
		default:
			break;
	}
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
