#include <stdint.h>
#include <stdbool.h>

#include <printf.h>
#include <rocketfpga.h>


void trigger_adsr(){
	enable_trigger(ADRS_TRIGGER);
	enable_gpio(ONBOARD_LED);
	while (get_gpio(BUTTON_1)){}
	disable_trigger(ADRS_TRIGGER);
	disable_gpio(ONBOARD_LED);
}


void enable_echo(){
	toogle_charlie_led(3);
	echo_delay = adc_3<<2;;
	toogle_trigger(ECHO_ENABLE);
}

void toogle_input(){
	uint64_t m = save_matrix();
	set_matrix(MATRIX_LINE_L, MATRIX_OUTPUT_L);
	set_matrix(MATRIX_LINE_R, MATRIX_OUTPUT_R);
	enable_charlie_led(0);
	while (get_gpio(BUTTON_1)){}
	disable_charlie_led(0);
	load_matrix(m);
}

void toogle_filter(){
	uint64_t m = save_matrix();
	float freq = (float) map_adc(adc_6, 100, 10000);
	printf("%f\n", freq);
	set_biquad(LOWPASS, freq, 1.0, 10.0);
	set_matrix(MATRIX_BIQUAD_OUT, MATRIX_OUTPUT_L);
	set_matrix(MATRIX_BIQUAD_OUT, MATRIX_OUTPUT_R);
	enable_charlie_led(1);
	while (get_gpio(BUTTON_2)){}
	disable_charlie_led(1);
	load_matrix(m);
}

void only_effects(){
	uint64_t m = save_matrix();
	float freq = (float) map_adc(adc_6, 100, 19000);
	printf("%f\n", freq);
	set_biquad(LOWPASS, freq, 0.7, 0.5);
	set_matrix(MATRIX_BIQUAD_OUT, MATRIX_OUTPUT_R);
	set_matrix(MATRIX_ECHO_OUT, MATRIX_OUTPUT_L);
	enable_charlie_led(2);
	while (get_gpio(BUTTON_3)){}
	disable_charlie_led(2);
	load_matrix(m);
}


void callback_set_echo(){
	echo_delay = adc_3<<2;
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
	static bool activate = true;
	
	if(activate){
		chord_t c = generate_major_chord(NOTE_E, 4, rand()%3);
		// switch (rand()%4)
		// {
		// 	case 0: c= generate_major_chord(rand()%12, 3+ rand()%2, CHORD_FIRST_INVERSION); break;
		// 	case 1: c= generate_min7_chord(rand()%12, 3+ rand()%2, CHORD_FIRST_INVERSION); break;
		// 	case 2: c= generate_minor_chord(rand()%12, 3+ rand()%2, CHORD_FIRST_INVERSION); break;
		// 	case 3: c= generate_dominant7_chord(rand()%12, 3+ rand()%2, CHORD_FIRST_INVERSION); break;
		// }

		osc_1 =  c.notes[0];
		osc_2 =  c.notes[1];
		osc_3 =  c.notes[2];
		if (c.len == 4){
			osc_4 =  c.notes[3];
		} else {
			osc_4 = 0;
		}
	}

	activate = !activate;

	toogle_trigger(ADRS_TRIGGER);
	toogle_gpio(ONBOARD_LED);
}


void main(){
	print_header();
	
	set_osc_type(OSC1_TYPE, SINE_TYPE);
	set_osc_type(OSC2_TYPE, SINE_TYPE);
	set_osc_type(OSC3_TYPE, SINE_TYPE);
	set_osc_type(OSC4_TYPE, SINE_TYPE);

	set_matrix(MATRIX_OSC_1, MATRIX_COMPR_IN);
	

	set_matrix(MATRIX_COMPR_OUT, MATRIX_OUTPUT_R);
	set_matrix(MATRIX_COMPR_OUT, MATRIX_OUTPUT_L);

	// charlie_leds_sequence_1();
	// set_all_charlie_leds(1);

	init_codec();

	// set_isr_callback_with_debounce(BUTTON4_ISR, enable_echo, MS(200));
	// set_timer_beat(TIMER1, BPM(240), beat_cb);
	// set_isr_callback_with_debounce(BUTTON2_ISR, beat_cb, MS(200));
	// set_isr_callback_with_debounce(BUTTON3_ISR, callback_set_echo, MS(200));
	// set_isr_callback_with_debounce(BUTTON3_ISR, only_effects, MS(200));
	// set_isr_callback_with_debounce(BUTTON2_ISR, toogle_filter, MS(200));
	// set_isr_callback_with_debounce(BUTTON1_ISR, toogle_input, MS(200));
    // set_isr_callback_with_debounce(BUTTON1_ISR, trigger_adsr, MS(50));
	
	// set_attack(adsr1, 256);
	// set_release(adsr1, 256);
	// set_decay(adsr1, 512);
	// set_sustain(adsr1, 0x7FFF);

	osc_1 =  phase_from_freq(map_adc(adc_1,50,10000));

	while(1){
		// set_codec_line_volume(map_adc(adc_1, 0, 31));
		// float freq = (float) map_adc(adc_6, 100, 19000);
		// printf("%f\n", freq);
		// timers[TIMER1].beat = BPM(map_adc(adc_3, 120, 2000));
		if (get_gpio(BUTTON_1))
		{
			osc_1 = phase_from_freq(map_adc(adc_1,50,10000));
		}
		
		set_compressor_thr(map_adc(adc_2, 1, 0x7FFF));
		set_compressor_ratio(map_adc(adc_3, 1, 0x7FFF));
		set_compressor_attack(map_adc(adc_6, 10 ,700));
		set_compressor_release(map_adc(adc_5, 10 ,700));

		// set_echo_drygain(map_adc(adc_2, 0, 0x8000));
		// set_echo_wetgain(map_adc(adc_1, 0, 0x8000));
	}	
}
