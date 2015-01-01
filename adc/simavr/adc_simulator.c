#include <stdio.h>
#include <string.h>
#include <simavr/sim_avr.h>
#include <simavr/avr_ioport.h>
#include <simavr/avr_adc.h>

#define FIRMWARE_LOCATION "adc.hex"

int16_t adc = 0;

void portb_5_changed(struct avr_irq_t * irq, uint32_t value, void * param)
{
	if( value )
		printf("LED be!\n");
	else
		printf("LED ki!\n");
}

void adc_out_trigger(struct avr_irq_t * irq, uint32_t value, void * param)
{
	avr_t * avr = (avr_t *)param;
	
	avr_adc_mux_t mux;
	memcpy(&mux, &value, sizeof(mux));
	
	
	adc += 200;
	
	int16_t mv = (adc >= 4000) ? 8000 - adc : adc;
	if( mv < 0 )
		mv = 4999;
	printf("ADC%d: %d mv\n", mux.src, mv);
	
	avr_raise_irq(avr_io_getirq(avr, AVR_IOCTL_ADC_GETIRQ, mux.src), mv);
}

const char * simulator_get_firmware()
{
	// visszaadja a hex fájl elérhetőségét
	return FIRMWARE_LOCATION;
}

void simulator_setup(avr_t * avr)
{
	// beállítjuk AVCC-t 5V-ra
	avr->avcc = 5000;
	
	// beállítjuk, hogy hívjon megszakítást a PORTB 5-ös pinjére
	avr_irq_register_notify(
		avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('B'), 5),
		portb_5_changed, 
		NULL);
	avr_irq_register_notify(
		avr_io_getirq(avr, AVR_IOCTL_ADC_GETIRQ, ADC_IRQ_OUT_TRIGGER),
		adc_out_trigger, 
		avr);
}

void simulator_loop(avr_t * avr)
{
}
