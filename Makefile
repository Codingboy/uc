LUFA_PATH=/home/bla/Downloads/LUFA-130901/LUFA

BINDIR=bin
SRCDIR=src
INCLUDEDIR=include
OBJDIR=obj
DOCDIR=doc

TARGET=bin/uc
MCU=atmega32u4
ARCH=AVR8
BOARD=USBKEY
F_CPU=16000000
F_USB=$(F_CPU)
F_CLOCK=$(F_CPU)
CONTROLLER=-mmcu=$(MCU)
C_STANDARD=c99

CC=avr-gcc
USBFLAGS=-DUSE_FLASH_DESCRIPTORS -DUSE_STATIC_OPTIONS="(USE_DEVICE_OPT_FULLSPEED | USB_OPT_AUTO_PLL)" -DUSB_DEVICE_ONLY -DUSE_DEVICE_OPT_FULLSPEED -DUSB_STREAM_TIMEOUT_MS=500 -DNO_SOF_EVENTS -DCONTROL_ONLY_DEVICE -DNO_DEVICE_REMOTE_WAKEUP -DNO_DEVICE_SELF_POWER
#CC_FLAGS=-c -Wall -std=c99 $(CONTROLLER) -DF_CPU=$(F_CPU) -DF_USB=$(F_USB) -DMCU=$(MCU) -DARCH=$(ARCH) -DBOARD=$(BOARD) -DF_CLOCK=$(F_CLOCK) $(USBFLAGS) -DDEBUG -I$(LUFA_PATH)/Drivers/USB/ -I$(LUFA_PATH)/Drivers/USB/Core/AVR8/ -DIN_EPNUM=1 -DOUT_EPNUM=2 -DIO_EPSIZE=8 -Iinclude -DUSB
CC_FLAGS=$(USBFLAGS) -DDEBUG -I$(LUFA_PATH)/Drivers/USB/ -I$(LUFA_PATH)/Drivers/USB/Core/AVR8/ -DIN_EPNUM=1 -DOUT_EPNUM=2 -DIO_EPSIZE=8 -Iinclude -DUSB

MKDIR=mkdir -p
RM=rm -f
RMDIR=rm -rf
CP=cp -f
CPDIR=cp -rf
MV=mv -f
ECHO=@echo
INSTALL=apt-get install -y

.PHONY: all time help installdep version major minor build clean commit doc trac flash uc

include $(LUFA_PATH)/Build/lufa_sources.mk
SRC=$(SRCDIR)/debug.c $(SRCDIR)/descriptors.c $(SRCDIR)/led.c $(SRCDIR)/gpio.c $(SRCDIR)/uc.c $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS) $(LUFA_SRC_PLATFORM)
include $(LUFA_PATH)/Build/lufa_core.mk
include $(LUFA_PATH)/Build/lufa_build.mk

all: flash

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

flash: hex
	avrdude -p m32u4 -P /dev/ttyACM0 -c avr109 -U flash:w:$<:i
	make build

clean:
	$(RM) $(OBJDIR)/*
	$(RM) $(BINDIR)/*
	$(RMDIR) $(DOCDIR)

trac:
	tracd -s --port 8000 ./.trac &

major:
	python $(SCRIPTDIR)/v.py $(INCLUDEDIR)/version.h --major
	git add $(SRCDIR)/*
	git add $(INCLUDEDIR)/*
	git commit -a
	VERSION=$$(python $(SCRIPTDIR)/v.py $(INCLUDEDIR)/version.hpp);\
	VERSION="v"$$VERSION;\
	git tag -a $$VERSION -m "major build";\
	trac-admin ./.trac version add $$VERSION
	git checkout development
	git merge topic
	git checkout release
	git merge development
	git checkout topic

minor:
	python $(SCRIPTDIR)/v.py $(INCLUDEDIR)/version.h --minor
	git add $(SRCDIR)/*
	git add $(INCLUDEDIR)/*
	git commit -a
	VERSION=$$(python $(SCRIPTDIR)/v.py $(INCLUDEDIR)/version.hpp);\
	VERSION="v"$$VERSION;\
	git tag -a $$VERSION -m "minor build";\
	trac-admin ./.trac version add $$VERSION
	git checkout development
	git merge topic
	git checkout topic

build:
	python $(SCRIPTDIR)/v.py $(INCLUDEDIR)/version.h --build
	git add $(SRCDIR)/*
	git add $(INCLUDEDIR)/*
	git commit -am "normal build"
	VERSION=$$(python $(SCRIPTDIR)/v.py $(INCLUDEDIR)/version.hpp);\
	VERSION="v"$$VERSION;\
	git tag -a $$VERSION -m "normal build";\
	trac-admin ./.trac version add $$VERSION

commit:
	git add $(SRCDIR)
	git add $(INCLUDEDIR)
	git commit -a

version:
	python $(SCRIPTDIR)/v.py $(INCLUDEDIR)/version.h
