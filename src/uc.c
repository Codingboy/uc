#include <stdbool.h>
#include "debug.h"
#include "gpio.h"
#include "led.h"
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <avr/wdt.h>
#include <avr/power.h>

#ifdef USB
#include <USB.h>
#include <Endpoint_AVR8.h>
#include "descriptors.h"
Led led1;
Gpio gpioLed1;
void usbSetLed(void)
{
	Endpoint_ClearSETUP();//ack setup packet
	u8 recvData = 0;
	while (recvData < 0)//never
	{
		while (!Endpoint_IsOUTReceived())
		{
			//wait for data
		}
		Endpoint_ClearOUT();//ack data packet
	}
	onLed(&led1);
	while (!Endpoint_IsINReady())
	{
		//wait until host ready to recv
	}
	Endpoint_ClearIN();//ack
}
#if 0
void usbGetLed(void)
{
	Endpoint_ClearSETUP();//ack setup packet
	u8 sendData = 0;
	while (sendData < 1)
	{
		while (!Endpoint_IsINReady())
		{
			//wait until host is ready
		}
		u8 state = stateLed(led2);
		Endpoint_Write_8(state);
		sendData++;
		Endpoint_ClearIN();
	}
	while (!Endpoint_IsOUTReceived())
	{
		//wait for host to send status
	}
	Endpoint_ClearOUT();//send message
	//rumgefrickel, works without this function dont know why
	//Endpoint_ClearStatusStage();//success :D
}
#endif
void usbClearLed(void)
{
	Endpoint_ClearSETUP();//ack setup packet
	u8 recvData = 0;
	while (recvData < 0)//never
	{
		while (!Endpoint_IsOUTReceived())
		{
			//wait for data
		}
		Endpoint_ClearOUT();//ack data packet
	}
	offLed(&led1);
	while (!Endpoint_IsINReady())
	{
		//wait until host ready to recv
	}
	Endpoint_ClearIN();//ack
}

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
					usbClearLed();
					break;
				case 1:
					usbSetLed();
					break;
				case 2:
					//usbToggleLed();
					break;
				default:
					break;
			}
		}
		else//device sends
		{
			switch (USB_ControlRequest.bRequest)
			{
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
#endif

ISR(TIMER0_OVF_vect)//each 10 ms
{
	handleDebug();
	TCNT0 = 256-157;//each 10 ms
#ifdef USB
	USB_USBTask();
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
	initLed(&led1, &gpioLed1, 0, 0, 1);
	sei();
	setDebug(2);
#ifdef USB
	MCUSR &= ~(1<<WDRF);
	wdt_disable();
	//clock_prescale_set(clock_div_1);
	USB_Init();
	while (USB_DeviceState != DEVICE_STATE_Configured)
	{
	}
	setDebug(1);
#endif
	while (true)
	{
	}
}
