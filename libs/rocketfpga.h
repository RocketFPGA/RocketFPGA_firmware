#ifndef _ROCKETFPGA_H
#define _ROCKETFPGA_H

#include <timer.h>
#include <uart.h>
#include <rocketfpga_codec.h>
#include <isr.h>


#define RAMFUNC __attribute__ ((section (".ramfunctions")))

// Global configuration
#define MEM_LEN  32768  // 32 kB
#define PHASE_SIZE  16  
#define SAMPLING_FREQ  48000  

// Utils
#define POWTWO(EXP) (1 << (EXP))
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923

// Memory mapped GPIO
#define gpio  (*(volatile uint32_t*) 0x05000000)
#define GPIO_LED  	 0x1
#define GPIO_BUTTON  0x2

// Memory mapped oscillators
static uint32_t * oscs = 0x10000000;
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

// Memory mapped echo
#define echo_delay (*(volatile uint32_t*)0x10000014)

// Memory mapped triggers and enablers
#define triggers (*(volatile uint32_t*)0x10000010)
#define ADRS_TRIGGER 0
#define OSC1_ENABLE 1
#define OSC2_ENABLE 2
#define OSC3_ENABLE 3
#define OSC4_ENABLE 4
#define ECHO_ENABLE 5

#define enable_trigger(n)		triggers |= 1UL << n
#define disable_trigger(n)		triggers &= ~(1UL << n)
#define toogle_trigger(n)		triggers ^= 1UL << n
#define get_trigger(n)			(triggers >> n) & 1U
#define set_trigger(n,v)		(v) ? enable_trigger(n) : disable_trigger(n)

// Memory mapped matrix
#define matrix_1 (*(volatile uint32_t*)0x10000018)
#define matrix_2 (*(volatile uint32_t*)0x1000001C)
#define MATRIX_IN 12
#define MATRIX_OUT 12

#define MATRIX_MIXER4_IN_1 1
#define MATRIX_MIXER4_IN_2 2
#define MATRIX_MIXER4_IN_3 3
#define MATRIX_MIXER4_IN_4 4
#define MATRIX_MULT_IN_1   5
#define MATRIX_MULT_IN_2   6
#define MATRIX_ECHO_IN     7
#define MATRIX_OUTPUT_R    8
#define MATRIX_OUTPUT_L    9
#define MATRIX_MOD_IN_1    10
#define MATRIX_MOD_IN_2    11
#define MATRIX_BIQUAD_IN   12

static const char *matrix_out_names[MATRIX_OUT+1] = {"", "MATRIX_MIXER4_IN_1",
                                          "MATRIX_MIXER4_IN_2",
                                          "MATRIX_MIXER4_IN_3",
                                          "MATRIX_MIXER4_IN_4",
                                          "MATRIX_MULT_IN_1",
                                          "MATRIX_MULT_IN_2",
                                          "MATRIX_ECHO_IN",
                                          "MATRIX_OUTPUT_R",
                                          "MATRIX_OUTPUT_L",
                                          "MATRIX_MOD_IN_1",
                                          "MATRIX_MOD_IN_2",
                                          "MATRIX_BIQUAD_IN"};

#define MATRIX_NONE         0
#define MATRIX_OSC_1        1
#define MATRIX_OSC_2        2
#define MATRIX_OSC_3        3
#define MATRIX_OSC_4        4
#define MATRIX_MIXER4_OUT   5
#define MATRIX_MULT_OUT     6
#define MATRIX_ECHO_OUT     7
#define MATRIX_ENVELOPE_OUT 8
#define MATRIX_MOD_OUT      9
#define MATRIX_LINE_MIC     10
#define MATRIX_BIQUAD_OUT   11

static const char *matrix_in_names[MATRIX_IN] = {"MATRIX_NONE",
                                                    "MATRIX_OSC_1",
                                                    "MATRIX_OSC_2",
                                                    "MATRIX_OSC_3",
                                                    "MATRIX_OSC_4",
                                                    "MATRIX_MIXER4_OUT",
                                                    "MATRIX_MULT_OUT",
                                                    "MATRIX_ECHO_OUT",
                                                    "MATRIX_ENVELOPE_OUT",
                                                    "MATRIX_MOD_OUT",
                                                    "MATRIX_LINE_MIC",
                                                    "MATRIX_BIQUAD_OUT"};



