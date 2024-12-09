global _start
extern kmain

section .text
bits 32

; Multiboot2 header (for GRUB)
align 8
    dd 0xE85250D6      ; Multiboot2 magic
    dd 0                ; Architecture (0 = 32-bit)
    dd header_end - header_start
    dd -(0xE85250D6 + 0 + (header_end - header_start))

header_start:
; End tag
align 8
    dw 0
    dw 8
    dd 0
header_end:

; _start is called by GRUB in 32-bit protected mode
_start:
    ; Enable long mode via EFER MSR
    mov ecx, 0xC0000080    ; EFER
    rdmsr
    or eax, 0x100          ; Set Long Mode Enable (LME)
    wrmsr

    ; Enable PAE in CR4
    mov eax, cr4
    or eax, 0x20
    mov cr4, eax

    ; Set up the page table
    mov eax, pml4          ; Load the address of PML4
    mov cr3, eax           ; Load it into CR3

    ; Enable paging in CR0
    mov eax, cr0
    or eax, 0x80000001     ; Set PG (paging) and PE (protected mode) bits
    mov cr0, eax

    ; Load the GDT
    lgdt [gdt_descriptor]

    ; Far jump to 64-bit code
    push 0x08              ; Code segment selector
    push _start_64         ; 64-bit entry point
    retf                   ; Far return to switch to 64-bit mode

bits 64
_start_64:
    ; Set up segment registers
    mov ax, 0x10           ; Data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Call the kernel main function
    call kmain

halt:
    hlt
    jmp halt

; GDT definition: null, code, and data segments
gdt:
    dq 0
    dq 0x00AF9A000000FFFF ; Code segment
    dq 0x00AF92000000FFFF ; Data segment
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt - 1
    dq gdt

section .data
align 4096
pml4:
    dq pml4e + 0x3
    times 511 dq 0

pml4e:
    dq pdpte + 0x3
    times 511 dq 0

pdpte:
    dq pde + 0x3
    times 511 dq 0

pde:
    dq 0x83
    times 511 dq 0
