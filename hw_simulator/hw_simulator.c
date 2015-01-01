#include <avr/io.h>
#include <util/delay.h>

int main()
{
  DDRD = 0xFF; // D port kimenetre
  DDRB = _BV(0); // B port 0. bitje kimenetre
  
  PORTD = 0;   // összes LED kikapcsolása
  PORTB = _BV(1) | _BV(2) | _BV(3); // felhúzóellenállások bekapcsolása a nyomógombokon

  int8_t num = 0;
  uint8_t last_pinb = PINB & (_BV(1) | _BV(2) | _BV(3));
  
  while(1)
  {
    uint8_t  absnum = (num < 0) ? -num: num; // abszolútérték
    uint8_t  mask = (1 << absnum) - 1;
    uint16_t leds = 0x10;
    if( num < 0 )
      leds |= (mask << (4 - absnum));
    if( num > 0 )
      leds |= (mask << 5);
    
    // LED-ek állítása
    PORTD = leds & 255;
    PORTB = (PORTB & 0xFE) | (leds >> 8);


    uint8_t this_pinb = PINB & (_BV(1) | _BV(2) | _BV(3));

    // gombok beolvasása és num állítása annak függvényében    
    if( this_pinb != last_pinb )
    {
      uint8_t change = this_pinb ^ last_pinb; // melyik változott
      
      if( (change & _BV(1)) && ( ! (this_pinb & _BV(1) ) ) && (num > -4 ))
        num--;
      if( (change & _BV(3)) && ( ! (this_pinb & _BV(3) ) ) && (num <  4 ))
        num++;
      if( (change & _BV(2)) && ( ! (this_pinb & _BV(2) ) ))
        num = 0;
      last_pinb = this_pinb;
      _delay_ms(30); // prell mentesítés
    }
  }

  return 0;
}
