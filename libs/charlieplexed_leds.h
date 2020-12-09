#ifndef _CHARLIPLEXED_LEDS_H
#define _CHARLIPLEXED_LEDS_H

#include <rocketfpga.h>

#define charlie  (*(volatile uint32_t*) 0x0A000000)
static uint8_t charlie_mask[12] = {2, 11, 9, 5, 4, 0, 10, 3, 7, 6, 1, 8};

#define set_all_charlie_leds(v)	    charlie = (v) ? 0xFFFF : 0

#define enable_charlie_led(n)		charlie |= 1UL << charlie_mask[n]
#define disable_charlie_led(n)		charlie &= ~(1UL << charlie_mask[n])
#define toogle_charlie_led(n)		charlie ^= 1UL << charlie_mask[n]
#define get_charlie_led(n)			(charlie >> charlie_mask[n]) & 1U
#define set_charlie_led(n,v)		(v) ? enable_charlie_led(n) : disable_charlie_led(n)

static void charlie_leds_sequence_1(){
    for (uint8_t j = 0; j < 2; j++)
	{
		for (uint8_t i = 4; i < 9; i++)
		{
			charlie |= 1 << charlie_mask[i];
			delay(2000);
		}
		for (uint8_t i = 4; i < 9; i++)
		{
			charlie &= ~(1 << charlie_mask[i]);
			delay(2000);
		}
	}
    set_all_charlie_leds(0);
}

#endif  // _CHARLIPLEXED_LEDS_H