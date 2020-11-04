#ifndef _UART_H
#define _UART_H

#include <stdint.h>
#include <stddef.h>

#define reg_console_uart (*(volatile uint32_t*)0x04000000)
#define read_console_char(c) read_uart_char(UART0, c)

#define UART_BUFFER_SIZE 200

#define UARTS    \
	X(0, UART_BUFFER_SIZE, 0x04000000)    \

typedef enum uart_t
{
#define X(a,b,c) UART ## a,
    UARTS
#undef X
    NUM_UARTS
} uart_t;

typedef struct uart_buffer_t {
    volatile uint32_t* addr;
    uint8_t * buffer;
    size_t write;
    size_t read;
    size_t size; 
} uart_buffer_t;

#endif  // _UART_H