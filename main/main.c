#include "sdkconfig.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "connect.h"
#include "server.h"
#include "toggleLed.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "wifi_connect";

void initialize_nvs(void)
{
    // initialize the default NVS partition
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void wifi_connect(void *params)
{
    esp_err_t err = wifi_connect_sta(CONFIG_SSID, CONFIG_PASSWORD, 10000);
    if (err)
    {
        ESP_LOGE(TAG, "Failed to connect");
        vTaskDelete(NULL);
    }
    vTaskDelete(NULL);
}

void app_main(void)
{
    initialize_nvs();
    wifi_init();
    init_led();
    xTaskCreate(wifi_connect, "wifi_connect", 1024 * 5, NULL, 5, NULL);
    init_Server_APIRest();
}