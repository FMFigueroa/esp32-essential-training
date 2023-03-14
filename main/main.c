#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

QueueHandle_t myQueue;

void task1(void *params)
{
    char TxBuff[30];
    int count = 0;
    myQueue = xQueueCreate(5, sizeof(TxBuff));
    while (true)
    {
        count++;
        sprintf(TxBuff, "message %d\n", count);

        if (xQueueSend(myQueue, (void *)TxBuff, pdMS_TO_TICKS(1000)))
        {
            printf("sending data: %s", TxBuff);
        }
        else
        {
            printf("failed to add message to queue, buffer full\n");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void task2(void *params)
{
    char RxBuff[30];
    while (true)
    {
        if (myQueue != 0)
        {
            if (xQueueReceive(myQueue, (void *)RxBuff, pdMS_TO_TICKS(5000)))
            {
                printf("received data: %s\r\n", RxBuff);
            }
            else
            {
                printf(" the data was not received in the established time: %s\r\n", RxBuff);
            }
        }
    }
}

void app_main(void)
{
    xTaskCreate(&task1, "receiving data", 2048, NULL, 1, NULL);
    xTaskCreate(&task2, "sending data", 2048, NULL, 1, NULL);
}
