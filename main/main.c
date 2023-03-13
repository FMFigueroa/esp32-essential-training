#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static TaskHandle_t myTask1Handler = NULL;

void sender(void *params) // char * params
{
    while (1)
    {
        /* Set bits in the task's notification value. */
        xTaskNotify(myTask1Handler, (1 << 0), eSetBits); //0001 << 0 = 0001 = 1  (value = 1)
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        xTaskNotify(myTask1Handler, (1 << 1), eSetBits); // 0001 << 1 = 0010 + value = (value = 3)
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        xTaskNotify(myTask1Handler, (1 << 2), eSetBits); // 0001 << 2 = 0100 + value = (value = 7)
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        xTaskNotify(myTask1Handler, (1 << 3), eSetBits); // 0001 << 3 = 1000 + value = (value = 15)
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
void receiver(void *params) // char * params
{
    uint state; // uint32_t * pulNotificationValue
    while (1)
    {
        xTaskNotifyWait(0, 0, &state, portMAX_DELAY);
        printf("received state: %d  times\n", state);
    }
}

void app_main()
{
    xTaskCreate(&receiver, "sender", 2048, "NULL", 2, &myTask1Handler);
    xTaskCreate(&sender, "receiver", 2048, "NULL", 2, NULL);
}