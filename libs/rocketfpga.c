#include <stdint.h>
#include <rocketfpga.h>



char getchar(){
	char c;
	while (!read_console_char(&c)){
	}
	return c;
}

void _putchar(char c){
	reg_console_uart = c;
}

void delay(uint32_t d){
	for (uint32_t i = 0; i < d; i++){
		__asm__ ("nop");
	}
}

// ADSR

void set_attack(uint32_t * adsr, uint16_t ms){
    uint16_t val = ADSR_MAX_VALUE/((ms * ADSR_CLK) + 1);
    adsr[0] = 	(adsr[0] & 0x0000FFFF) 	| ((val & 0x0000FFFF) << 16);
}

void set_decay(uint32_t * adsr, uint16_t ms){
    uint16_t val = ADSR_MAX_VALUE/(ms * ADSR_CLK);
    adsr[0] = 	(adsr[0] & 0xFFFF0000) 	| (val & 0x0000FFFF);
}

void set_sustain(uint32_t * adsr, float level){
    level = (level > 0.99) ? 0.99 : level;
    uint16_t val = (uint16_t)(((float)POWTWO(16))*level);
    adsr[1] = 	(adsr[1] & 0x0000FFFF) 	| ((val & 0x0000FFFF) << 16);
}

void set_release(uint32_t * adsr, uint16_t ms){
    uint16_t val = ADSR_MAX_VALUE/(ms * ADSR_CLK);
    adsr[1] = 	(adsr[1] & 0xFFFF0000) 	| (val & 0x0000FFFF);
}