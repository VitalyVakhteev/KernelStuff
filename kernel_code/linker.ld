ENTRY(start)

SECTIONS {
    . = 1M;  /* Start address of the kernel */

    /* Text (code) section */
    .text : ALIGN(4K) {
        *(.multiboot)   /* Multiboot header */
        *(.text)        /* Code */
    } :ro

    /* Read-only data section */
    .rodata : ALIGN(4K) {
        *(.rodata)      /* Read-only data */
        *(.eh_frame)    /* Exception handling frames */
    } :ro

    /* Read-write data section */
    .data : ALIGN(4K) {
        *(.data)        /* Initialized data */
        *(.got)         /* Global Offset Table */
        *(.got.plt)     /* Global Offset Table for PLT */
        *(.igot.plt)    /* Indirect GOT PLT */
    } :rw

    /* Uninitialized data (BSS) section */
    .bss : ALIGN(4K) {
        *(.bss)         /* Uninitialized data */
    } :rw
}

PHDRS {
    ro PT_LOAD FLAGS(5); /* Readable, executable */
    rw PT_LOAD FLAGS(6); /* Readable, writable */
}
