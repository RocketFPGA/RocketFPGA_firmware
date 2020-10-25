#ifndef _ISR_H
#define _ISR_H

#define ISR_SOURCES \
	X(TIMER0) 		\
	X(TIMER1) 		\
	X(BUTTON1) 		\

typedef void (*isr_handler)(void *);

enum
{
#define X(a) a ## _ISR,
    ISR_SOURCES
#undef X
    NUM_ISR_SOURCES
};

typedef struct isr_struct {
    isr_handler callback;
    void * args;
} isr_struct;

extern isr_struct isr_struct_array[NUM_ISR_SOURCES];

#define irq_vector (*(volatile uint32_t*) 0x09000000)
#define irq_vector_mask (*(volatile uint32_t*) 0x09000004)

#define set_isr_callback(isr, func) do {isr_struct_array[isr].callback = func; irq_vector_mask |= 1UL << isr;} while(0);
#define set_isr_callback_with_args(isr, func, args_in) do {isr_struct_array[isr].callback = func; isr_struct_array[isr].args = args_in; irq_vector_mask |= 1UL << isr;} while(0);
#define unset_isr_callback(isr) do {isr_struct_array[isr].callback = NULL; irq_vector_mask &= ~(1UL << isr);} while(0);


#endif  // _ISR_H