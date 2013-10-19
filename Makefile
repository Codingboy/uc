TARGET=$(BIN)/uc
ARGS=

MCU=atmega32u4
ARCH=AVR8
BOARD=USBKEY
F_CPU=16000000
F_USB=$(F_CPU)
F_CLOCK=$(F_CPU)
CONTROLLER=-mmcu=$(MCU)

BIN=bin
SBIN=sbin
SRC=src
OBJ=obj
INCLUDE=include
LIB=lib
EXTLIB=extlib
SCRIPT=script
DOC=doc

CC=avr-gcc
USBFLAGS=-DUSE_FLASH_DESCRIPTORS -DUSE_STATIC_OPTIONS="(USE_DEVICE_OPT_FULLSPEED | USB_OPT_AUTO_PLL)" -DUSB_DEVICE_ONLY -DCONTROL_ONLY_DEVICE
CFLAGS=-c -Wall -g -std=c99 -Os -I$(INCLUDE) $(CONTROLLER) -DF_CPU=$(F_CPU) -DF_USB=$(F_USB) -DMCU=$(MCU) -DARCH=$(ARCH) -DBOARD=$(BOARD) -DF_CLOCK=$(F_CLOCK) $(USBFLAGS)
LFLAGS=-L$(LIB)

MKDIR=mkdir -p
RM=rm -f
RMDIR=rm -rf
CP=cp -f
CPDIR=cp -rf
MV=mv -f
ECHO=@echo
INSTALL=apt-get install -y

MODULES_=debug led button gpio
MODULES=$(addsuffix .o, $(addprefix $(OBJ)/, $(MODULES_)))

.PHONY: all time help installdep version major minor build clean commit doc trac flash uc

all: uc

help:
	$(ECHO) "uc"
	$(ECHO) "all"
	$(ECHO) "help"
	$(ECHO) "doc"
	$(ECHO) "major"
	$(ECHO) "minor"
	$(ECHO) "commit"
	$(ECHO) "version"
	$(ECHO) "clean"
	$(ECHO) "installdep"
	$(ECHO) "flash"

installdep:
	$(INSTALL) avr-gcc
	$(INSTALL) doxygen
	$(INSTALL) git
	$(INSTALL) firefox
	$(INSTALL) binutils
	$(INSTALL) tar
	$(INSTALL) python
	$(INSTALL) trac
	$(INSTALL) trac-git
	$(INSTALL) avr-libc
	$(INSTALL) gcc-avr
	$(INSTALL) binutils-avr
	$(INSTALL) avrdude

flash: $(BIN)/uc.hex
	avrdude -p m32u4 -P /dev/ttyACM0 -c avr109 -U flash:w:$<:i
	make build

$(BIN)/uc.elf: $(OBJ)/uc.o $(MODULES)
	$(MKDIR) $(BIN)
	$(CC) $(CONTROLLER) -o $@ $^

$(BIN)/uc.hex: $(BIN)/uc.elf
	$(MKDIR) $(BIN)
	avr-objcopy -j .text -j .data -O ihex $^ $@

uc: flash

$(LIB)/libuc.a: $(MODULES)
	$(MKDIR) $(LIB)
	ar r $@ $^

major:
	python $(SCRIPT)/v.py $(INCLUDE)/version.h --major
	git add $(SRC)/*
	git add $(INCLUDE)/*
	git commit -a
	VERSION=$$(python $(SCRIPT)/v.py $(INCLUDE)/version.hpp);\
	VERSION="v"$$VERSION;\
	git tag -a $$VERSION -m "major build";\
	trac-admin ./.trac version add $$VERSION
	git checkout development
	git merge topic
	git checkout release
	git merge development
	git checkout topic

minor:
	python $(SCRIPT)/v.py $(INCLUDE)/version.h --minor
	git add $(SRC)/*
	git add $(INCLUDE)/*
	git commit -a
	VERSION=$$(python $(SCRIPT)/v.py $(INCLUDE)/version.hpp);\
	VERSION="v"$$VERSION;\
	git tag -a $$VERSION -m "minor build";\
	trac-admin ./.trac version add $$VERSION
	git checkout development
	git merge topic
	git checkout topic

build:
	python $(SCRIPT)/v.py $(INCLUDE)/version.h --build
	git add $(SRC)/*
	git add $(INCLUDE)/*
	git commit -am "normal build"
	VERSION=$$(python $(SCRIPT)/v.py $(INCLUDE)/version.hpp);\
	VERSION="v"$$VERSION;\
	git tag -a $$VERSION -m "normal build";\
	trac-admin ./.trac version add $$VERSION

commit:
	git add $(SRC)
	git add $(INCLUDE)
	git commit -a

version:
	python $(SCRIPT)/v.py $(INCLUDE)/version.h

clean:
	$(RMDIR) $(BIN)
	$(RMDIR) $(OBJ)
	$(RMDIR) $(LIB)
	$(RMDIR) $(DOC)
	$(RM) cachegrind.*
	$(RM) core
	$(RM) vgcore.*
	$(RM) $(SRC)/.*.swp
	$(RM) $(INCLUDE)/.*.swp
	$(RM) $(SRC)/*~
	$(RM) $(INCLUDE)/*~
	$(RM) doxygen.log

doc: Doxyfile $(SRC)/* $(INCLUDE)/* mainpage
	$(MKDIR) $(DOC)
	doxygen

$(OBJ)/%.o: $(SRC)/%.c $(INCLUDE)/%.h
	$(MKDIR) $(OBJ)
	$(CC) $(CFLAGS) -o $@ $<

$(INCLUDE)/%.h:
	touch $@

trac:
	tracd -s --port 8000 ./.trac &
