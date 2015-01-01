TARGET = i2c_slave_simulator
LIBS = -lm -lsimavr
CC = gcc
CFLAGS = -g -Wall

all: $(TARGET)

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c)) core.o
HEADERS = $(wildcard *.h)

core.o: ../../simulator_core/core.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@.elf

clean:
	-rm -f *.o
	-rm -f $(TARGET).elf
