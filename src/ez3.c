#include "ez3.h"
#include <avr/delay.h>
#include "time.h"

void initEz3(Ez3* ez3, Gpio* rx, u8 portRx, u8 bitRx, Gpio* tx, u8 portTx, u8 bitTx, Gpio* pw, u8 portPw, u8 bitPw)
{
	initGpio(rx, portRx, bitRx);
	initGpio(tx, portTx, bitTx);
	ctlGpioIn(pw);
	ctlGpioOut(rx);
	_delay_ms(250);
	setGpioOut(rx);
	_delay_us(20);
	clearGpioOut(rx);
	_delay_ms(200);
	ez3->rx = rx;
	ez3->tx = tx;
}

u16 measureEz3(Ez3* ez3)
{
	setGpioOut(ez3->rx);
	_delay_us(20);
	clearGpioOut(ez3->rx);
	while (!getGpioIn(ez3->pw))
	{
	}
	u32 timestamp = getTime();
	while (getGpioIn(ez3->pw) && getTime() < timestamp+37500)
	{
	}
	u32 time = getTime();
	if (time >= timestamp+37500)
	{
		return 0xffff;//no object in range
	}
	else
	{
		u16 ret = time/58;
		if (time%58 >= 58/2)
		{
			ret++;
		}
		return ret;
	}
}
