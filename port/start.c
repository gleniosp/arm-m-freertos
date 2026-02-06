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

void start()
{
    BaseType_t xReturn;

    xReturn = xTaskCreate(vTask1, "T1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xReturn = xTaskCreate(vTask2, "T2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    vTaskStartScheduler();
    return;
}