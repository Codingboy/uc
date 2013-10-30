#include <stdbool.h>
#include "debug.h"
#include "gpio.h"
#include "led.h"
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <avr/wdt.h>
#include <avr/power.h>

#include <USB.h>
#include <Endpoint_AVR8.h>
#include "descriptors.h"

Led led0;
Gpio gpioLed0;
Led led1;
Gpio gpioLed1;
Led led2;
Gpio gpioLed2;
Led led3;
Gpio gpioLed3;

void usbOnLed(void)
{
	if (USB_ControlRequest.wValue >= 4)
	{
		Endpoint_StallTransaction();
		return;
	}
	Endpoint_ClearSETUP();//ack setup packet
	switch (USB_ControlRequest.wValue)
	{
		case 0:
			onLed(&led0);
			break;
		case 1:
			onLed(&led1);
			break;
		case 2:
			onLed(&led2);
			break;
		case 3:
			onLed(&led3);
			break;
	}
	Endpoint_ClearStatusStage();//ack control request
}

void usbOffLed(void)
{
	if (USB_ControlRequest.wValue >= 4)
	{
		Endpoint_StallTransaction();
		return;
	}
	Endpoint_ClearSETUP();//ack setup packet
	switch (USB_ControlRequest.wValue)
	{
		case 0:
			offLed(&led0);
			break;
		case 1:
			offLed(&led1);
			break;
		case 2:
			offLed(&led2);
			break;
		case 3:
			offLed(&led3);
			break;
	}
	Endpoint_ClearStatusStage();//ack control request
}

void usbToggleLed(void)
{
	if (USB_ControlRequest.wValue >= 4)
	{
		Endpoint_StallTransaction();
		return;
	}
	Endpoint_ClearSETUP();//ack setup packet
	switch (USB_ControlRequest.wValue)
	{
		case 0:
			toggleLed(&led0);
			break;
		case 1:
			toggleLed(&led1);
			break;
		case 2:
			toggleLed(&led2);
			break;
		case 3:
			toggleLed(&led3);
			break;
	}
	Endpoint_ClearStatusStage();//ack control request
}

void usbCheckLed(void)
{
	if (USB_ControlRequest.wValue >= 4)
	{
		Endpoint_StallTransaction();
		return;
	}
	Endpoint_ClearSETUP();//ack setup packet
	u8 state;
	switch (USB_ControlRequest.wValue)
	{
		case 0:
			state = checkLed(&led0);
			break;
		case 1:
			state = checkLed(&led1);
			break;
		case 2:
			state = checkLed(&led2);
			break;
		case 3:
			state = checkLed(&led3);
			break;
	}
	while (!Endpoint_IsINReady())
	{
		//wait until host is ready
	}
	Endpoint_Write_8(state);
	Endpoint_ClearIN();
#if 0
	while (!Endpoint_IsOUTReceived())
	{
		//wait for host to send status
	}
	Endpoint_ClearOUT();//send message
	//rumgefrickel, works without this function dont know why
	//Endpoint_ClearStatusStage();//success :D
#endif
}

void usbReadEz3(void)
{
	Endpoint_ClearSETUP();//ack setup packet
	Endpoint_ClearStatusStage();//ack control request
}