#define set_matrix_1(in, out) 	 matrix_1 = ((matrix_1 & ~(0xF << (4*(out-1)))) | ((in & 0xF) << (4*(out-1))))
#define set_matrix_2(in, out) 	 matrix_2 = ((matrix_2 & ~(0xF << (4*(out-1)))) | ((in & 0xF) << (4*(out-1))))
#define set_matrix(in, out) 	 if(out <= 8){set_matrix_1(in, out);}else{set_matrix_2(in, out-8);};

// Memory mapped ADSR: assumes release and decay from max value
static uint32_t * adsr1 = 0x10000020;

#define ADSR_ACC_LEN 26
#define ADSR_MAX_VALUE POWTWO(ADSR_ACC_LEN)
#define ADSR_CLK 6000  // 6 kHz = 49.152 MHz / 2^(12+1)

void set_attack(uint32_t * adsr, uint16_t ms);
void set_decay(uint32_t * adsr, uint16_t ms);
void set_sustain(uint32_t * adsr, float level);
void set_release(uint32_t * adsr, uint16_t ms);

// Memory mapped ADC
#define adc_1 (*(volatile uint32_t*)0x10010000)

// Memory mapped modulator
#define modulator1 (*(volatile uint32_t*)0x1000002C)

#define set_modulation_gain(a) 	    modulator1 = 	(modulator1 & 0x0000FFFF) 	| ((a & 0x0000FFFF) << 16)
#define set_modulation_offset(a) 	modulator1 = 	(modulator1 & 0xFFFF0000) 	| (a & 0x0000FFFF)

// Memory mapped modulator
#define modulator1 (*(volatile uint32_t*)0x1000002C)

#define set_modulation_gain(a) 	    modulator1 = 	(modulator1 & 0x0000FFFF) 	| ((a & 0x0000FFFF) << 16)
#define set_modulation_offset(a) 	modulator1 = 	(modulator1 & 0xFFFF0000) 	| (a & 0x0000FFFF)

// Memory mapped biquad filter
#define biquad_reg_1 (*(volatile uint32_t*)0x10000030)
#define biquad_reg_2 (*(volatile uint32_t*)0x10000034)
#define biquad_reg_3 (*(volatile uint32_t*)0x10000038)

typedef enum {
	LOWPASS,
	HIGHPASS,
	BANDPASS,
	NOTCH,
	PEAK,
    LOWSHELF,
    HIGHSHELF
} biquad_type ; 

void set_biquad(biquad_type type, float f, float peak_gain_lin, float Q);

#define set_biquad_a0(v) 	    biquad_reg_1 = 	(biquad_reg_1 & 0x0000FFFF) 	| ((v & 0x0000FFFF) << 16)
#define set_biquad_a1(v) 	    biquad_reg_1 = 	(biquad_reg_1 & 0xFFFF0000) 	| (v & 0x0000FFFF)
#define set_biquad_a2(v) 	    biquad_reg_2 = 	(biquad_reg_2 & 0x0000FFFF) 	| ((v & 0x0000FFFF) << 16)
#define set_biquad_b1(v) 	    biquad_reg_2 = 	(biquad_reg_2 & 0xFFFF0000) 	| (v & 0x0000FFFF)
#define set_biquad_b2(v) 	    biquad_reg_3 = 	(biquad_reg_3 & 0x0000FFFF) 	| ((v & 0x0000FFFF) << 16)

#define get_biquad_a0() 	    (biquad_reg_1 & 0xFFFF0000) >> 16
#define get_biquad_a1() 	    (biquad_reg_1 & 0x0000FFFF) 	
#define get_biquad_a2() 	    (biquad_reg_2 & 0xFFFF0000) >> 16
#define get_biquad_b1() 	    (biquad_reg_2 & 0x0000FFFF) 	
#define get_biquad_b2() 	    (biquad_reg_3 & 0xFFFF0000) >> 16

#endif  // _ROCKETFPGA_H