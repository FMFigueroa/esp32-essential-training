#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// #include "esp_system.h" //For IDF 4.x use this lib
#include "esp_random.h" //esp_system.h" For IDF 5.x use this lib
#include "math.h"

#define TAG "DICE"

int dice_role()
{
    int random = esp_random();
    int positiveNumber = abs(random);          // the function abs() convert from number random to positive number random.!
    int diceNumber = (positiveNumber % 6) + 1; // 0 - 5 = 0... > 6 = 1 ;
    return diceNumber;
}

void app_main(void)
{
    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        // ESP_LOGI(TAG, "random number %d", esp_random()); // For IDF 4.x use this sintaxis
        // ESP_LOGI(TAG, "random number %ld", esp_random());
        ESP_LOGI(TAG, "random number %d", dice_role());
    }
}
