#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "cJSON.h"
#include "app.h"

#define BTN 0

static SemaphoreHandle_t btn_sem;

// Iterruption RAM Attribute counter
static void IRAM_ATTR on_btn_pushed(void *args)
{
    // semaphore to memory interruption
    xSemaphoreGiveFromISR(btn_sem, NULL);
}

static void btn_push_task(void *params)
{
    while (true)
    {
        xSemaphoreTake(btn_sem, portMAX_DELAY);
        // create payload object
        cJSON *payload = cJSON_CreateObject();
        cJSON_AddBoolToObject(payload, "btn_state", gpio_get_level(BTN));
        char *message = cJSON_Print(payload);
        printf("message: %s\n", message);
        send_ws_message(message);
        cJSON_Delete(payload);
        free(message);
    }
}

void init_btn(void)
{
    // create Task
    xTaskCreate(btn_push_task, "btn_push_task", 2048, NULL, 5, NULL);
    // create Semaphore
    btn_sem = xSemaphoreCreateBinary();
    // Select pin
    esp_rom_gpio_pad_select_gpio(BTN);
    // GPIO set input
    gpio_set_direction(BTN, GPIO_MODE_INPUT);
    // interruption btn any adge pull-down or pull-up
    gpio_set_intr_type(BTN, GPIO_INTR_ANYEDGE);
    // Service memory interruption
    gpio_install_isr_service(0);
    // add  handler function to interruption
    gpio_isr_handler_add(BTN, on_btn_pushed, NULL);
}