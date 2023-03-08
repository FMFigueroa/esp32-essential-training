#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define PIN 2

void app_main(void)
{
    // GPIO mode setting
    gpio_set_direction(PIN, GPIO_MODE_OUTPUT);
    uint32_t isOn = 0;
    while (true)
    {
        isOn = !isOn;
        // GPIO toggle setting
        gpio_set_level(PIN, isOn);
        // Delay
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}