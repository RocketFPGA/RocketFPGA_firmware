#include <stdint.h>
#include <stdbool.h>

#include "libs/printf.h"
#include "libs/rocketfpga.h"
#include "libs/rocketfpga_codec.h"

#define codec (*(volatile uint32_t*) 0x01000000)


uint32_t osc1_main = 70;
uint32_t osc2_main = 130;
uint8_t counter = 0; 

uint32_t bpm = CLK_HZ/50;
uint32_t filter_out = 0;
float beta = 0.11; 
int32_t irq_divider=0;
uint8_t i = 1;

uint32_t osc_base = 500;
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


	// if (irq_divider % 10 == 0)
	// {
	// 	if(gpio && GPIO_BUTTON){
	// 		osc2 = phase_from_freq(filter_out/10);
	// 	}else{
	// 		osc_base = phase_from_freq(filter_out);
	// 	}
	// 	osc1 = osc_base * i;
	// 	i = ((i +1) % 3) + 1;

	// 	gpio = !(gpio & GPIO_LED);
	// 	toogle_trigger(ADRS_TRIGGER);
	// }
	// irq_divider++;

	timer = timer + bpm;
}

void main(){
	// irq_enable(7);
	// timer = timer + bpm;
	// irq_global_enable();


	osc1 = phase_from_freq(200);
	osc2 = phase_from_freq(200);
	set_matrix(MATRIX_OSC_1, MATRIX_MOD_IN_1);
	set_matrix(MATRIX_OSC_2, MATRIX_MOD_IN_2);
	
	// set_osc_type(OSC1_TYPE, 3);
	set_osc_type(OSC2_TYPE, 3);

	set_modulation_gain(0x3FFF);
	set_modulation_offset(0x3FFF);

	set_matrix(MATRIX_MOD_OUT, MATRIX_ECHO_IN);

	set_matrix(MATRIX_OSC_1, MATRIX_OUTPUT_R);
	set_matrix(MATRIX_OSC_2, MATRIX_OUTPUT_L);

	
	echo_delay = 5000;
	enable_trigger(ECHO_ENABLE);

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

	overdrive = 10000;
	while (1){
		// overdrive = overdrive + 1009;
		delay(1000);
		// read_rocket_command();
		// printf(">");
	}
}
