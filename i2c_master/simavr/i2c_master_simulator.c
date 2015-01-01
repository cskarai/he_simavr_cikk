#include <stdio.h>
#include <simavr/sim_avr.h>
#include <simavr/avr_twi.h>

#define FIRMWARE_LOCATION "i2c_master.hex"
#define I2C_ADDRESS 10

avr_irq_t * i2c_input_irq;
uint8_t     state = 0;
uint8_t     rbyte = 105;

void i2c_output_irq(struct avr_irq_t * irq, uint32_t value, void * param)
{
	avr_twi_msg_irq_t v;
	v.u.v = value;
	
	//printf("%d %d %d\n", v.u.twi.msg, v.u.twi.addr, v.u.twi.data);
	if( (v.u.twi.addr & 0xFE) == I2C_ADDRESS )
	{
		if( v.u.twi.msg & TWI_COND_START )
		{
			rbyte = 105;
			printf("START fogadva\n");
			avr_raise_irq(i2c_input_irq, avr_twi_irq_msg(TWI_COND_ACK, I2C_ADDRESS, 1));
			return;
		}
		if( v.u.twi.msg & TWI_COND_WRITE )
		{
			printf("I2C byte fogadva: %d\n", v.u.twi.data);
			avr_raise_irq(i2c_input_irq, avr_twi_irq_msg(TWI_COND_ACK, I2C_ADDRESS, 1));
			return;
		}
		if( v.u.twi.msg & TWI_COND_READ )
		{
			printf("Byte elküldve: %d\n", rbyte);
			avr_raise_irq(i2c_input_irq, avr_twi_irq_msg(TWI_COND_READ, I2C_ADDRESS, rbyte++));
			return;
		}
		if( v.u.twi.msg & TWI_COND_STOP )
		{
			printf("STOP fogadva\n");
			return;
		}
	}
	if( (v.u.twi.addr & 0xFE) == 0 )
	{
		if( v.u.twi.msg & TWI_COND_START )
		{
			printf("Üzenetszóró START fogadva\n");
			avr_raise_irq(i2c_input_irq, avr_twi_irq_msg(TWI_COND_ACK, I2C_ADDRESS, 1));
			return;
		}
		if( v.u.twi.msg & TWI_COND_WRITE )
		{
			printf("Üzenetszóró I2C byte fogadva: %d\n", v.u.twi.data);
			avr_raise_irq(i2c_input_irq, avr_twi_irq_msg(TWI_COND_ACK, I2C_ADDRESS, 1));
			return; 
		}
		if( v.u.twi.msg & TWI_COND_STOP )
		{
			printf("Üzenetszóró STOP fogadva\n");
			return;
		}
	}
}

const char * simulator_get_firmware()
{
	// visszaadja a hex fájl elérhetőségét
	return FIRMWARE_LOCATION;
}

void simulator_setup(avr_t * avr)
{
	/*avr->trace = 1;
	avr->log = LOG_TRACE;*/
	
	i2c_input_irq = avr_io_getirq(avr, AVR_IOCTL_TWI_GETIRQ(0), TWI_IRQ_INPUT);
	avr_irq_register_notify(avr_io_getirq(avr, AVR_IOCTL_TWI_GETIRQ(0), TWI_IRQ_OUTPUT), i2c_output_irq, NULL);
}


void simulator_loop(avr_t * avr)
{
}
