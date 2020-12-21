#ifndef _COMPRESSOR_LEDS_H
#define _COMPRESSOR_LEDS_H

// Memory mapped compressor
#define compressor_params1 (*(volatile uint32_t*)0x10000040)
#define compressor_params2 (*(volatile uint32_t*)0x10000044)

#define set_compressor_thr(a) 	   	 	compressor_params1 = (compressor_params1 & 0x0000FFFF) 	| ((a & 0x0000FFFF) << 16)
#define set_compressor_ratio(a) 		compressor_params1 = (compressor_params1 & 0xFFFF0000) 	| (a & 0x0000FFFF)
#define set_compressor_attack(a) 	    compressor_params2 = (compressor_params2 & 0x0000FFFF) 	| ((a & 0x0000FFFF) << 16)
#define set_compressor_release(a) 		compressor_params2 = (compressor_params2 & 0xFFFF0000) 	| (a & 0x0000FFFF)

#endif  // _COMPRESSOR_LEDS_H