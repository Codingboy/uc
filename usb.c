#include "globals.h"
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

void EVENT_USB_Device_Configuration_Changed()
{
	//controlendpoint is configured internally by lufa with default settings
	//Endpoint_ConfigureEndpoint(ENDPOINT_CONTROLEP, EP_TYPE_CONTROL, ENDPOINT_DIR_IN, ENDPOINT_CONTROLEP_DEFAULT_SIZE, ENDPOINT_BANK_SINGLE);
	///\todo do i need those endpoints?
	Endpoint_ConfigureEndpoint(IN_EPNUM, EP_TYPE_BULK, IO_EPSIZE, 1);
	Endpoint_ConfigureEndpoint(OUT_EPNUM, EP_TYPE_BULK, IO_EPSIZE, 1);
}

#if 0
void usbSetLed()
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
	onLed(led2);
	while (!Endpoint_IsINReady())
	{
		//wait until host ready to recv
	}
	Endpoint_ClearIN();//ack
}

void usbGetLed()
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

void usbClearLed()
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
	offLed(led2);
	while (!Endpoint_IsINReady())
	{
		//wait until host ready to recv
	}
	Endpoint_ClearIN();//ack
}

void usbSetServo()
{
	Endpoint_ClearSETUP();//ack setup packet
	u8 recvData = 0;
	u8 value = USB_ControlRequest.wValue;
	while (recvData < 0)//never
	{
		while (!Endpoint_IsOUTReceived())
		{
			//wait for data
		}
		//value = Endpoint_Read_8();//read value
		recvData++;
		Endpoint_ClearOUT();//ack data packet
	}
	setStateServo(servo, value);
	while (!Endpoint_IsINReady())
	{
		//wait until host ready to recv
	}
	Endpoint_ClearIN();//ack
}

void usbGetServo()
{
	Endpoint_ClearSETUP();//ack setup packet
	u8 sendData = 0;
	while (sendData < 1)
	{
		while (!Endpoint_IsINReady())
		{
			//wait until host is ready
		}
		u8 state = getStateServo(servo);
		Endpoint_Write_8(state);
		sendData++;
		Endpoint_ClearIN();
	}
	//while (!Endpoint_IsOUTReceived())
	{
		//wait for host to send status
	}
	//Endpoint_ClearOUT();//send message
	//Endpoint_ClearStatusStage();//success :D
}

void usbGetButtons()
{
	Endpoint_ClearSETUP();//ack setup packet
	u8 sendData = 0;
	while (sendData < 3)
	{
		while (!Endpoint_IsINReady())
		{
			//wait until host is ready
		}
		u8 state = stateButton(but1);;
		Endpoint_Write_8(state);
		sendData++;
		state = stateButton(but2);;
		Endpoint_Write_8(state);
		sendData++;
		state = stateButton(but3);;
		Endpoint_Write_8(state);
		sendData++;
		Endpoint_ClearIN();
	}
	//while (!Endpoint_IsOUTReceived())
	{
		//wait for host to send status
	}
	//Endpoint_ClearOUT();//send message
	//Endpoint_ClearStatusStage();//success :D
}

void usbGetEZ3()
{
	Endpoint_ClearSETUP();//ack setup packet
	u8 sendData = 0;
	while (sendData < 2)
	{
		while (!Endpoint_IsINReady())
		{
			//wait until host is ready
		}
		u16 state = measureEZ3(ez3);
		Endpoint_Write_16_BE(state);//write as big endian
		sendData += 2;
		Endpoint_ClearIN();
	}
	//while (!Endpoint_IsOUTReceived())
	{
		//wait for host to send status
	}
	//Endpoint_ClearOUT();//send message
	//Endpoint_ClearStatusStage();//success :D
}

void usbGetServoReady()
{
	Endpoint_ClearSETUP();//ack setup packet
	u8 sendData = 0;
	while (sendData < 1)
	{
		while (!Endpoint_IsINReady())
		{
			//wait until host is ready
		}
		u8 state = checkReadyServo(servo);
		Endpoint_Write_8(state);
		sendData++;
		Endpoint_ClearIN();
	}
	//while (!Endpoint_IsOUTReceived())
	{
		//wait for host to send status
	}
	//Endpoint_ClearOUT();//send message
	//Endpoint_ClearStatusStage();//success :D
}

void usbGetTemperature()
{
	Endpoint_ClearSETUP();//ack setup packet
	u8 sendData = 0;
	measureTemperature();
	while (sendData < 2)
	{
		while (!Endpoint_IsINReady())
		{
			//wait until host is ready
		}
		u16 value = measureTemperature();
		Endpoint_Write_8(value>>8);
		sendData++;
		Endpoint_Write_8(value & 0xff);
		sendData++;
		Endpoint_ClearIN();
	}
	//while (!Endpoint_IsOUTReceived())
	{
		//wait for host to send status
	}
	//Endpoint_ClearOUT();//send message
	//Endpoint_ClearStatusStage();//success :D

}
#endif
