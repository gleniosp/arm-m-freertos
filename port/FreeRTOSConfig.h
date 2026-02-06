#ifndef _CONFIG_H__
#define _CONFIG_H__

#define configTICK_TYPE_WIDTH_IN_BITS  TICK_TYPE_WIDTH_32_BITS
#define configMAX_SYSCALL_INTERRUPT_PRIORITY  1
/*
    0x100 = 256, meaning that the stack size will have 256 words (where each word equals 32 bits in Cortex M3).
    256 words of 32 bits each (4 bytes) = 1024 bytes = 1 KB
*/
#define configMINIMAL_STACK_SIZE  0x100
#define configMAX_PRIORITIES  5
#define configUSE_PREEMPTION  1
#define configUSE_IDLE_HOOK  0
#define configUSE_TICK_HOOK  0
#define configTOTAL_HEAP_SIZE  0x1000 // total = 4 KB
#define configCPU_CLOCK_HZ  8000000 // 8 MHz
#define configTICK_RATE_HZ  10 // 10 Hz

#endif