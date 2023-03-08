#include <stdio.h>
#include "esp_log.h"

void app_main(void)

{
    // Especific log info
    esp_log_level_set("INFO", ESP_LOG_INFO);
    ESP_LOGE("INFO", "This is an error");
    ESP_LOGW("INFO", "This is an warning");
    ESP_LOGI("INFO", "This is an info ");
    // notice that these logs are not printed
    ESP_LOGD("INFO", "This is a debug ");
    ESP_LOGV("INFO", "This is an vervose ");

    // Logging library:
    int number = 0;
    // log errror
    ESP_LOGE("LOG", "This is an error %d", number++);
    // log warning
    ESP_LOGW("LOG", "This is an warning %d", number++);
    // log information
    ESP_LOGI("LOG", "This is an info %d", number++);
    // log debug
    ESP_LOGD("LOG", "This is a debug %d", number++);
    // log verbose
    ESP_LOGV("LOG", "This is an vervose %d", number++);
    // if you want see the debug and vervose logs,
    // you must active it from menuconfig, run <idf.py menuconfig>
    // this is the route: Component config -> Log output-> Default set log info (X)...change it by vervose and save it.

    // For more about Logging library visit the official document:
    // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/log.html
}