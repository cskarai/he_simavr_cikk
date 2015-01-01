#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define SPI_SCK               5
#define SPI_MISO              4
#define SPI_MOSI              3
#define SPI_SS                2

#define SPI_DDR               DDRB
#define SPI_PORT              PORTB
#define SPI_PIN               PINB

int main()
{
  SPI_DDR &= ~( _BV(SPI_MOSI) | _BV(SPI_SCK) | _BV(SPI_SS) );
  SPI_PORT |= _BV(SPI_MOSI) | _BV(SPI_SS) | _BV(SPI_SCK);
  SPI_DDR |= _BV(SPI_MISO);
  SPI_PORT &= ~_BV(SPI_MISO);

  SPCR =  _BV(SPR0) | _BV(SPE) | _BV(MSTR);
  SPSR = 0;

  _delay_ms(10);
  SPDR = 'A';
  while (!(SPSR & (1 << SPIF)));

  uint8_t cmd = SPDR;
  _delay_ms(10);
  SPDR = cmd + 1; 

  while (!(SPSR & (1 << SPIF)));

  cmd = SPDR;
  _delay_ms(10);
  SPDR = cmd + 1; 

  while (!(SPSR & (1 << SPIF)));

  cli();
  sleep_mode();

  return 0;
}
