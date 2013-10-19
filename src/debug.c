#ifdef DEBUG

#include "led.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "debug.h"

u8 DEBUGMODE;
u8 DEBUGSTATE;
Led* DEBUGLED;
Gpio* DEBUGGPIO;

void initDebug()
{
	DEBUGMODE = 0;
	DEBUGSTATE = 0;

	initLed(DEBUGLED, DEBUGGPIO, 4, 6, 1);
	offLed(DEBUGLED);

	//normal mode
	TCCR0B &= ~(1<<WGM02);
	TCCR0A &= ~(1<<WGM01);
	TCCR0A &= ~(1<<WGM00);

	//normal port operation for OC0A
	TCCR0A &= ~(1<<COM0A1);
	TCCR0A &= ~(1<<COM0A0);

	//normal port operation for OC0B
	TCCR0A &= ~(1<<COM0B1);
	TCCR0A &= ~(1<<COM0B0);

	//set prescaler to 1024
	TCCR0B |= 1<<CS02;
	TCCR0B &= ~(1<<CS01);
	TCCR0B |= 1<<CS00;

	TCNT0 = 256-157;//each 10 ms

	//enable overflow interrupts
	TIMSK0 |= 1<<TOIE0;

	sei();
}

void setDebug(u8 mode)
{
	DEBUGMODE = mode;
	DEBUGSTATE = 0;
}

void handleDebug()
{
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
		case 9:
			switch (DEBUGSTATE)
			{
				case 10:
					onLed(DEBUGLED);
					break;
				case 20:
					offLed(DEBUGLED);
					DEBUGSTATE = 0;
					break;
			}
			break;
		case 10:
			switch (DEBUGSTATE)
			{
				case 5:
					onLed(DEBUGLED);
					break;
				case 10:
					offLed(DEBUGLED);
					DEBUGSTATE = 0;
					break;
			}
			break;
	}
}

#endif
