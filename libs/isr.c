#include <stdint.h>
#include <stddef.h>

#include "isr.h"

isr_struct isr_struct_array[NUM_ISR_SOURCES] = {0};


// Here the callback != NULL can be deleted if we suppose that a unmasked IRQ is always registed
// The current vector variable can be deleted and all the vector will be checked (memory or time?)

void  isr_irq(void){
	uint32_t current_vector = irq_vector;

	for (size_t i = 0; i < NUM_ISR_SOURCES && current_vector; i++, current_vector >>= 1){	
		if ((current_vector & 0b1 ) && isr_struct_array[i].callback != NULL){
			isr_struct_array[i].callback(isr_struct_array[i].args);
		}
	}

	irq_vector = 0;
}