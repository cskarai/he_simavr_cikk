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

// SPI adat fogadása
ISR(SPI_STC_vect)
{
  uint8_t cmd = SPDR;
  SPDR = cmd + 1;
  if( cmd == 'Q' ) // kilépés az emulátorból
  {
    cli();
    sleep_mode();
  }
}

int main()
{
  SPI_DDR &= ~( _BV(SPI_MOSI) | _BV(SPI_SCK) | _BV(SPI_SS) );
  SPI_PORT |= _BV(SPI_MOSI) | _BV(SPI_SS) | _BV(SPI_SCK);
  SPI_DDR |= _BV(SPI_MISO);
  SPI_PORT &= ~_BV(SPI_MISO);

  SPCR =  _BV(SPR0) | _BV(SPE) | _BV(SPIE);
  SPSR = 0;

  SPDR = '?';

  sei();

  while(1);

  return 0;
}