/*
off led
	bmRequestType = REQTYPE_VENDOR | REQREC_DEVICE | REQDIR_HOSTTODEVICE
	bRequest = 0
	wValue = [0;3]
	wLength = 0
on led
	bmRequestType = REQTYPE_VENDOR | REQREC_DEVICE | REQDIR_HOSTTODEVICE
	bRequest = 1
	wValue = [0;3]
	wLength = 0
toggle led
	bmRequestType = REQTYPE_VENDOR | REQREC_DEVICE | REQDIR_HOSTTODEVICE
	bRequest = 2
	wValue = [0;3]
	wLength = 0
check led
	bmRequestType = REQTYPE_VENDOR | REQREC_DEVICE | REQDIR_DEVICETOHOST
	bRequest = 3
	wValue = [0;3]
	wLength = 1
read ez3
	bmRequestType = REQTYPE_VENDOR | REQREC_DEVICE | REQDIR_DEVICETOHOST
	bRequest = 4
	dataOUT = 
*/
void EVENT_USB_Device_ControlRequest(void)
{
	if (((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_TYPE) == REQTYPE_VENDOR)//type == vendor
		&& ((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_RECIPIENT) == REQREC_DEVICE))//recipient == device
	{
		if ((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_DIRECTION) == REQDIR_HOSTTODEVICE)//host sends
		{
			switch (USB_ControlRequest.bRequest)
			{
				case 0:
					usbOffLed();
					break;
				case 1:
					usbOnLed();
					break;
				case 2:
					usbToggleLed();
					break;
				case 5:
					break;
				default:
					break;
			}
		}
		else//device sends
		{
			switch (USB_ControlRequest.bRequest)
			{
				case 3:
					usbCheckLed();
					break;
				case 4:
					usbReadEz3();
					break;
				default:
					break;
			}
		}
	}
}

void EVENT_USB_Device_ConfigurationChanged(void)
{
	//controlendpoint is configured internally by lufa with default settings
	//Endpoint_ConfigureEndpoint(ENDPOINT_CONTROLEP, EP_TYPE_CONTROL, ENDPOINT_DIR_IN, ENDPOINT_CONTROLEP_DEFAULT_SIZE, ENDPOINT_BANK_SINGLE);
	///\todo do i need those endpoints?
	Endpoint_ConfigureEndpoint(IN_EPNUM, EP_TYPE_BULK, IO_EPSIZE, 1);
	Endpoint_ConfigureEndpoint(OUT_EPNUM, EP_TYPE_BULK, IO_EPSIZE, 1);
}

ISR(TIMER0_OVF_vect)//each 10 ms
{
	handleDebug();
	TCNT0 = 256-157;//each 10 ms
	USB_USBTask();
#if 0
	if (USB_DeviceState == DEVICE_STATE_Configured)
	{
		Endpoint_SelectEndpoint(OUT_EPNUM);

		/* Check to see if a packet has been sent from the host */
		if (Endpoint_IsOUTReceived())
		{
			/* Check to see if the packet contains data */
			if (Endpoint_IsReadWriteAllowed())
			{
				/* Create a temporary buffer to hold the read in report from the host */
				//uint8_t GenericData[GENERIC_REPORT_SIZE];

				/* Read Generic Report Data */
				Endpoint_Read_8();

				/* Process Generic Report Data */
				//ProcessGenericHIDReport(GenericData);
			}

			/* Finalize the stream transfer to send the last packet */
			Endpoint_ClearOUT();
		}

		Endpoint_SelectEndpoint(IN_EPNUM);

		/* Check to see if the host is ready to accept another packet */
		if (Endpoint_IsINReady())
		{
			/* Create a temporary buffer to hold the report to send to the host */
			//uint8_t GenericData[GENERIC_REPORT_SIZE];

			/* Create Generic Report Data */
			//CreateGenericHIDReport(GenericData);

			/* Write Generic Report Data */
			Endpoint_Write_8(42);

			/* Finalize the stream transfer to send the last packet */
			Endpoint_ClearIN();
		}
	}
#endif
}

int main(void)
{
	initDebug();
	setDebug(2);
	initLed(&led0, &gpioLed0, 1, 0, 1);
	initLed(&led1, &gpioLed1, 1, 1, 1);
	initLed(&led2, &gpioLed2, 1, 2, 1);
	initLed(&led3, &gpioLed3, 1, 3, 1);
	MCUSR &= ~(1<<WDRF);
	wdt_disable();
	//clock_prescale_set(clock_div_1);
	USB_Init();
	sei();
	while (USB_DeviceState != DEVICE_STATE_Configured)
	{
	}
	setDebug(1);
	while (true)
	{
	}
}
