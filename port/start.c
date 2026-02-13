#include "FreeRTOS.h"
#include "task.h"

static void vTask1(void *pvParameters)
{
    volatile int a = 0;
    while (1)
    {
        a++;
    };
}

static void vTask2(void *pvParameters)
{
    volatile int b = 0;
    while (1)
    {
        b++;
    };
}

extern uint32_t _sdata;  // Start of .data in RAM
extern uint32_t _edata;  // End of .data in RAM
extern uint32_t _sidata; // Start of .data in Flash
extern uint32_t _sbss;   // Start of .bss
extern uint32_t _ebss;   // End of .bss

void start()
{
    BaseType_t xReturn;

    // Initialize .data section (copy from Flash to RAM)
    uint32_t *src = &_sidata; // Source (Flash)
    uint32_t *dst = &_sdata;  // Destination (RAM)
    while (dst < &_edata)
    {
        *dst++ = *src++; // Copy word and increment pointers
    }

    // Initialize .bss section (zero out)
    dst = &_sbss;
    while (dst < &_ebss)
    {
        *dst++ = 0; // Zero word and increment pointer
    }

    xReturn = xTaskCreate(vTask1, "T1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xReturn = xTaskCreate(vTask2, "T2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    xPortStartScheduler();
    return;
}