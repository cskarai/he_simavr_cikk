include ../Makefile.common

PROJECT_1=hw_simulator
PROJECT_2=hw_simulator_2
SRCS_1=hw_simulator.c
SRCS_2=hw_simulator_2.c
OBJS_1=$(SRCS_1:.c=.o)
OBJS_2=$(SRCS_2:.c=.o)

all : $(PROJECT_1).src $(PROJECT_1).hex $(PROJECT_2).src $(PROJECT_2).hex
	

%.o : %.c
	$(AVR-CC) -c $(AVR-CFLAGS) $< -o $@

$(PROJECT_1).obj : $(OBJS_1)
	$(AVR-CC) $(AVR-CFLAGS) $(OBJS_1) -o $@ $(AVR-LDFLAGS)
	

$(PROJECT_1).src : $(PROJECT_1).obj
	$(AVR-OBJDUMP) -S --demangle $(PROJECT_1).obj >$@
	$(AVR-OBJDUMP) -EB -s $(PROJECT_1).obj >>$@
#	$(AVR-OBJDUMP) -EB -s -j .data $< >>$@

$(PROJECT_1).hex : $(PROJECT_1).obj
	$(AVR-OBJ2HEX) -R .eeprom -O ihex $(PROJECT_1).obj simavr/$@



$(PROJECT_2).obj : $(OBJS_2)
	$(AVR-CC) $(AVR-CFLAGS) $(OBJS_2) -o $@ $(AVR-LDFLAGS)
	

$(PROJECT_2).src : $(PROJECT_2).obj
	$(AVR-OBJDUMP) -S --demangle $(PROJECT_2).obj >$@
	$(AVR-OBJDUMP) -EB -s $(PROJECT_2).obj >>$@
#	$(AVR-OBJDUMP) -EB -s -j .data $< >>$@

$(PROJECT_2).hex : $(PROJECT_2).obj
	$(AVR-OBJ2HEX) -R .eeprom -O ihex $(PROJECT_2).obj simavr/$@

clean :
	rm -f simavr/*.hex *.obj *.o *.src
