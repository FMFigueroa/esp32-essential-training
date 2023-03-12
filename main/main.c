#include <stdio.h>
#include <esp_timer.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/task.h"

/*
The main idea of the tasks,
is that they are executed in parallel with the functions.
*/

void vTask1()
{
    for (size_t i = 0; i < 5; i++)
    {
        printf("vTask1 %d: %lld\n", i, esp_timer_get_time() / 1000);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    vTaskDelete(NULL); // Delete of memory after Task execution
}

void vFunctions()
{
    for (size_t i = 0; i < 5; i++)
    {
        printf("Function %d: %lld\n", i, esp_timer_get_time() / 1000);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vTask2()
{
    for (size_t i = 0; i < 5; i++)
    {
        printf("vTask2 %d: %lld\n", i, esp_timer_get_time() / 1000);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    vTaskDelete(NULL); // Delete of memory after Task execution
}

void app_main(void)
{
    //=================== portTICK_PERIOD_MS ============================

    printf("\nTimer output in miliseconds init: %lld\n", esp_timer_get_time() / 1000);
    // xTaskCreate(vTask1, "name", memory, parameter, priority 1-5, callHandler);
    xTaskCreate(vTask1, "Task1", 2048, NULL, 1, NULL);
    xTaskCreate(vTask2, "Task2", 2048, NULL, 5, NULL);
    vFunctions();
    printf("Timer portTICK_PERIOD_MS 1000 miliseconds:\n");
    printf("Timer: %lld milisecond\n", esp_timer_get_time() / 1000);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    printf("Timer: %lld milisecond\n", esp_timer_get_time() / 1000);

    //=================== pdMS_TO_TICKS ============================

    printf("\n");
    printf("Timer pdMS_TO_TICKS 1000 miliseconds :\n");
    printf("Timer: %lld milisecond\n", esp_timer_get_time() / 1000);
    vTaskDelay(pdMS_TO_TICKS(1000));
    printf("Timer: %lld milisecond\n", esp_timer_get_time() / 1000);
}