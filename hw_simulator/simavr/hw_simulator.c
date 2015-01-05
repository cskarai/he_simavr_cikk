#include <stdio.h>
#include <simavr/sim_avr.h>
#include <simavr/avr_ioport.h>
#include "hw_simulator_ui.h"

#define FIRMWARE_LOCATION "hw_simulator.hex"

uint8_t     state = 0;
avr_irq_t * button_irq[3];

uint8_t     led_numbers[9];
uint8_t     pinb_state = 0xE;

void pin_changed(struct avr_irq_t * irq, uint32_t value, void * param)
{
	uint8_t num = *(uint8_t *)param;
	if( value )
		printf("LED%d be!\n", num);
	else
		printf("LED%d ki!\n", num);
	
	hw_simulator_ui_set_led_state(num, value);
}

const char * simulator_get_firmware()
{
	// visszaadja a hex fájl elérhetőségét
	return FIRMWARE_LOCATION;
}

void simulator_setup(avr_t * avr)
{
	hw_simulator_ui_init();
	
	// beállítjuk, hogy hívjon megszakítást ha a D0-7 vagy B0 változik
	uint8_t i;
	for( i=0; i < 8; i++ )
	{
		led_numbers[i] = i;
		avr_irq_register_notify(
			avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('D'), i),
			pin_changed, 
			&led_numbers[i]);
	}
	led_numbers[8]=8;
	avr_irq_register_notify(
		avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('B'), 0),
		pin_changed, 
		&led_numbers[8]);
	
	for( i=0; i < 3; i++ )
	{
	  button_irq[i] = avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('B'), i + 1);
	  avr_raise_irq(button_irq[i], 1);
	}
	
}

void simulator_loop(avr_t * avr)
{
	int event = hw_simulator_ui_poll();
	
	switch(event)
	{
		case BUTTON_1_PRESSED:
			printf("Nyomógomb 1 lenyomva\n");
			avr_raise_irq(button_irq[0], 0);
			pinb_state &= ~2;
			break;
		case BUTTON_1_RELEASED:
			printf("Nyomógomb 1 felengedve\n");
			avr_raise_irq(button_irq[0], 1);
			pinb_state |= 2;
			break;
		case BUTTON_2_PRESSED:
			printf("Nyomógomb 2 lenyomva\n");
			avr_raise_irq(button_irq[1], 0);
			pinb_state &= ~4;
			break;
		case BUTTON_2_RELEASED:
			printf("Nyomógomb 2 felengedve\n");
			avr_raise_irq(button_irq[1], 1);
			pinb_state |= 4;
			break;
		case BUTTON_3_PRESSED:
			printf("Nyomógomb 3 lenyomva\n");
			avr_raise_irq(button_irq[2], 0);
			pinb_state &= ~8;
			break;
		case BUTTON_3_RELEASED:
			printf("Nyomógomb 3 felengedve\n");
			avr_raise_irq(button_irq[2], 1);
			pinb_state |= 8;
			break;
		case QUIT:
			// kilépés
			avr->state = cpu_Done;
			break;
		case NO_OPERATION:
		default:
			break;
	}
	
	avr_ioport_external_t e = {
		.name = 'B',
		.mask = 14,
		.value = pinb_state,
	};
	avr_ioctl((avr_t *)avr, AVR_IOCTL_IOPORT_SET_EXTERNAL(e.name), &e);
}
