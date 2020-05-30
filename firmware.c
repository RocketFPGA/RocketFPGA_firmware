#include <stdint.h>
#include <stdbool.h>

#include "libs/printf.h"
#include "libs/rocketfpga.h"

uint32_t osc1_main = 70;
uint32_t osc2_main = 130;
uint8_t counter = 0; 

uint32_t bpm = CLK_HZ/50;
uint32_t filter_out = 0;
float beta = 0.11; 
int32_t irq_divider=0;
uint8_t i = 1;

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


	if (irq_divider % 5 == 0)
	{
		osc1 = phase_from_freq(filter_out*i);

		i = ((i +1) % 3) + 1;

		gpio = !(gpio & GPIO_LED);
		toogle_trigger(ADRS_TRIGGER);
	}
	irq_divider++;

	timer = timer + bpm;
}

void print_osc(){
	printf("OSC1: %d ",freq_from_phase(osc1));
	printf("OSC2: %d ",freq_from_phase(osc2));
	printf("OSC3: %d ",freq_from_phase(osc3));
	printf("OSC4: %d\n",freq_from_phase(osc4));
}

void main(){
	irq_enable(7);
	timer = timer + bpm;
	irq_global_enable();
	
	set_matrix(MATRIX_OSC_1, MATRIX_MULT_IN_1);
	set_matrix(MATRIX_ENVELOPE_OUT,MATRIX_MULT_IN_2);

	set_matrix(MATRIX_MULT_OUT, MATRIX_ECHO_IN);

	set_matrix(MATRIX_ECHO_OUT, MATRIX_OUTPUT_L);
	set_matrix(MATRIX_ECHO_OUT, MATRIX_OUTPUT_R);

	echo_delay = 6000;
	enable_trigger(ECHO_ENABLE);

	set_attack(adsr1, 1000);
	set_decay(adsr1, 100);
	set_sustain(adsr1, 1);
	set_release(adsr1, 1000);

	while (1)
	{
		filter_out = (uint32_t)((float)filter_out - (beta * ((float)filter_out - adc_1)));
	}
	

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
