TARGET=uc
MCU=atmega32u4
ARCH=AVR8
BOARD=USBKEY
F_CPU=16000000
F_USB=$(F_CPU)
F_CLOCK=$(F_CPU)
CONTROLLER=-mmcu=$(MCU)
SRC=debug.c descriptors.c led.c gpio.c uc.c usb.c
LUFA_PATH=/home/dtk/Downloads/LUFA-130901/LUFA
include $(LUFA_PATH)/Build/lufa_core.mk
include $(LUFA_PATH)/Build/lufa_build.mk

CC=avr-gcc
USBFLAGS=-DUSE_FLASH_DESCRIPTORS -DUSE_STATIC_OPTIONS="(USE_DEVICE_OPT_FULLSPEED | USB_OPT_AUTO_PLL)" -DUSB_DEVICE_ONLY -DCONTROL_ONLY_DEVICE
CC_FLAGS=-c -Wall -std=c99 $(CONTROLLER) -DF_CPU=$(F_CPU) -DF_USB=$(F_USB) -DMCU=$(MCU) -DARCH=$(ARCH) -DBOARD=$(BOARD) -DF_CLOCK=$(F_CLOCK) $(USBFLAGS) -DDEBUG -DUSB -I$(LUFA_PATH)/Drivers/USB/ -I$(LUFA_PATH)/Drivers/USB/Core/AVR8/

MKDIR=mkdir -p
RM=rm -f
RMDIR=rm -rf
CP=cp -f
CPDIR=cp -rf
MV=mv -f
ECHO=@echo
INSTALL=apt-get install -y

.PHONY: all time help installdep version major minor build clean commit doc trac flash uc

all: uc

installdep:
	$(INSTALL) gcc-avr
	$(INSTALL) doxygen
	$(INSTALL) git
	$(INSTALL) firefox
	$(INSTALL) binutils
	$(INSTALL) tar
	$(INSTALL) python
	$(INSTALL) trac
	$(INSTALL) trac-git
	$(INSTALL) avr-libc
	$(INSTALL) binutils-avr
	$(INSTALL) avrdude

flash: uc.hex
	avrdude -p m32u4 -P /dev/ttyACM0 -c avr109 -U flash:w:$<:i
	make build

clean:
	$(RM) *.o
	$(RM) *.d
	$(RM) *.elf
	$(RM) *.hex
	$(RM) *.map
	$(RM) *.eep

trac:
	tracd -s --port 8000 ./.trac &
