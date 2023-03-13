#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static TaskHandle_t myTask1Handle = NULL;
static TaskHandle_t myTask2Handle = NULL;

void sender(void *params) // char * params
{
    while (1)
    {
        /* dispatcher receive as param to where will send notification,
        can to be a task or several task the same time. */
        xTaskNotifyGive(myTask2Handle);
        xTaskNotifyGive(myTask2Handle);
        xTaskNotifyGive(myTask2Handle);
        xTaskNotifyGive(myTask2Handle);
        vTaskDelay(5000 / portTICK_PERIOD_MS); //send notification each 5 milisegundo
    }
}

void receiver(void *params) // char * params
{
    int notificationValue;
    while (1)
    {
        notificationValue = ulTaskNotifyTake(pdFALSE, (TickType_t)portMAX_DELAY);
        if (notificationValue > 0) {
                printf("recived notification: %d\r\n", notificationValue);
        }    
    }
}

void app_main()
{
    xTaskCreate(&receiver, "sender", 2048, "NULL", 2, &myTask2Handle);
    xTaskCreate(&sender, "receiver", 2048, "NULL", 2, NULL);
}
