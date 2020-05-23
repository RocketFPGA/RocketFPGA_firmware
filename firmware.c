#include <stdint.h>
#include <stdbool.h>

#include "libs/printf.h"
#include "libs/rocketfpga.h"

uint32_t osc1_main = 70;
uint32_t osc2_main = 130;
uint8_t counter = 0; 

uint32_t bpm = CLK_HZ; 

void  isr_irq(void){
	// if ( (gpio & GPIO_LED))
	// {
	// 		timer = timer + bpm/6;
	// }else{
	// 	timer = timer + bpm/8;
	// 	osc1 = phase_from_freq(osc1_main * (counter + 1));
	// 	osc2 = phase_from_freq(osc2_main * (counter + 1));
	// 	counter = (counter + 1) % 4;
	// }
	// gpio = !(gpio & GPIO_LED);

	// toogle_trigger(ADRS_TRIGGER);
}

void print_osc(){
	printf("OSC1: %d ",freq_from_phase(osc1));
	printf("OSC2: %d ",freq_from_phase(osc2));
	printf("OSC3: %d ",freq_from_phase(osc3));
	printf("OSC4: %d\n",freq_from_phase(osc4));
}

void main(){
	// irq_enable(7);
	// timer = timer + CLK_HZ;
	// irq_global_enable();
	
	uint8_t attack = 1;
	uint8_t decay = 8;
	uint8_t sustain = 3;
	uint8_t release = 8;
	
	set_attack(adsr,attack);
	set_decay(adsr,decay);	
	set_sustain(adsr,sustain);
	set_release(adsr,release);

	// set_matrix(MATRIX_OSC_2, MATRIX_MIXER4_IN_1);
	// set_matrix(MATRIX_OSC_1, MATRIX_MIXER4_IN_2);
	// set_matrix(MATRIX_OSC_1, MATRIX_ECHO_IN);
	// set_matrix(MATRIX_ECHO_OUT, MATRIX_MIXER4_IN_3);
	// set_matrix(MATRIX_MIXER4_OUT, MATRIX_OUTPUT_L);
	// set_matrix(MATRIX_MIXER4_OUT, MATRIX_OUTPUT_R);

	// set_matrix(MATRIX_OSC_1, MATRIX_MIXER4_IN_1);
	// set_matrix(MATRIX_OSC_2, MATRIX_MIXER4_IN_2);
	// set_matrix(MATRIX_OSC_3, MATRIX_MIXER4_IN_3);
	// set_matrix(MATRIX_OSC_4, MATRIX_MIXER4_IN_4);
	// set_matrix(MATRIX_MIXER4_OUT, MATRIX_OUTPUT_L);
	// set_matrix(MATRIX_MIXER4_OUT, MATRIX_OUTPUT_R);

	while (getchar() != '\n');

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


	while (1){
		read_rocket_command();
		printf(">");
	}
}
