#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

int main()
{
  DDRD = 0;
  PORTD = _BV(0) | _BV(1);
  DDRB = _BV(5);
  uint8_t last_pind = PIND & _BV(0);

  while(1)
  {
    if( ! ( PIND & _BV(1) ) )
    {
      // az interrupt tiltása és a sleep együtt kilépést jelent simavr alatt
      cli();
      sleep_mode();
    }
    uint8_t this_pind = PIND & _BV(0);
    if( this_pind != last_pind )
    {
      _delay_ms(30); // prell mentesítés
      last_pind = this_pind;
      if( ! this_pind )
        PINB = _BV(5); // LED állapotának megfordítása a gomb lenyomásakor
    }
  }

  return 0;
}
