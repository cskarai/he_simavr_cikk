#include <stdio.h>
#include <simavr/sim_avr.h>
#include <simavr/avr_ioport.h>

#define FIRMWARE_LOCATION "push_button.hex"

uint8_t     state = 0;
avr_irq_t * button1_irq;
avr_irq_t * button2_irq;


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
	
	button1_irq = avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('D'), 0);
	button2_irq = avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('D'), 1);
	
	avr_raise_irq(button1_irq, 1);
	avr_raise_irq(button2_irq, 1);
}

void simulator_loop(avr_t * avr)
{
	switch(state)
	{
		case 0:
			if( avr->cycle >= 250000 )
			{
				printf("LED gomb lenyomva!\n");
				avr_raise_irq(button1_irq, 0);
				state++;
			}
			break;
		case 1:
			if( avr->cycle >= 270000 )
			{
				printf("LED gomb felengedve!\n");
				avr_raise_irq(button1_irq, 1);
				state++;
			}
			break;
		case 2:
			if( avr->cycle >= 500000 )
			{
				printf("LED gomb lenyomva!\n");
				avr_raise_irq(button1_irq, 0);
				state++;
			}
			break;
		case 3:
			if( avr->cycle >= 520000 )
			{
				printf("LED gomb felengedve!\n");
				avr_raise_irq(button1_irq, 1);
				state++;
			}
			break;
		case 4:
			if( avr->cycle >= 550000 )
			{
				printf("Kilépés gomb lenyomva!\n");
				avr_raise_irq(button2_irq, 0);
				state++;
			}
			break;
	}
}
