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
	// irq_enable(7);
	// timer = timer + bpm;
	// irq_global_enable();

	osc1 = phase_from_freq(600);
	set_osc_type(OSC1_TYPE, 1);

	set_matrix(MATRIX_OSC_1, MATRIX_BIQUAD_IN);
	
	set_matrix(MATRIX_OSC_1, MATRIX_OUTPUT_L);
	set_matrix(MATRIX_OSC_1, MATRIX_OUTPUT_R);

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



	// while(1){
	// 	osc1 = phase_from_freq((freq_from_phase(osc1)+100)%23000);
	// 	delay(2000);
	// }
	set_biquad(LOWPASS, 12000.0, 1.0, 1.0);
	while (1){
		// printf("%d %f\n",adc_1, 20000.0*(adc_1/3000.0));
		
		osc1 = phase_from_freq((freq_from_phase(osc1)+100)%10000);
		delay(2000);

		// printf("%d\n",timer);
		// read_rocket_command();
		// printf(">");
	}
}
