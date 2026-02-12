- `make` to compile the code

- `make qemu` to emulate the compiled code via QEMU

- `make gdb` to use GDB to debug the code running on QEMU

As an alternative, after running QEMU, you can use the VS Code interface to debug with GDB. JUST BEAR IN MIND THAT
VS Code MIGHT NOT BE ABLE TO STEP INTO ASSEMBLY INSTRUCTIONS THAT MIGHT SHOW IN THE FREERTOS KERNEL, LIKE IN THE `prvPortStartFirstTask` FUNCTION (LINE 247, PATH: FreeRTOS-Kernel -> portable -> GCC -> ARM_CM3 -> port.c). IN THIS CASE IT'S BETTER TO USE THE GDB IN THE COMMAND LINE.
Just go to the 'Debug' icon on the left, above the 'Extensions' icon, and run using the pre-configured `GDB` launcher,
which is provided by the `.vscode/launch.json` file.

Both `gdb` via the command line or VS Code will run and stop at the reset handler from the `boot.S` file as the first
instruction.

In VS Code, you can do `Step into` to keep going into the functions, until you reach the FreeRTOS-Kernel functions, like
the `xTaskCreate` one that we're using. If something is failing and you don't know why, you can keep doing `Step into`
until something returns or breaks, so you find the cause.

- `make clean` to clean the generated files
