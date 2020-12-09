#ifndef _ISR_H
#define _ISR_H

#define ISR_SOURCES \
	X(TIMER0) 		\
	X(TIMER1) 		\
	X(UART0) 		\
	X(BUTTON1) 		\
	X(BUTTON2) 		\
	X(BUTTON3) 		\
	X(BUTTON4) 		\

typedef void (*isr_handler)(void *);

enum
{
#define X(a) a ## _ISR,
    ISR_SOURCES
#undef X
    NUM_ISR_SOURCES
};

typedef struct isr_struct_t {
    isr_handler callback;
    void * args;
	uint32_t debounce;
	uint32_t next_time;
} isr_struct_t;

extern isr_struct_t isr_struct_array[NUM_ISR_SOURCES];

#define irq_vector (*(volatile uint32_t*) 0x09000000)
#define irq_vector_mask (*(volatile uint32_t*) 0x09000004)

#define set_isr_callback(isr, func) do {isr_struct_array[isr].callback = func; isr_struct_array[isr].debounce = 0; irq_vector_mask |= 1UL << isr;} while(0);
#define set_isr_callback_with_args(isr, func, args_in) do {isr_struct_array[isr].callback = func; isr_struct_array[isr].debounce = 0; isr_struct_array[isr].args = args_in; irq_vector_mask |= 1UL << isr;} while(0);
#define set_isr_callback_with_debounce(isr, func, deb) do {isr_struct_array[isr].callback = func; isr_struct_array[isr].debounce = deb; isr_struct_array[isr].next_time = *timers[0].addr + deb; irq_vector_mask |= 1UL << isr;} while(0);

#define unset_isr_callback(isr) do {isr_struct_array[isr].callback = NULL; irq_vector_mask &= ~(1UL << isr);} while(0);


#endif  // _ISR_H