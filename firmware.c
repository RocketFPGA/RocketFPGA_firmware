#include <stdint.h>
#include <stdbool.h>

#include <printf.h>
#include <rocketfpga.h>

void enable_echo(){
	toogle_charlie_led(3);
	toogle_charlie_led(2);
	echo_delay = adc_3*8;
	toogle_trigger(ECHO_ENABLE);
}

void callback_set_echo(){
	echo_delay = adc_3*8;
}

void type_callback(){
	static uint8_t type = SINE_TYPE;
	set_osc_type(OSC1_TYPE, type);
	type = (type + 1) % 4;

	for (size_t i = 4; i < 8; i++)
	{
		if ((i-4) > type){
			disable_charlie_led(i);
		}else{
			enable_charlie_led(i);
		}
	}
}

void beat_cb(){
	osc1 = phase_from_freq(50+adc_1/8);

	toogle_trigger(ADRS_TRIGGER);
	gpio = ~gpio;
	toogle_charlie_led(11);
}

void manual_trigger(){
	echo_delay = adc_3<<2;
	osc2 = phase_from_freq(adc_2/8);

	enable_trigger(ADRS_TRIGGER);
	set_osc_type(OSC1_TYPE, SQUARE_TYPE);

	while (get_gpio(BUTTON_2))
	{	
		osc1 = phase_from_freq(50+adc_1);
		// 	osc1--;
		// if 
	}
	set_osc_type(OSC1_TYPE, SINE_TYPE);

	disable_trigger(ADRS_TRIGGER);
}


void main(){
	print_header();
	
	set_osc_type(OSC1_TYPE, SINE_TYPE);
	set_osc_type(OSC2_TYPE, SINE_TYPE);
	set_osc_type(OSC3_TYPE, SINE_TYPE);
	set_osc_type(OSC4_TYPE, SINE_TYPE);

	set_matrix(MATRIX_OSC_2, MATRIX_MOD_IN_CARR)
	set_matrix(MATRIX_OSC_1, MATRIX_MOD_IN_MOD)
	set_matrix(MATRIX_OSC_1, MATRIX_MULT_IN_1);
	set_matrix(MATRIX_ENVELOPE_OUT, MATRIX_MULT_IN_2);
	set_matrix(MATRIX_MULT_OUT, MATRIX_ECHO_IN);
	set_matrix(MATRIX_ECHO_OUT, MATRIX_OUTPUT_L);
	set_matrix(MATRIX_MULT_OUT, MATRIX_OUTPUT_R);

	set_all_charlie_leds(0);
	charlie_leds_sequence_1();

	init_codec();

	set_attack(adsr1, 50);
	set_decay(adsr1, 10);
	set_sustain(adsr1, 0.2);
	set_release(adsr1, 200);

	set_isr_callback_with_debounce(BUTTON4_ISR, enable_echo, MS(200));
	set_timer_beat(TIMER1, BPM(240), beat_cb);
	set_isr_callback(BUTTON2_ISR, manual_trigger);
	set_isr_callback_with_debounce(BUTTON3_ISR, callback_set_echo, MS(200));
	set_isr_callback_with_debounce(BUTTON1_ISR, type_callback, MS(200));

	type_callback();

	while(1){
		uint32_t aux = 10+adc_2>>1;
		timers[TIMER1].beat = BPM(aux);
		set_echo_drygain(adc_4<<3);
		set_echo_wetgain(adc_5<<3);
		printf("%08X\n",echo_gains);

		set_release(adsr1, adc_6+20);

	}	
}
