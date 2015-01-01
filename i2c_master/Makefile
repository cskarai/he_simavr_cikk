include ../Makefile.common

PROJECT=i2c_master
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

I2C_FLAGS=-DTWI_FREQ=50000

all : $(PROJECT).src $(PROJECT).hex
	

%.o : %.c
	$(AVR-CC) -c $(AVR-CFLAGS) $(I2C_FLAGS) $< -o $@

%.obj : $(OBJS)
	$(AVR-CC) $(AVR-CFLAGS) $(OBJS) -o $@ $(AVR-LDFLAGS)
	

%.src : $(PROJECT).obj
	$(AVR-OBJDUMP) -S --demangle $(PROJECT).obj >$@
	$(AVR-OBJDUMP) -EB -s $(PROJECT).obj >>$@
#	$(AVR-OBJDUMP) -EB -s -j .data $< >>$@

%.hex : $(PROJECT).obj
	$(AVR-OBJ2HEX) -R .eeprom -O ihex $(PROJECT).obj simavr/$@

clean :
	rm -f simavr/*.hex *.obj *.o *.src
