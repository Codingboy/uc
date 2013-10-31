#include <avr/io.h>
#include "time.h"
#include <avr/interrupt.h>

static u32 TIME;//time in µs

void incrementTime(void)
{
	TIME += 100;
}

void initTime(void)
{
	TIME = 0;

	TCCR1A &= ~(1<<COM1A1);
	TCCR1A &= ~(1<<COM1A0);
	TCCR1A &= ~(1<<COM1B1);
	TCCR1A &= ~(1<<COM1B0);
	TCCR1A &= ~(1<<COM1C1);
	TCCR1A &= ~(1<<COM1C0);

	TCCR1A &= ~(1<<WGM11);
	TCCR1A &= ~(1<<WGM10);
	TCCR1B &= ~(1<<WGM13);
	TCCR1B &= ~(1<<WGM12);

	TCCR1B &= ~(1<<ICNC1);

	TCCR1B &= ~(1<<ICES1);

	//set prescaler to 64
	TCCR1B &= ~(1<<CS12);
	TCCR1B |= 1<<CS11;
	TCCR1B |= 1<<CS10;

	TCCR1C &= ~(1<<FOC1A);
	TCCR1C &= ~(1<<FOC1B);
	TCCR1C &= ~(1<<FOC1C);

	//each 100µs
	TCNT1H = 0xff;
	TCNT1L = 0xff - 25;
}

u32 getTime(void)
{
	u8 sreg = SREG;
	cli();
	u32 time = TIME;
	SREG = sreg;
	return time;
}

inline u32 convertTimeMS(u32 time)
{
	return time/1000;
}

ISR(TIMER1_OVF_vect)//each 100µs
{
	incrementTime();
	TCNT1H = 0xff;
	TCNT1L = 0xff - 25;
}
