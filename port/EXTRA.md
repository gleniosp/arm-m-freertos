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
- Now, connect the board to the PC via USB and flash `main.bin` into the board with using one of the `stlink-tools`:
  - `sudo st-flash write main.bin 0x08000000` (same address as the FLASH ORIGIN in the linker script)
- To debug with VS Code:
  - Go the local workspace with the `port` and `FreeRTOS-Kernel`
  - Create a `.vscode/launch.json` file and use the same config as in this `arm-m-freertos` repository
  - Start the GDB server with `sudo st-util` (the board should be connected to the PC)
    - The GDB server will print the listening port, something like `*:4242`.
  - Then, go to the `.vscode/launch.json` and change the line `"miDebuggerServerAddress": "127.0.0.1:1234"` to `"miDebuggerServerAddress": "127.0.0.1:4242"`, if that's the case.
    - You may also need to update the `miDebuggerPath` to the proper `gdb` command line tool path in the local machine
  - Then, launch the `GDB` config in the Debug screen (the icon above the extensions icon in the left sidebar of VS Code)
  - Put breakpoints in the `port/start.c` function to see it pausing there, for example, within the tasks `vTask1` and `vTask2`.
- To debug with GDB in the command line:
  - Go the `port` folder in the local workspace
  - Make sure if you have installed the GDB dashboard: https://github.com/cyrus-and/gdb-dashboard. Essentially:
    - `wget -P ~ https://github.com/cyrus-and/gdb-dashboard/raw/master/.gdbinit`
    - `pip install pygments` (for syntax highlighting)
  - Run `gdb main.elf` (main.elf is the one with the debug symbols)
  - In another terminal, run the GDB server with `sudo st-util` (the board should be connected to the PC)
  - The GDB server will print the listening port, something like `*:4242`.
  - Go back to the terminal which has the `gdb` command running and is now accepting commands, and run: `target remote localhost:4242`
  - Then you can use GDB commands normally to debug the code. For example:
    - Put a breakpoint for the symbol `xPortStartScheduler` (which is used in `port/start.c`) with: `b xPortStartScheduler`
    - Then, run you tell GDB that is a remote target and you want to reset it and halt it with: `monitor reset halt`
    - Then, you can continue the program execution with: `c`
    - The program will then stop inside the breakpoint function, `xPortStartScheduler`, and halt.
    - Then, run `c` again to continue
    - GDB will simply continue to execute the program and it'll simply show a mostly empty screen with `Output/messages` title.
      - This is because the program is simply running in its infinite loop now, without breakpoints, and alternating between the `vTask1` and `vTask2` tasks that we created in `port/start.c`.
      - If you press `Ctrl + c` within GDB, it'll pause the program/CPU and you'll see it paused within one these tasks.
      - Send `c` again and the programs goes back running in the infinite loop
      - Press `Ctrl + c` again and it'll pause again...and so on and so forth.
