#include <stdint.h>
#include <stddef.h>

#include "timer.h"
#include "isr.h"

volatile timer_data_t timers[NUM_TIMERS] = {
    #define X(a,b) {.addr = b},
        TIMERS
    #undef X 
};

void set_timer_to(timer_t timer, uint32_t val){
	*timers[timer].addr += val;
}

void timers_beat_callback(void * args){
	timer_data_t * args_timer = (timer_data_t*) args;
	*(args_timer->addr) += args_timer->beat;
	args_timer->callback();
}

void set_timer_beat(timer_t timer, uint32_t beat, timer_beat_callback callback){
	timers[timer].beat = beat;
	timers[timer].callback = callback;
	*(timers[timer].addr) = *(timers[timer].addr);  // Start the beat right now
	set_isr_callback_with_args(timer, timers_beat_callback, (void*) &timers[timer]);
}

void timers_timeout_callback(void * args){
	timer_data_t * args_timer = (timer_data_t*) args;
	unset_isr_callback(args_timer->timer);
	args_timer->callback();
}

void set_timer_timeout(timer_t timer, uint32_t n, timer_beat_callback callback){
	timers[timer].callback = callback;
	timers[timer].timer = timer;
	*(timers[timer].addr) += n;
	set_isr_callback_with_args(timer, timers_timeout_callback, (void*) &timers[timer]);
}

