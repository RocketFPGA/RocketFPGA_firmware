#ifndef _OSCILLATORS_LEDS_H
#define _OSCILLATORS_LEDS_H

// Memory mapped oscillators
#define oscs ((volatile uint32_t*)0x10000000)

#define osc1 (*(volatile uint32_t*)0x10000000)
#define osc2 (*(volatile uint32_t*)0x10000004)
#define osc3 (*(volatile uint32_t*)0x10000008)
#define osc4 (*(volatile uint32_t*)0x1000000C)

#define osc_type (*(volatile uint32_t*)0x10000028)
#define OSC1_TYPE 30
#define OSC2_TYPE 28
#define OSC3_TYPE 26
#define OSC4_TYPE 24

#define SINE_TYPE 0
#define RAMP_TYPE 1
#define SQUARE_TYPE 2
#define TRIANGLE_TYPE 3

#define set_osc_type(x,a) 	osc_type = 	(osc_type & ~(0x3 << x)) | ((a & 0x3) << x)

static uint32_t phase_from_freq(uint32_t freq){
	return freq * POWTWO(PHASE_SIZE) / SAMPLING_FREQ;  
}

static uint32_t freq_from_phase(uint32_t phase){
	return phase *  SAMPLING_FREQ / POWTWO(PHASE_SIZE);
}

#endif  // _OSCILLATORS_LEDS_H