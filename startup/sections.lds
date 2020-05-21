OUTPUT_ARCH( "riscv" )

ENTRY( _reset )

MEMORY
{
    FLASH (rx)      : ORIGIN = 0x00100000, LENGTH = 0x400000    /* entire flash, 4 MiB */
    RAM (xrw)       : ORIGIN = 0x00000000, LENGTH = 0x8000      /* 32 kB */
}
SECTIONS
{
    .text :
    {
        _start_text = .;
        KEEP(*(.init))
        . = ORIGIN(FLASH) + 0x100;
        _start_vector = .;
        KEEP(*(.isr_vector))
        *(.text*)
        *(.rodata)         /* .rodata sections (constants, strings, etc.) */
        *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
        *(.srodata)        /* .rodata sections (constants, strings, etc.) */
        *(.srodata*)       /* .rodata* sections (constants, strings, etc.) */
        *(.rodata*)
        . = ALIGN(4);
        _end_text = .;
    } > FLASH

    _stored_data = .;

    .data : AT (_stored_data)
    {
        _start_data = .;
        *(.data)
        *(.data*)
        _global_pointer = . + 0x800;
        *(.sdata*)
        . = ALIGN(4);
        _end_data = .;
    } > RAM

    .bss :
    {
        _start_bss = .;
        *(.bss)
        *(.bss*)
        *(.sbss)
        *(.sbss*)        
        *(COMMON)
        . = ALIGN(4);
        _end_bss = .;
        _end = .;
    } > RAM

}

PROVIDE(_start_heap = _end);
PROVIDE(_end_stack  = ORIGIN(RAM) + (LENGTH(RAM)) );