#ifndef _GPIO_H
#define _GPIO_H

#include <rocketfpga.h>

#define gpio  (*(volatile uint32_t*) 0x05000000)

// Memory mapped GPIO
#define gpio  (*(volatile uint32_t*) 0x05000000)
#define GPIO_LED  	 0x1

#ifdef SHIELD_POTS
#define BUTTON_1  	 0x1
#define BUTTON_2  	 0x2
#define BUTTON_3  	 0x3
#define BUTTON_4  	 0x4
#endif

#define enable_gpio(n)		gpio |= 1UL << n
#define disable_gpio(n)		gpio &= ~(1UL << n)
#define toogle_gpio(n)		gpio ^= 1UL << n
#define get_gpio(n)			(gpio >> n) & 1U
#define set_gpio(n,v)		(v) ? enable_gpio(n) : disable_gpio(n)

#endif  // _GPIO_H