    .section .vectors
vector_table:
    /*
        __stack is defined in the `map.ld` file. It's the address corresponding to the start of the SRAM for our
        Cortex M3 CPU + 8 KB (total SRAM memory) - 4 (just to be safe and don't write at something that doesn't exist).
    */
    .word __stack
    .word reset_handler
    .zero 400

    .section .text
    .align 1
    .type reset_handler, %function
reset_handler:
    b start
    b .
