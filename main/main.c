#include <stdio.h>
#include <esp_timer.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

void app_main(void)
{
    //portTICK_PERIOD_MS
    printf("\n");
    printf("Timer miliseconds 100:\n");
    printf("Timer: %lld milisecond\n", esp_timer_get_time() / 1000);
    vTaskDelay(100 / portTICK_PERIOD_MS); // 1 Tick o 10ms
    printf("Timer: %lld milisecond\n", esp_timer_get_time() / 1000);

    // pdMS_TO_TICKS
    printf("\n");
    printf("Timer miliseconds 100:\n");
    printf("Timer: %lld milisecond\n", esp_timer_get_time() / 1000);
    vTaskDelay(pdMS_TO_TICKS(100));
    printf("Timer: %lld milisecond\n", esp_timer_get_time() / 1000);
}