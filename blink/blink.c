#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

int main()
{
  DDRB = _BV(5);
  uint8_t cnt = 10;
  while(--cnt)
  {
    PORTB ^= _BV(5);
    _delay_ms(250);
  }

  // az interrupt tiltása és a sleep együtt kilépést jelent simavr alatt
  cli();
  sleep_mode();
  
  return 0;
}
