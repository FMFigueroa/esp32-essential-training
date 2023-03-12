#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void task1(void *params) // char * params
{

    while (true)
    {
        printf("reading temperature from %s\n", (char *)params);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void task2(void *params) // char * params
{
    while (true)
    {
        printf("reading humidity from %s\n", (char *)params);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    xTaskCreate(&task1, "temperature reading", 2048, "task 1", 2, NULL);
    xTaskCreatePinnedToCore(&task2, "humidity reading", 2048, "task 2", 2, NULL,1);
}
