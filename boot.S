/* Multiboot2 header and 32-bit entry point */
.set MB2_MAGIC,    0xE85250D6
.set MB2_ARCH,     0
.set MB2_LENGTH,   mb2_end - mb2_start
.set MB2_CHECKSUM, -(MB2_MAGIC + MB2_ARCH + MB2_LENGTH)

.section .multiboot
.align 8
mb2_start:
    .long MB2_MAGIC
    .long MB2_ARCH
    .long MB2_LENGTH
    .long MB2_CHECKSUM
    /* Required end tag */
    .short 0
    .short 0
    .long 8
mb2_end:

.section .bss
.align 16
stack_bottom:
    .skip 16384
stack_top:

.section .text
.global _start
.type _start, @function
.extern kernel_main
_start:
    cli
    mov $stack_top, %esp
    push %ebx              /* Multiboot2 info pointer */
    push %eax              /* Multiboot2 magic */
    call kernel_main
.hang:
    cli
    hlt
    jmp .hang
