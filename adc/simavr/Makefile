include ../../Makefile.common

TARGET = adc_simulator
LIBS = -lm -lsimavr $(GCC-LDFLAGS)
CC = gcc
CFLAGS = -g -Wall $(GCC-CFLAGS)

all: $(TARGET)

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c)) core.o
HEADERS = $(wildcard *.h)

core.o: ../../simulator_core/core.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@$(EXTENSION)

clean:
	-rm -f *.o
	-rm -f $(TARGET)$(EXTENSION)

run:
	echo === Running adc tests ===
	./adc_simulator$(EXTENSION)
