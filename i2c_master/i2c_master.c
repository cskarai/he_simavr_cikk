#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "i2c_master_lib.h"


volatile uint8_t state = 0;

void i2c_callback(uint8_t type, uint8_t * messageBuf, uint8_t len)
{
  if( type == TWI_MASTER_SEND_DATA_COMPLETED )
  {
    state ++;
  }
  if( type == TWI_MASTER_REQUEST_DATA_COMPLETED )
  {
    if(( messageBuf[1] == 105 ) && (messageBuf[2] == 106 ))
      state ++;
  }
  if( type == TWI_MASTER_GENERAL_CALL_COMPLETED )
  {
    state++;
  }
}


int main()
{
  sei();
  TWI_Master_Initialize(i2c_callback);
  
  while(1)
  {
    switch(state)
    {
      case 0:
        {
          uint8_t buf[3];
          buf[0] = 10; // i2c address
          buf[1] = 100;
          buf[2] = 101;
          TWI_Master_SendData(buf, sizeof(buf));
          state++;
        }
        break;
      case 2:
        {
          uint8_t buf[3];
          buf[0] = 10; // i2c address
          buf[1] = 102;
          buf[2] = 103;
          TWI_Master_RequestData(buf, sizeof(buf), 2);
          state++;
        }
        break;
      case 4:
        TWI_Master_GeneralCall(110);
        state++;
        break;
      case 6:
        cli();
        sleep_mode();
        break;
    }
  }
  
  return 0;
}
