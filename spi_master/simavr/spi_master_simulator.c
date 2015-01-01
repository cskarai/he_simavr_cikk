#include <stdio.h>
#include <simavr/sim_avr.h>
#include <simavr/avr_spi.h>

#define FIRMWARE_LOCATION "spi_master.hex"

uint8_t state = 0;
avr_irq_t * spi_irq;

void spi_output_irq(struct avr_irq_t * irq, uint32_t value, void * param)
{
	// itt beállítjuk, hogy mit kellene visszaküldeni
	// meglepő, mert SPI alatt amikor az interrupt meghívódik,
	// az adatnak már a pufferben kellene lennie
	avr_raise_irq(spi_irq, value + 1);
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
	avr_irq_register_notify(avr_io_getirq(avr, AVR_IOCTL_SPI_GETIRQ(0), SPI_IRQ_OUTPUT),
	                        spi_output_irq, NULL);
	
	spi_irq = avr_io_getirq(avr, AVR_IOCTL_SPI_GETIRQ(0), SPI_IRQ_INPUT);
}

void simulator_loop(avr_t * avr)
{
}
