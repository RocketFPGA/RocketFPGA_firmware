#include <stdint.h>
#include <stddef.h>

#include "timer.h"
#include "isr.h"

volatile timer timers[NUM_TIMERS] = {
    #define X(a,b) {.addr = b},
        TIMERS
    #undef X 
};

void set_timer_to(size_t timer, uint32_t val){
	*timers[timer].addr += val;
}

void timers_beat_callback(void * args){
	timer * args_timer = (timer*) args;
	*(args_timer->addr) += args_timer->beat;
	args_timer->callback();
}

void set_timer_beat(size_t timer, uint32_t beat, timer_beat_callback callback){
	timers[timer].beat = beat;
	timers[timer].callback = callback;
	*(timers[timer].addr) += *(timers[timer].addr);  // Start the beat right now
	set_isr_callback_with_args(timer, timers_beat_callback, (void*) &timers[timer]);
}

void timers_timeout_callback(void * args){
	timer * args_timer = (timer*) args;
	unset_isr_callback(args_timer->timer);
	args_timer->callback();
}

void set_timer_timeout(size_t timer, uint32_t n, timer_beat_callback callback){
	timers[timer].callback = callback;
	timers[timer].timer = timer;
	*(timers[timer].addr) += n;
	set_isr_callback_with_args(timer, timers_timeout_callback, (void*) &timers[timer]);
}

