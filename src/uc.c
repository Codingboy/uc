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
void EVENT_USB_Device_ControlRequest(void)
{
	if (((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_TYPE) == REQTYPE_VENDOR)//type == vendor
		&& ((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_RECIPIENT) == REQREC_DEVICE))//recipient == device
	{
		if ((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_DIRECTION) == REQDIR_HOSTTODEVICE)//host sends
		{
			switch (USB_ControlRequest.bRequest)
			{
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

void EVENT_USB_Device_ConfigurationChanged()
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



int main()
{
	initDebug();
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
