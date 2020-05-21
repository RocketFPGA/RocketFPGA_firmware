#include <stdint.h>
#include <stdbool.h>

#include "libs/printf.h"
#include "libs/rocketfpga.h"

uint32_t osc1_main = 70;
uint32_t osc2_main = 130;
uint8_t counter = 0; 

uint32_t bpm = CLK_HZ; 

void  isr_irq(void){
	if ( (gpio & GPIO_LED))
	{
			timer = timer + bpm/6;
	}else{
		timer = timer + bpm/8;
		osc1 = phase_from_freq(osc1_main * (counter + 1));
		osc2 = phase_from_freq(osc2_main * (counter + 1));
		counter = (counter + 1) % 4;
	}
	gpio = !(gpio & GPIO_LED);

	toogle_trigger(ADRS_TRIGGER);
}

void print_osc(){
	printf("OSC1: %d ",freq_from_phase(osc1));
	printf("OSC2: %d ",freq_from_phase(osc2));
	printf("OSC3: %d ",freq_from_phase(osc3));
	printf("OSC4: %d\n",freq_from_phase(osc4));
}

void main(){
	irq_enable(7);
	timer = timer + CLK_HZ;
	irq_global_enable();

	osc3 = phase_from_freq(3500);
	osc4 = phase_from_freq(70);

	echo_offset = 0;
	
	uint8_t attack = 1;
	uint8_t decay = 8;
	uint8_t sustain = 3;
	uint8_t release = 8;
	
	set_attack(adsr,attack);
	set_decay(adsr,decay);	
	set_sustain(adsr,sustain);
	set_release(adsr,release);

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
		char read = getchar();
		if (read == '\r'){
			read = getchar();
		}
		
		if (read == 'a'){
			printf("\nThe Hardcore Audio Processor");
			printf("\nButton: %d", (gpio & GPIO_BUTTON) >> 1);

		// ECHO MANAGEMENT
		}else if (read == 'd'){
			toogle_trigger(ECHO_ENABLE);
			printf("Toggling echo: %d \n", get_trigger(ECHO_ENABLE) );
		}else if (read == 'e'){
			echo_offset = echo_offset + 1000;
			printf("Echo: %d\n", echo_offset);
		}else if (read == 'w'){
			echo_offset = echo_offset - 1000;
			printf("Echo: %d\n", echo_offset);

		// OSCILLATORS
		}else if (read == 'r'){
			toogle_trigger(OSC1_ENABLE);
		}else if (read == 'f'){
			osc1_main = osc1_main + phase_from_freq(100);
			print_osc();
		}else if (read == 'v'){
			osc1_main = osc1_main - phase_from_freq(100);
			print_osc();
		
		}else if (read == 't'){
			toogle_trigger(OSC2_ENABLE);
		}else if (read == 'g'){
			osc2_main = osc2_main + phase_from_freq(100);
			print_osc();
		}else if (read == 'b'){
			osc2_main = osc2_main - phase_from_freq(100);
			print_osc();
		
		}else if (read == 'y'){
			toogle_trigger(OSC3_ENABLE);
		}else if (read == 'h'){
			osc3 = osc3 + phase_from_freq(100);
			print_osc();
		}else if (read == 'n'){
			osc3 = osc3 - phase_from_freq(100);
			print_osc();
		
		}else if (read == 'u'){
			toogle_trigger(OSC4_ENABLE);
		}else if (read == 'j'){
			osc4 = osc4 + phase_from_freq(10);
			print_osc();
		}else if (read == 'm'){
			osc4 = osc4 - phase_from_freq(10);
			print_osc();
		}

		if (read == '3')
			attack++;
		if (read == '4')
			attack--;
		if (read == '5')
			sustain++;
		if (read == '6')
			sustain--;

		if (read == '+'){
			bpm = bpm - 100000;
			printf("BPM: %d\n", bpm);
		}

		if (read == '-'){
			bpm = bpm + 100000;
			printf("BPM: %d\n", bpm);
		}
	
		if (read == 'p'){
			disable_trigger(OSC1_ENABLE);
			disable_trigger(OSC2_ENABLE);
			disable_trigger(OSC3_ENABLE);
			disable_trigger(OSC4_ENABLE);

			attack = 1;
			decay = 8;
			sustain = 3;
			release = 8;
		}

		set_attack(adsr,attack);
		set_decay(adsr,decay);	
		set_sustain(adsr,sustain);
		set_release(adsr,release);

		printf("\n>");
	}
}
