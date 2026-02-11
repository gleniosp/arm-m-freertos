    .section .vectors
vector_table:
    /*
        __stack is defined in the `map.ld` file. It's the address corresponding to the start of the SRAM for our
        Cortex M3 CPU + 8 KB (total SRAM memory) - 4 (just to be safe and don't write at something that doesn't exist).
    */
    .word __stack
    .word reset_handler
    /*
        Just adding the default label we define below as the default exception handler for all exceptions in the
        Vector table, except for the reset_handler and the SysTick interrupt.
        The default label is a branch instruction to the current location, which essentially creates a while(true) loop.
        It'll be helpful to debug code and something stops at one of these exceptions, it'll keep spining there and
        we can check the last 4 bits value in the `xpsr` register in GDB to see which exception was triggered.
    */
    .word default
    .word default
    .word default
    .word default
    .word default
    .word default
    .word default
    .word default
    .word default
    .word default
    .word default
    .word default
    .word default
    // the 15th interrupt is for SysTick, in the vector table
    .word xPortSysTickHandler
    .zero 400

    .section .text
    .align 1
    .type reset_handler, %function
reset_handler:
    b start
    b .

    .section .text
    .align 1
    .type default, %function
default:
    b .