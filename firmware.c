#include <stdint.h>
#include <stdbool.h>

#include "libs/printf.h"
#include "libs/rocketfpga.h"
#include "libs/rocketfpga_codec.h"

#define codec (*(volatile uint32_t*) 0x01000000)


uint32_t bpm = CLK_HZ/4;

void  isr_irq(void){
	gpio = !(gpio & GPIO_LED);
	toogle_trigger(ADRS_TRIGGER);
	timer = timer + bpm;
}

void main(){
	irq_enable(7);
	timer = timer + bpm;
	irq_global_enable();

	osc1 = phase_from_freq(500);
	set_osc_type(OSC1_TYPE, 0);

	set_matrix(MATRIX_OSC_1, MATRIX_MULT_IN_1);
	set_matrix(MATRIX_ENVELOPE_OUT, MATRIX_MULT_IN_2);
	
	set_matrix(MATRIX_MULT_OUT, MATRIX_OUTPUT_R);
	set_matrix(MATRIX_MULT_OUT, MATRIX_OUTPUT_L);

	set_attack(adsr1, 50);
	set_decay(adsr1, 40);
	set_sustain(adsr1, 0.2);
	set_release(adsr1, 40);

	printf("\n");
	printf("  _____            _        _   ______ _____   _____          \n");
	printf(" |  __ \\          | |      | | |  ____|  __ \\ / ____|   /\\    \n");
	printf(" | |__) |___   ___| | _____| |_| |__  | |__) | |  __   /  \\   \n");
	printf(" |  _  // _ \\ / __| |/ / _ \\ __|  __| |  ___/| | |_ | / /\\ \\  \n");
	printf(" | | \\ \\ (_) | (__|   <  __/ |_| |    | |    | |__| |/ ____ \\ \n");
	printf(" |_|  \\_\\___/ \\___|_|\\_\\___|\\__|_|    |_|     \\_____/_/    \\_\\\n");
	printf("\n");
	printf(" The Hardcore Audio Processor\n");
	printf("\n");
	printf(">");
		
	init_codec();

	uint8_t i = 0;
	while (1){
		// printf("%x\n",triggers);
		// read_rocket_command();
		// printf(">");
	}
}
