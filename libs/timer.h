#ifndef _TIMER_H
#define _TIMER_H

#include <stdint.h>
#include <stddef.h>

#define CLK_HZ  12000000  // 12 MHz

// N = CLK_HZ*60/BPM = 720000000/BPM
#define BPM(x) (uint32_t)(720000000.0/(float)x)
#define US(x) (uint32_t)(x*12)
#define MS(x) (uint32_t)(x*12000)
#define S(x) (uint32_t)(x*12000000)

typedef void (*timer_beat_callback)();

#define TIMERS            \
	X(0, 0x08000000)    \
	X(1, 0x08000004)    \

typedef enum timer_t
{
#define X(a,b) TIMER ## a,
    TIMERS
#undef X
    NUM_TIMERS
} timer_t;

typedef struct timer_data_t{
    volatile uint32_t* addr;
    timer_beat_callback callback;
    union
    {
        uint32_t beat;
        uint32_t timer;
    };
} timer_data_t;

extern volatile timer_data_t timers[NUM_TIMERS];

void set_timer_to(timer_t timer, uint32_t val);

void set_timer_beat(timer_t timer, uint32_t beat, timer_beat_callback callback);
#define clear_timer_beat unset_isr_callback
void set_timer_timeout(timer_t timer, uint32_t ms, timer_beat_callback callback);
#define clear_timer_timeout unset_isr_callback

#endif  // _TIMER_H