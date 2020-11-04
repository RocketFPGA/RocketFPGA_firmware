#include <uart.h>
#include <isr.h>
#include <stdbool.h>
#include <printf.h>

// Define buffers
#define X(a,b,c) char buffer_uart_ ## a[b];
    UARTS
#undef X


volatile uart_buffer_t uarts[NUM_UARTS] = {
    #define X(a,b,c) {.addr = c, .buffer = buffer_uart_##a, .write = 0, .read = 0, .size = b},
        UARTS
    #undef X 
};

void uart_callback(void * args){
	uart_buffer_t * uart_buffer = (uart_buffer_t*) args;

    uart_buffer->buffer[uart_buffer->write] = *(uart_buffer->addr);

    uart_buffer->write = (uart_buffer->write + 1) % uart_buffer->size;

    if (uart_buffer->write == uart_buffer->read){
        uart_buffer->read = (uart_buffer->read + 1) % uart_buffer->size;
    }
}

size_t available_uart_data(uart_t uart){
    uart_buffer_t * uart_buffer = &uarts[uart];
    if (uart_buffer->read <= uart_buffer->write){
        return uart_buffer->write - uart_buffer->read;
    } else {
        return uart_buffer->write + (uart_buffer->size - uart_buffer->read);
    }
}

bool read_uart_char(uart_t uart, char * c) {
    uart_buffer_t * uart_buffer = &uarts[uart];

    if (available_uart_data(uart))
    {
        *c = uart_buffer->buffer[uart_buffer->read];
        uart_buffer->read = (uart_buffer->read + 1) % uart_buffer->size;

        return true;
    }
    return false;
}

// size_t read_uart(uart_t uart, uint8_t * buf, size_t len){
//     uart_buffer_t uart_buffer = uarts[uart];
//     size_t available = available_uart_data(uart);
//     size_t readed = (available > len) ? len : available;

//     memcpy(buf, uart_buffer->buffer[read])
// }

void init_uart(uart_t uart){
    #define X(a,b,c) set_isr_callback_with_args(UART ## a ## _ISR, uart_callback, (void*) &uarts[UART ## a]);
        UARTS
    #undef X 
}
