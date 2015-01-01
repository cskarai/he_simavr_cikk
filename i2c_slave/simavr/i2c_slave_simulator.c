#include <stdio.h>
#include <simavr/sim_avr.h>
#include <simavr/avr_twi.h>

#define FIRMWARE_LOCATION "i2c_slave.hex"

const char * simulator_get_firmware()
{
	// visszaadja a hex fájl elérhetőségét
	return FIRMWARE_LOCATION;
}

void simulator_setup(avr_t * avr)
{
}


void simulator_loop(avr_t * avr)
{
	// ne csinálj semmit
}
