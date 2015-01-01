#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define I2C_ADDRESS 17

// I2C szolga státuszkódok

#define TWI_SRX_ADR_ACK            0x60
#define TWI_SRX_ADR_ACK_M_ARB_LOST 0x68
#define TWI_SRX_GEN_ACK            0x70
#define TWI_SRX_GEN_ACK_M_ARB_LOST 0x78
#define TWI_SRX_ADR_DATA_ACK       0x80
#define TWI_SRX_ADR_DATA_NACK      0x88
#define TWI_SRX_GEN_DATA_ACK       0x90
#define TWI_SRX_GEN_DATA_NACK      0x98
#define TWI_SRX_STOP_RESTART       0xA0

#define TWI_STX_ADR_ACK            0xA8
#define TWI_STX_ADR_ACK_M_ARB_LOST 0xB0
#define TWI_STX_DATA_ACK           0xB8
#define TWI_STX_DATA_NACK          0xC0
#define TWI_STX_DATA_ACK_LAST_BYTE 0xC8

#define TWI_NO_STATE               0xF8
#define TWI_BUS_ERROR              0x00

int main()
{
	TWSR = 0;
	TWBR = 0;
	TWAR = I2C_ADDRESS << 1;

	TWCR = (1 << TWEN) | // TWI engedélyezése
	       (1 << TWIE) | (1 << TWINT) |
	       (1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
	       (0 << TWWC);

	uint8_t i2c_response = 'I';

	do
	{
		while (!(TWCR & (1 << TWINT)));

		uint8_t twsr = TWSR & 0xF8;
		uint8_t twcr = (1 << TWEN) |    // TWI újra engedélyezése
				(0 << TWIE) | (1 << TWINT) |
				(1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
				(0 << TWWC);

		if (twsr == TWI_STX_ADR_ACK || twsr == TWI_STX_DATA_ACK)
		{
			TWDR = i2c_response;
		}
		else if (twsr == TWI_SRX_ADR_ACK)
		{
			/* ne csinálj semmit */;
		}
		else if (twsr == TWI_SRX_ADR_DATA_ACK)
		{
			uint8_t twdr = TWDR;
			
			if( twdr == 'Q' )
			{
				cli();
				sleep_mode();
			}
			
			i2c_response = twdr + 1;
		}
		else if (twsr == TWI_SRX_STOP_RESTART)
		{
			/* ne csinálj semmit */;
		}
		else if (twsr == TWI_STX_DATA_ACK_LAST_BYTE || twsr == TWI_SRX_ADR_DATA_NACK || twsr == TWI_BUS_ERROR)
		{
			twcr = (1 << TWSTO) | (1 << TWINT); // helyreállítás BUS_ERROR után
		}

		TWCR = twcr;
	} while (1);
}
