#include "avr/io.h"              
#include "avr/interrupt.h"
#include "i2c_master_lib.h"

static TWI_callBack  TWI_callBack_Fn;

static unsigned char TWI_buf[ TWI_BUFFER_SIZE ];    // Transceiver buffer
static unsigned char TWI_msgSize;                   // Number of bytes to be transmitted.
static unsigned char TWI_type = TWI_NONE;          // Type of the current TWI call
static unsigned char TWI_expectedSize = 0;          // Number of bytes to be transmitted.

void TWI_Master_Initialize(TWI_callBack callback)
{
  TWI_callBack_Fn = callback;
  TWSR = 0;
  TWBR = TWI_TWBR;                                  // Set bit rate register (Baudrate). Defined in header file.
  TWDR = 0xFF;                                      // Default content = SDA released.
  TWCR = (1<<TWEN)|                                 // Enable TWI-interface and release TWI pins.
         (0<<TWIE)|(0<<TWINT)|                      // Disable Interupt.
         (0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // No Signal requests.
         (0<<TWWC);                                 //
  TWI_type = TWI_NONE;
}    
    
unsigned char TWI_Master_IsBusy()
{
  return ( TWCR & (1<<TWIE) );                  // IF TWI Interrupt is enabled then the Transceiver is busy
}

static void TWI_Master_write( unsigned char type, unsigned char *data, unsigned char len, unsigned char expSize );

void TWI_Master_GeneralCall( unsigned char data )
{
  unsigned char genBuf[2];
  genBuf[0] = 0; // general call
  genBuf[1] = data;
  TWI_Master_write( TWI_MASTER_GENERAL_CALL, genBuf, 2, 0 );
}

void TWI_Master_SendData( unsigned char * data, unsigned char len )
{
  TWI_Master_write( TWI_MASTER_SEND_DATA, data, len, 0 );
}

void TWI_Master_RequestData( unsigned char * data , unsigned char len, unsigned char expSize )
{
  TWI_Master_write( TWI_MASTER_REQUEST_DATA, data, len, expSize );
}

static void TWI_Master_write( unsigned char type, unsigned char *data, unsigned char len, unsigned char expSize )
{
  TWI_msgSize = len;                        // Number of data to transmit.
  TWI_type = type;
  TWI_expectedSize = expSize;

  uint8_t i;
  for ( i = 0; i < len; i++ )
    TWI_buf[ i ] = data[ i ];

  TWI_buf[ 0 ] &= 0xFE;
  
  TWCR = (1<<TWEN)|                             // TWI Interface enabled.
         (1<<TWIE)|(1<<TWINT)|                  // Enable TWI Interupt and clear the flag.
         (0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|       // Initiate a START condition.
         (0<<TWWC);                             //
}


ISR(TWI_vect)
{
  static unsigned char TWI_bufPtr;

  uint8_t twsr = TWSR & 0xF8;
 
  
  switch (twsr)
  {
    case TWI_START:             // START has been transmitted  
    case TWI_REP_START:         // Repeated START has been transmitted
      TWI_bufPtr = 0;                                     // Set buffer pointer to the TWI Address location
    case TWI_MTX_ADR_ACK:       // SLA+W has been tramsmitted and ACK received
    case TWI_MTX_DATA_ACK:      // Data byte has been tramsmitted and ACK received
      if (TWI_bufPtr < TWI_msgSize)
      {
        TWDR = TWI_buf[TWI_bufPtr++];
        TWCR = (1<<TWEN)|                                 // TWI Interface enabled
               (1<<TWIE)|(1<<TWINT)|                      // Enable TWI Interupt and clear the flag to send byte
               (0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           //
               (0<<TWWC);                                 //  
      }else                    // Send STOP after last byte
      {
        TWDR = 0xFF;                                      // Default content = SDA released.
        if(( TWI_type == TWI_MASTER_SEND_DATA ) || ( TWI_type == TWI_MASTER_GENERAL_CALL ) )
        {
           TWCR = (1<<TWEN)|                                 // TWI Interface enabled
           (0<<TWIE)|(1<<TWINT)|                      // Disable TWI Interrupt and clear the flag
           (0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|           // Initiate a STOP condition.
           (0<<TWWC);                                 //
   
           TWI_callBack_Fn(TWI_type | TWI_COMPLETED, 0, 0);
        }
        else // request data (repeated start)
        {
           TWI_buf[ 0 ] |= 0x01;

           TWCR = (1<<TWEN)|                             // TWI Interface enabled.
           (1<<TWIE)|(1<<TWINT)|                  // Enable TWI Interupt and clear the flag.
           (0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|       // Initiate a START condition.
           (0<<TWWC);                             //
        }
      }
      break;
    case TWI_MRX_DATA_ACK:      // Data byte has been received and ACK tramsmitted
    case TWI_MRX_ADR_ACK:       // SLA+R has been tramsmitted and ACK received
      TWI_buf[TWI_bufPtr++] = TWDR;
      if (TWI_bufPtr < (TWI_expectedSize+1) )              // Detect the last byte to NACK it.
      {
        TWCR = (1<<TWEN)|                                 // TWI Interface enabled
               (1<<TWIE)|(1<<TWINT)|                      // Enable TWI Interupt and clear the flag to read next byte
               (1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // Send ACK after reception
               (0<<TWWC);                                 //  
      }else                    // Send NACK after next reception
      {
        TWCR = (1<<TWEN)|                                 // TWI Interface enabled
               (1<<TWIE)|(1<<TWINT)|                      // Enable TWI Interupt and clear the flag to read next byte
               (0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // Send NACK after reception
               (0<<TWWC);                                 // 
      }    
      break; 
    case TWI_MRX_DATA_NACK:     // Data byte has been received and NACK tramsmitted
      TWI_buf[TWI_bufPtr] = TWDR;
      TWCR = (1<<TWEN)|                                 // TWI Interface enabled
             (0<<TWIE)|(1<<TWINT)|                      // Disable TWI Interrupt and clear the flag
             (0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|           // Initiate a STOP condition.
             (0<<TWWC);                                 //
      TWI_callBack_Fn(TWI_type | TWI_COMPLETED, TWI_buf + 1, TWI_bufPtr);
      break;      
    case TWI_ARB_LOST:          // Arbitration lost
      TWCR = (1<<TWEN)|                                 // TWI Interface enabled
             (1<<TWIE)|(1<<TWINT)|                      // Enable TWI Interupt and clear the flag
             (0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|           // Initiate a (RE)START condition.
             (0<<TWWC);                                 //
      break;
    case TWI_MTX_ADR_NACK:      // SLA+W has been tramsmitted and NACK received
    case TWI_MRX_ADR_NACK:      // SLA+R has been tramsmitted and NACK received    
    case TWI_MTX_DATA_NACK:     // Data byte has been tramsmitted and NACK received
//    case TWI_NO_STATE              // No relevant state information available; TWINT = “0”
    case TWI_BUS_ERROR:         // Bus error due to an illegal START or STOP condition
    default:     
                                                        // Reset TWI Interface
      TWCR = (1<<TWEN)|                                 // Enable TWI-interface and release TWI pins
             (0<<TWIE)|(0<<TWINT)|                      // Disable Interupt
             (0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // No Signal requests
             (0<<TWWC);                                 //
      TWI_callBack_Fn(TWI_type | TWI_FAILED, 0, 0);
  }
}
