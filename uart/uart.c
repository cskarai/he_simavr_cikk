#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define BAUD_RATE 9600

// adat fogadása
ISR(USART_RX_vect)
{
  uint8_t data = UDR0;

  switch(data)
  {
    case 'Q': // kilépés
      {
        cli();
        sleep_mode();
      }
      break;
    default: // egyébként adjunk vissza +1-et
      UDR0 = data + 1;
      break;
  }
}

int main()
{
  DDRD = _BV(1); // TX kimenetre állítása
  PORTD = _BV(0) | _BV(1); // a felhúzó ellenállás RX-en és TX bekapcsolva
  
  UCSR0A = _BV(U2X0); // Dupla sebességű mód
  UCSR0B = _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0); // RX/TX engedélyezve, RX-en megszakítás
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01); // 8 bit
  UBRR0 = (uint8_t)( (F_CPU + BAUD_RATE * 4L) / (BAUD_RATE * 8L) - 1 ); // bitráta

  sei();

  while(1);

  return 0;
}
