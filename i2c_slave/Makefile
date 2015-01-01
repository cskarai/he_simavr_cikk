CC=/usr/bin/avr-gcc
CFLAGS=-g -Wall -Os -finline-limit=2 -mmcu=atmega328p -DF_CPU=1000000UL
LDFLAGS=-lm
OBJ2HEX=/usr/bin/avr-objcopy 
OBJDUMP=/usr/bin/avr-objdump
PROJECT=i2c_slave
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

all : $(PROJECT).src $(PROJECT).hex
	

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.obj : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)
	

%.src : $(PROJECT).obj
	$(OBJDUMP) -S --demangle $(PROJECT).obj >$@
	$(OBJDUMP) -EB -s $(PROJECT).obj >>$@
#	$(OBJDUMP) -EB -s -j .data $< >>$@

%.hex : $(PROJECT).obj
	$(OBJ2HEX) -R .eeprom -O ihex $(PROJECT).obj simavr/$@

clean :
	rm -f simavr/*.hex *.obj *.o *.src
