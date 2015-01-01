#ifndef __I2C_MASTER_LIB_H__
#define __I2C_MASTER_LIB_H__

/****************************************************************************
  TWI parameter definitions
****************************************************************************/
#ifndef TWI_BUFFER_SIZE
#define TWI_BUFFER_SIZE 8   // Set this to the largest message size that will be sent including address byte.
#endif

#ifndef TWI_FREQ
#define TWI_FREQ    100000
#endif

#define TWI_TWBR  ((F_CPU / TWI_FREQ) - 16) / 2 // TWI Bit rate Register setting.
                                        // Se Application note for detailed 
/****************************************************************************
  Global definitions
****************************************************************************/

#define TWI_STATUS_MASK                     3

#define TWI_COMPLETED                       1
#define TWI_FAILED                          2

#define TWI_NONE                            0
#define TWI_MASTER_SEND_DATA                4
#define TWI_MASTER_REQUEST_DATA             8
#define TWI_MASTER_GENERAL_CALL             12
#define TWI_MASTER_SEND_DATA_COMPLETED      (TWI_MASTER_SEND_DATA | TWI_COMPLETED)
#define TWI_MASTER_SEND_DATA_FAILED         (TWI_MASTER_SEND_DATA | TWI_FAILED)
#define TWI_MASTER_REQUEST_DATA_COMPLETED   (TWI_MASTER_REQUEST_DATA | TWI_COMPLETED)
#define TWI_MASTER_REQUEST_DATA_FAILED      (TWI_MASTER_REQUEST_DATA | TWI_FAILED)
#define TWI_MASTER_GENERAL_CALL_COMPLETED   (TWI_MASTER_GENERAL_CALL | TWI_COMPLETED)
#define TWI_MASTER_GENERAL_CALL_FAILED      (TWI_MASTER_GENERAL_CALL | TWI_FAILED)

typedef void (*TWI_callBack)(unsigned char type, unsigned char * buf, unsigned char len); 

/****************************************************************************
  Function definitions
****************************************************************************/
void TWI_Master_Initialize(TWI_callBack);
void TWI_Master_GeneralCall( unsigned char );
void TWI_Master_SendData( unsigned char * , unsigned char );
void TWI_Master_RequestData( unsigned char * , unsigned char, unsigned char );
unsigned char TWI_Master_IsBusy();

/****************************************************************************
  Bit and byte definitions
****************************************************************************/
#define TWI_READ_BIT  0       // Bit position for R/W bit in "address byte".
#define TWI_ADR_BITS  1       // Bit position for LSB of the slave address bits in the init byte.

#define TRUE          1
#define FALSE         0

/****************************************************************************
  TWI State codes
****************************************************************************/
// General TWI Master staus codes                      
#define TWI_START                  0x08  // START has been transmitted  
#define TWI_REP_START              0x10  // Repeated START has been transmitted
#define TWI_ARB_LOST               0x38  // Arbitration lost

// TWI Master Transmitter staus codes                      
#define TWI_MTX_ADR_ACK            0x18  // SLA+W has been tramsmitted and ACK received
#define TWI_MTX_ADR_NACK           0x20  // SLA+W has been tramsmitted and NACK received 
#define TWI_MTX_DATA_ACK           0x28  // Data byte has been tramsmitted and ACK received
#define TWI_MTX_DATA_NACK          0x30  // Data byte has been tramsmitted and NACK received 

// TWI Master Receiver staus codes  
#define TWI_MRX_ADR_ACK            0x40  // SLA+R has been tramsmitted and ACK received
#define TWI_MRX_ADR_NACK           0x48  // SLA+R has been tramsmitted and NACK received
#define TWI_MRX_DATA_ACK           0x50  // Data byte has been received and ACK tramsmitted
#define TWI_MRX_DATA_NACK          0x58  // Data byte has been received and NACK tramsmitted

// TWI Slave Transmitter staus codes
#define TWI_STX_ADR_ACK            0xA8  // Own SLA+R has been received; ACK has been returned
#define TWI_STX_ADR_ACK_M_ARB_LOST 0xB0  // Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been returned
#define TWI_STX_DATA_ACK           0xB8  // Data byte in TWDR has been transmitted; ACK has been received
#define TWI_STX_DATA_NACK          0xC0  // Data byte in TWDR has been transmitted; NOT ACK has been received
#define TWI_STX_DATA_ACK_LAST_BYTE 0xC8  // Last data byte in TWDR has been transmitted (TWEA = “0”); ACK has been received

// TWI Slave Receiver staus codes
#define TWI_SRX_ADR_ACK            0x60  // Own SLA+W has been received ACK has been returned
#define TWI_SRX_ADR_ACK_M_ARB_LOST 0x68  // Arbitration lost in SLA+R/W as Master; own SLA+W has been received; ACK has been returned
#define TWI_SRX_GEN_ACK            0x70  // General call address has been received; ACK has been returned
#define TWI_SRX_GEN_ACK_M_ARB_LOST 0x78  // Arbitration lost in SLA+R/W as Master; General call address has been received; ACK has been returned
#define TWI_SRX_ADR_DATA_ACK       0x80  // Previously addressed with own SLA+W; data has been received; ACK has been returned
#define TWI_SRX_ADR_DATA_NACK      0x88  // Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
#define TWI_SRX_GEN_DATA_ACK       0x90  // Previously addressed with general call; data has been received; ACK has been returned
#define TWI_SRX_GEN_DATA_NACK      0x98  // Previously addressed with general call; data has been received; NOT ACK has been returned
#define TWI_SRX_STOP_RESTART       0xA0  // A STOP condition or repeated START condition has been received while still addressed as Slave

// TWI Miscellaneous status codes
#define TWI_NO_STATE               0xF8  // No relevant state information available
#define TWI_BUS_ERROR              0x00  // Bus error due to an illegal START or STOP condition

#endif
