#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "driver/gpio.h"

#define PIN 2

void init_led(void)
{
    esp_rom_gpio_pad_select_gpio(PIN);
    gpio_set_direction(PIN, GPIO_MODE_OUTPUT);
}

void toggle_led(bool is_on)
{
    gpio_set_level(PIN, is_on);
}