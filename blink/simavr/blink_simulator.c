#include <stdio.h>
#include <simavr/sim_avr.h>
#include <simavr/avr_ioport.h>

#define FIRMWARE_LOCATION "blink.hex"

void portb_5_changed(struct avr_irq_t * irq, uint32_t value, void * param)
{
	if( value )
		printf("LED be!\n");
	else
		printf("LED ki!\n");
}

const char * simulator_get_firmware()
{
	// visszaadja a hex fájl elérhetőségét
	return FIRMWARE_LOCATION;
}

void simulator_setup(avr_t * avr)
{
	// beállítjuk, hogy hívjon megszakítást a PORTB 5-ös pinjére
	avr_irq_register_notify(
		avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('B'), 5),
		portb_5_changed, 
		NULL);
}


void simulator_loop(avr_t * avr)
{
	// ne csinálj semmit
}
