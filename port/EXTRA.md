# QEMU

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

# REAL HARDWARE

To run on real hardware, you'll need:

- The `stlink-tools` on your machine: https://github.com/stlink-org/stlink
- This helpful Pyjamabrah container: https://hub.docker.com/repository/docker/pyjamabrah/sandbox/tags
  - Which you can run and associate with a local workspace on your machine: `docker run -it -v <workspace path in your machine>:/home/pyjamabrah/workspace --name lab pyjamabrah/sandbox:latest`
  - Then clone FreeRTOS in the container workspace: `git clone https://github.com/FreeRTOS/FreeRTOS-Kernel`
    - Remember that you can clone to the local workspace in your machine and it'll me mapped through the Docker volume to the container workspace
  - Then copy the `port` folder (or download it from within the GitHub Codespaces environment) from this `arm-m-freertos` project and put in the container workspace.
  - Then run the following to compile the `port` code:
    - `cd port`
    - Then go to the `map.ld` file (linker script) and update the FLASH ORIGIN to `FLASH : ORIGIN = 0x08000000` instead of `0x0`. The MCU documentation says that `0x0` is mapped internally to the beginning of the Flash memory. However, even though QEMU can properly do this map for us, it doesn't work when flashing directly to the hardware. So, we need to update it. ARM will be able to do the proper mapping internally without issues.
    - Now, compile the code with `make`.
    - Once the code is compiled, `main.elf` will be available.
    - `arm-none-eabi-objcopy -O binary main.elf main.bin`.
    - The binary `main.bin` is now available in the container workspace and consequently in your local workspace as well, in the `port` folder.
- Now, flash `main.bin` into the board with using one of the `stlink-tools`:
  - `sudo st-flash write main.bin 0x08000000` (same address as the FLASH ORIGIN in the linker script)
