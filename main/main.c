#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static TaskHandle_t myTask1Handler = NULL;

void sender(void *params) // char * params
{
    while (1)
    {
        /* Set bits in the task's notification value. */
        //Part 1
        xTaskNotify(myTask1Handler, (1 << 0), eSetBits);
        xTaskNotify(myTask1Handler, (1 << 1), eSetBits);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        // Part 2
        xTaskNotify(myTask1Handler, (1 << 2), eSetBits); 
        xTaskNotify(myTask1Handler, (1 << 3), eSetBits); 
        vTaskDelay(500 / portTICK_PERIOD_MS);
        // Part 3
        xTaskNotify(myTask1Handler, (15 & 7), eSetBits);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}
void receiver(void *params) // char * params
{
    uint state; // uint32_t * pulNotificationValue
    while (1)
    {
        xTaskNotifyWait(0xffffffff, 0, &state, portMAX_DELAY);
        printf("received state: %d  times\n", state);
    }
}

void app_main()
{
    xTaskCreate(&receiver, "sender", 2048, "NULL", 2, &myTask1Handler);
    xTaskCreate(&sender, "receiver", 2048, "NULL", 2, NULL);
}