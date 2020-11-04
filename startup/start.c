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

// IRQ Handling
void irq_enable(unsigned int irq)
{
	uint32_t mie;
	__asm__ volatile ("csrrs %0, mie, %1\n" : "=r" (mie) : "r" (1 << irq));
}

uint32_t irq_is_enabled(unsigned int irq)
{
	uint32_t mie;
	__asm__ volatile ("csrr %0, mie" : "=r" (mie));
	return !!(mie & (1 << irq));
}

void irq_global_disable(void)
{	
	uint32_t mstatus;
	__asm__ volatile ("csrrc %0, mstatus, %1\n" : "=r" (mstatus) : "r" (1 << 3) );
}

void irq_global_enable(void)
{	
	uint32_t mstatus;
	__asm__ volatile ("csrrs %0, mstatus, %1\n" : "=r" (mstatus) : "r" (1 << 3));
}


// Flash modes
#define reg_spictrl (*(volatile uint32_t*) 0x02000000)

extern uint32_t flashio_worker_begin;
extern uint32_t flashio_worker_end;

void flashio(uint8_t *data, int len, uint8_t wrencmd)
{
	uint32_t func[&flashio_worker_end - &flashio_worker_begin];

	uint32_t *src_ptr = &flashio_worker_begin;
	uint32_t *dst_ptr = func;

	while (src_ptr != &flashio_worker_end)
		*(dst_ptr++) = *(src_ptr++);

	((void(*)(uint8_t*, uint32_t, uint32_t))func)(data, len, wrencmd);
}

void set_flash_qspi_flag()
{
	uint8_t buffer[8];

	// Read Configuration Registers (RDCR1 35h)
	buffer[0] = 0x35;
	buffer[1] = 0x00; // rdata
	flashio(buffer, 2, 0);
	uint8_t sr2 = buffer[1];

	// Write Enable Volatile (50h) + Write Status Register 2 (31h)
	buffer[0] = 0x31;
	buffer[1] = sr2 | 2; // Enable QSPI
	flashio(buffer, 2, 0x50);
}

void set_flash_mode_spi()
{
	reg_spictrl = (reg_spictrl & ~0x007f0000) | 0x00000000;
}

void set_flash_mode_dual()
{
	reg_spictrl = (reg_spictrl & ~0x007f0000) | 0x00400000;
}

void set_flash_mode_quad()
{
	reg_spictrl = (reg_spictrl & ~0x007f0000) | 0x00240000;
}

void set_flash_mode_qddr()
{
	reg_spictrl = (reg_spictrl & ~0x007f0000) | 0x00670000;
}

void enable_flash_crm()
{
	reg_spictrl |= 0x00100000;
}


// Reset entry point

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

    // Enable global interrupts
    irq_enable(7);
	irq_global_enable();

    // Set high speed SPI mode
    // set_flash_mode_dual();
    // set_flash_qspi_flag();
	// set_flash_mode_qddr();
	// enable_flash_crm(); // Not stablecy check in hw v2

    /* Run the program! */
	init_uart();
    main();
}
