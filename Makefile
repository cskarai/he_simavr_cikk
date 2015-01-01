all : blink_avr blink_sim push_button_avr push_button_sim uart_avr uart_sim \
      spi_slave_avr spi_slave_sim spi_master_avr spi_master_sim \
      adc_avr adc_sim i2c_master_avr i2c_master_sim hw_simulator_avr hw_simulator_sim

clean : clean_blink_avr clean_blink_sim clean_push_button_avr clean_push_button_sim clean_uart_avr clean_uart_sim \
        clean_spi_slave_avr clean_spi_slave_sim clean_spi_master_avr clean_spi_master_sim \
        clean_adc_avr clean_i2c_master_avr clean_i2c_master_sim clean_adc_sim clean_hw_simulator_avr clean_hw_simulator_sim
        
run : run_blink run_push_button run_uart run_spi_slave run_spi_master run_adc run_i2c_master

blink_avr :
	cd blink; $(MAKE)

blink_sim :
	cd blink/simavr; $(MAKE)

push_button_avr :
	cd push_button; $(MAKE)

push_button_sim :
	cd push_button/simavr; $(MAKE)

uart_avr :
	cd uart; $(MAKE)

uart_sim :
	cd uart/simavr; $(MAKE)

spi_master_avr :
	cd spi_master; $(MAKE)

spi_master_sim :
	cd spi_master/simavr; $(MAKE)
	
spi_slave_avr :
	cd spi_slave; $(MAKE)

spi_slave_sim :
	cd spi_slave/simavr; $(MAKE)

adc_avr :
	cd adc; $(MAKE)

adc_sim :
	cd adc/simavr; $(MAKE)

i2c_master_avr :
	cd i2c_master; $(MAKE)

i2c_master_sim :
	cd i2c_master/simavr; $(MAKE)

hw_simulator_avr :
	cd hw_simulator; $(MAKE)

hw_simulator_sim :
	cd hw_simulator/simavr; $(MAKE)


clean_blink_avr :
	cd blink; $(MAKE) clean

clean_blink_sim :
	cd blink/simavr; $(MAKE) clean

clean_push_button_avr :
	cd push_button; $(MAKE) clean

clean_push_button_sim :
	cd push_button/simavr; $(MAKE) clean

clean_uart_avr :
	cd uart; $(MAKE) clean

clean_uart_sim :
	cd uart/simavr; $(MAKE) clean

clean_spi_master_avr :
	cd spi_master; $(MAKE) clean

clean_spi_master_sim :
	cd spi_master/simavr; $(MAKE) clean
	
clean_spi_slave_avr :
	cd spi_slave; $(MAKE) clean

clean_spi_slave_sim :
	cd spi_slave/simavr; $(MAKE) clean

clean_adc_avr :
	cd adc; $(MAKE) clean

clean_adc_sim :
	cd adc/simavr; $(MAKE) clean

clean_i2c_master_avr :
	cd i2c_master; $(MAKE) clean

clean_i2c_master_sim :
	cd i2c_master/simavr; $(MAKE) clean

clean_hw_simulator_avr :
	cd hw_simulator; $(MAKE) clean

clean_hw_simulator_sim :
	cd hw_simulator/simavr; $(MAKE) clean

	
run_blink :
	cd blink/simavr; $(MAKE) run

run_push_button :
	cd push_button/simavr; $(MAKE) run

run_uart :
	cd uart/simavr; $(MAKE) run

run_spi_slave :
	cd spi_slave/simavr; $(MAKE) run

run_spi_master :
	cd spi_master/simavr; $(MAKE) run

run_adc :
	cd adc/simavr; $(MAKE) run

run_i2c_master :
	cd i2c_master/simavr; $(MAKE) run
