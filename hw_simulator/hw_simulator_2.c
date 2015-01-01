#include <avr/io.h>
#include <util/delay.h>

#define TIME_200_ms (F_CPU / 5 / 1024)

int main()
{
  DDRD = 0xFF; // D port kimenetre
  DDRB = _BV(0); // B port 0. bitje kimenetre
  
  PORTD = 0;   // összes LED kikapcsolása
  PORTB = _BV(1) | _BV(2) | _BV(3); // felhúzóellenállások bekapcsolása a nyomógombokon

  uint8_t last_pinb = PINB & (_BV(1) | _BV(2) | _BV(3));
  
  TCCR1A = 0;
  TCCR1B = _BV(CS10) | _BV(CS12);
  
  uint16_t last_tcnt1 = TCNT1;
  uint8_t is_on = 1;
  uint8_t blinking = 4;
  uint8_t speed = 1;
  uint8_t speeds[] = {1, 3, 6, 10};
  
  uint16_t limit = TIME_200_ms * (speeds[speed]);
  TCNT1 += limit;
  
  while(1)
  {
    uint16_t elapsed = TCNT1 - last_tcnt1;
    if( elapsed >= limit )
    {
      last_tcnt1 += limit;
      
      uint16_t leds = 0;
      if( is_on )
      {
	leds = (1<< blinking);
      }
      
      // LED-ek állítása
      PORTD = leds & 255;
      PORTB = (PORTB & 0xFE) | (leds >> 8);
      is_on = !is_on;
    }
    
    uint8_t this_pinb = PINB & (_BV(1) | _BV(2) | _BV(3));

    // gombok beolvasása és num állítása annak függvényében    
    if( this_pinb != last_pinb )
    {
      uint8_t change = this_pinb ^ last_pinb; // melyik változott
      
      if( (change & _BV(1)) && ( ! (this_pinb & _BV(1) ) ) && (blinking > 0 ))
      {
        blinking--;
        is_on = 1;
        last_tcnt1 = TCNT1 - limit;
      }
      if( (change & _BV(3)) && ( ! (this_pinb & _BV(3) ) ) && (blinking < 8 ))
      {
        blinking++;
        is_on = 1;
        last_tcnt1 = TCNT1 - limit;
      }
      if( (change & _BV(2)) && ( ! (this_pinb & _BV(2) ) ))
      {
        speed = (speed + 1) & 3;
	limit = TIME_200_ms * (speeds[speed]);
      }
      last_pinb = this_pinb;
      _delay_ms(30); // prell mentesítés
    }
  }

  return 0;
}
