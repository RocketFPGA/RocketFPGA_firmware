// #include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "printf.h"

#define gpio  (*(volatile uint32_t*) 0x02000000)
#define GPIO_LED  	 0x1
#define GPIO_BUTTON  0x2

#define reg_uart_data (*(volatile uint32_t*)0x04000000)
#define timer (*(volatile uint32_t*)0x08000000)

#define osc1 (*(volatile uint32_t*)0x10000000)
#define osc2 (*(volatile uint32_t*)0x10000004)
#define osc3 (*(volatile uint32_t*)0x10000008)
#define osc4 (*(volatile uint32_t*)0x1000000C)

#define adsr (*(volatile uint32_t*)0x10000014)
#define echo_offset (*(volatile uint32_t*)0x10000018)

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

#define set_attack(x,a) 	x = 	(x & 0x0FFFFFFF) 	| ((a & 0x0000000F) << 28)
#define set_decay(x,a) 		x = 	(x & 0xF0FFFFFF) 	| ((a & 0x0000000F) << 24)
#define set_sustain(x,a) 	x = 	(x & 0xFF0FFFFF) 	| ((a & 0x0000000F) << 20)
#define set_release(x,a) 	x = 	(x & 0xFFF0FFFF) 	| ((a & 0x0000000F) << 16)

#define PHASE_SIZE  16  
#define POWTWO(EXP) (1 << (EXP))
#define SAMPLING_FREQ  48000  

#define MEM_LEN  32768  // 32 kB
#define CLK_S  12000000  // 12 MHz

uint32_t phase_from_freq(uint32_t freq){
	return freq * POWTWO(PHASE_SIZE) / SAMPLING_FREQ;  
}

uint32_t freq_from_phase(uint32_t phase){
	return phase *  SAMPLING_FREQ / POWTWO(PHASE_SIZE);
}

uint32_t osc1_main = 70;
uint32_t osc2_main = 130;
uint8_t counter = 0; 

uint32_t bpm = CLK_S; 

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

void _putchar(char c){
	reg_uart_data = c;
}

void print_osc(){
	printf("OSC1: %d ",freq_from_phase(osc1));
	printf("OSC2: %d ",freq_from_phase(osc2));
	printf("OSC3: %d ",freq_from_phase(osc3));
	printf("OSC4: %d\n",freq_from_phase(osc4));
}

void print(const char *p){
	while (*p)
		_putchar(*(p++));
}

void delay(uint32_t d){
	for (uint32_t i = 0; i < d; i++){
		__asm__ ("nop");
	}
}

char getchar(){
	int32_t c = -1;
	while (c == -1) {
		c = reg_uart_data;
	}
	return c;
}

void irq_enable(unsigned int irq)
{
	uint32_t mie;

	__asm__ volatile ("csrrs %0, mie, %1\n"
			  : "=r" (mie)
			  : "r" (1 << irq));
}

uint32_t irq_is_enabled(unsigned int irq)
{
	uint32_t mie;

	__asm__ volatile ("csrr %0, mie" : "=r" (mie));

	return !!(mie & (1 << irq));
}

void irq_global_disable(void)
{	
	uint32_t mstatus;

	__asm__ volatile ("csrrc %0, mstatus, %1"
			  : "=r" (mstatus)
			  : "r" (1 << 3)
			  : "memory");

	__asm__ volatile ("csrwi mie, 0\n"
			  "csrwi mip, 0\n");
}

void irq_global_enable(void)
{	
	uint32_t mstatus;

	__asm__ volatile ("csrrs %0, mstatus, %1\n"
			: "=r" (mstatus)
			: "r" (1 << 3));

}

void main(){
	irq_enable(7);
	timer = timer + CLK_S;
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

	print("\n");
	print("  _____            _        _   ______ _____   _____          \n");
	print(" |  __ \\          | |      | | |  ____|  __ \\ / ____|   /\\    \n");
	print(" | |__) |___   ___| | _____| |_| |__  | |__) | |  __   /  \\   \n");
	print(" |  _  // _ \\ / __| |/ / _ \\ __|  __| |  ___/| | |_ | / /\\ \\  \n");
	print(" | | \\ \\ (_) | (__|   <  __/ |_| |    | |    | |__| |/ ____ \\ \n");
	print(" |_|  \\_\\___/ \\___|_|\\_\\___|\\__|_|    |_|     \\_____/_/    \\_\\\n");
	print("\n");
	print(" The Hardcore Audio Processor\n");
	print("\n");
	print(">");

	while (1){
		char read = getchar();
		if (read == '\r'){
			read = getchar();
		}
		
		if (read == 'a'){
			print("\nThe Hardcore Audio Processor");
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

		print("\n>");

		

	}
}
