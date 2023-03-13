#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static TaskHandle_t myTask1Handler = NULL;

void sender(void *params) // char * params
{
    while (1)
    {

        xTaskNotify(myTask1Handler, (1 << 2), eSetValueWithoutOverwrite); //0001 << 2= 0100 = 4 (Shift Left)
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        xTaskNotify(myTask1Handler, (1 | 4 ), eSetValueWithoutOverwrite); //0001 | 0100 = 0101 = 5 (OR)
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        xTaskNotify(myTask1Handler, (12 >> 1), eSetValueWithoutOverwrite); //1100 >> 1= 0110 = 6 (Shift Right) 
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        xTaskNotify(myTask1Handler, (15 & 7), eSetValueWithoutOverwrite); // 1111 & 0111 = 7 (AND)
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