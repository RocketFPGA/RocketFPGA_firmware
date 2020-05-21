#ifndef _ROCKETFPGA_H
#define _ROCKETFPGA_H

// Global configuration
#define MEM_LEN  32768  // 32 kB
#define CLK_HZ  12000000  // 12 MHz
#define PHASE_SIZE  16  
#define SAMPLING_FREQ  48000  

#define POWTWO(EXP) (1 << (EXP))

// Memory mapped GPIO
#define gpio  (*(volatile uint32_t*) 0x02000000)
#define GPIO_LED  	 0x1
#define GPIO_BUTTON  0x2

// Memory mapped UART
#define reg_uart_data (*(volatile uint32_t*)0x04000000)

// Memory mapped timer
#define timer (*(volatile uint32_t*)0x08000000)

// Memory mapped oscillators
#define osc1 (*(volatile uint32_t*)0x10000000)
#define osc2 (*(volatile uint32_t*)0x10000004)
#define osc3 (*(volatile uint32_t*)0x10000008)
#define osc4 (*(volatile uint32_t*)0x1000000C)

// Memory mapped ADSR
#define adsr (*(volatile uint32_t*)0x10000014)

#define set_attack(x,a) 	x = 	(x & 0x0FFFFFFF) 	| ((a & 0x0000000F) << 28)
#define set_decay(x,a) 		x = 	(x & 0xF0FFFFFF) 	| ((a & 0x0000000F) << 24)
#define set_sustain(x,a) 	x = 	(x & 0xFF0FFFFF) 	| ((a & 0x0000000F) << 20)
#define set_release(x,a) 	x = 	(x & 0xFFF0FFFF) 	| ((a & 0x0000000F) << 16)

// Memory mapped echo
#define echo_offset (*(volatile uint32_t*)0x10000018)

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

#endif  // _ROCKETFPGA_H