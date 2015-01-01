#include <stdio.h>
#include <simavr/sim_avr.h>
#include <simavr/avr_uart.h>

#define FIRMWARE_LOCATION "uart.hex"

uint8_t state = 0;
avr_irq_t * uart_irq;

void uart_output_irq(struct avr_irq_t * irq, uint32_t value, void * param)
{
	printf("'%c' fogadva\n", value);
}

const char * simulator_get_firmware()
{
	// visszaadja a hex fájl elérhetőségét
	return FIRMWARE_LOCATION;
}

void simulator_setup(avr_t * avr)
{
	// az UART konfigurálása
	avr_irq_register_notify(avr_io_getirq(avr, AVR_IOCTL_UART_GETIRQ('0'), UART_IRQ_OUTPUT),
	                        uart_output_irq, NULL);
	
	uart_irq = avr_io_getirq(avr, AVR_IOCTL_UART_GETIRQ('0'), UART_IRQ_INPUT);
}

void simulator_loop(avr_t * avr)
{
	switch( state )
	{
		case 0:
			if( avr->cycle > 200000 )
			{
				state++;
				printf("'A' elküldve\n");
				avr_raise_irq(uart_irq, 'A');
			}
			break;
		case 1:
			if( avr->cycle > 400000 )
			{
				state++;
				printf("'D' elküldve\n");
				avr_raise_irq(uart_irq, 'D');
			}
			break;
		case 2:
			if( avr->cycle > 600000 )
			{
				state++;
				printf("'Q' elküldve\n");
				avr_raise_irq(uart_irq, 'Q');
			}
			break;
	}
}
