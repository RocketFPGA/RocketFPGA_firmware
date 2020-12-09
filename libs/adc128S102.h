#ifndef _ADC128S102_H
#define _ADC128S102_H

#define adc_array    (*volatile uint32_t*) 0x0B000008)

#define adc_1    (*(volatile uint32_t*) 0x0B000008)
#define adc_2    (*(volatile uint32_t*) 0x0B00000C)
#define adc_3    (*(volatile uint32_t*) 0x0B000018)
#define adc_4    (*(volatile uint32_t*) 0x0B000004)
#define adc_5    (*(volatile uint32_t*) 0x0B000010)
#define adc_6    (*(volatile uint32_t*) 0x0B000014)

#endif  // _ADC128S102_H