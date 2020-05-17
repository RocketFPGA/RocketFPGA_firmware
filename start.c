#include <stdint.h>

extern uint32_t  _start_vector;
extern uint32_t  _stored_data;
extern uint32_t  _start_data;
extern uint32_t  _end_data;
extern uint32_t  _start_bss;
extern uint32_t  _end_bss;
extern uint32_t  _end_stack;
extern uint32_t  _start_heap;
extern uint32_t  _global_pointer;
extern void (* const IV[])(void);

extern void main(void);
void __attribute__((section(".init"),naked)) _reset(void) {
    register uint32_t *src, *dst;
    asm volatile("la gp, _global_pointer");
    asm volatile("la sp, _end_stack");

    /* Set up vectored interrupt, with IV starting at offset 0x100 */
    asm volatile("csrw mtvec, %0":: "r"((uint8_t *)(&_start_vector)));

    src = (uint32_t *) &_stored_data;
    dst = (uint32_t *) &_start_data;
    /* Copy the .data section from flash to RAM. */
    while (dst < (uint32_t *)&_end_data) {
        *dst = *src;
        dst++;
        src++;
    }

    /* Initialize the BSS section to 0 */
    dst = &_start_bss;
    while (dst < (uint32_t *)&_end_bss) {
        *dst = 0U;
        dst++;
    }

    /* Run the program! */
    main();
}
