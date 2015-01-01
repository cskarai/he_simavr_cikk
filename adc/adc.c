#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define LIMIT_HIGH 600
#define LIMIT_LOW  400

int main()
{
  DDRB = _BV(5);
  ADMUX = _BV(REFS0) | 0;
  ADCSRB = 0;
  ADCSRA = _BV(ADSC) | _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);

  uint8_t state = 0;

  while(1)
  {
    while( ( ADCSRA & _BV(ADSC) ) );
    uint16_t val = ADC;

    if( val >= 1000 )
    {
      // az interrupt tiltása és a sleep együtt kilépést jelent simavr alatt
      cli();
      sleep_mode();
    }

    if( (!state) && (val >= LIMIT_HIGH) )
    {
      state = 1;
      PORTB |= _BV(5);
    }
    else if( (state) && (val <= LIMIT_LOW) )
    {
      state = 0;
      PORTB &= ~_BV(5);
    }
    
    ADCSRA |= _BV(ADSC);
  }

  return 0;
}
