    .section .vectors
vector_table:
    /* This might be loaded in the stack pointer as 0xabcc when the CPU boots up (and debugging it with GDB),
    because it should be an address closest to a multiple of 4, as the Cortex M3 used by this project uses
    32-bit (4 bytes) CPU registers for operations. */
    .word 0xabcd
    .word reset_handler
    .zero 400

    .section .text
    .align 1
    .type reset_handler, %function
reset_handler:
    b start
    b .
