#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <simavr/sim_avr.h>
#include <simavr/sim_hex.h>
#include <sys/time.h>
#include <unistd.h>

#define SYNCRONIZATION_CYCLES 10000

extern const char * simulator_get_firmware();
extern void         simulator_setup(avr_t * avr);
extern void         simulator_loop(avr_t * avr);

int main(int argc, char * argv[])
{
	char *simulator_base_dir = (char *)malloc(strlen(argv[0])+1);
	strcpy(simulator_base_dir, argv[0]);
	uint8_t p = strlen(argv[0]);
	while( p && ((argv[0][p] != '/') && (argv[0][p] != '\\') && (argv[0][p] != ':' )))
		simulator_base_dir[p--] = 0;
	chdir(simulator_base_dir);
	free(simulator_base_dir);
	
	// létrehozzuk az avr szimulátort
	avr_t * avr = avr_make_mcu_by_name("atmega328p");
	if (!avr) {
		fprintf(stderr, "Nem sikerült az AVR magot létrehozni!\n");
		return -1;
	}
	printf("Az %s mag elkészült!\n", avr->mmcu );

	avr_init(avr);
	avr->frequency = 1000000;

	uint32_t base, size;
	uint8_t * boot = read_ihex_file(simulator_get_firmware(), &size, &base);
	
	memcpy(avr->flash + base, boot, size);
	free(boot);
	avr->pc = base;
	avr->codeend = base + size;
	printf("A program feltöltődött a 0x%04x-0x%04x címre, (%d byte)!\n", base, base+size, size );
	
	// meghívjuk a setup függvényt
	simulator_setup(avr);
	
	// itt megpróbáljuk szinkronban tartani a simavr-t
	// minden 10.000-ik feldolgozott órajel után sleep-pel lassítjuk,
	// ugyanis a szimulátor olyan gyorsan megy, ahogy csak bír
	struct timeval tv;
	gettimeofday(&tv, NULL);
	uint32_t last_cycle = 0;
	long int start_sec = tv.tv_sec;
	long int last_timestamp = tv.tv_usec;
	
	long int sync_length = 1000000UL * SYNCRONIZATION_CYCLES / avr->frequency;
	long int expected_timestamp = sync_length + last_timestamp;
	
	for (;;) {
		int state = avr_run(avr);
		if ( state == cpu_Done || state == cpu_Crashed)
			break;
		
		simulator_loop(avr);
		
		if( avr->cycle - last_cycle > SYNCRONIZATION_CYCLES )
		{
			gettimeofday(&tv, NULL);
			long int this_timestamp = (tv.tv_sec - start_sec) * 1000000 + tv.tv_usec;
			
			if( this_timestamp < expected_timestamp )
				// amennyivel gyorsabbak vagyunk, azt usleep-eljük
				usleep( expected_timestamp - this_timestamp );
			
			expected_timestamp += sync_length;
			last_cycle += SYNCRONIZATION_CYCLES;
		}
	}
	avr_terminate(avr);
	printf("A program kilépett!\n");
	return 0;
}
