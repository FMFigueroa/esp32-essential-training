#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#define TAG "NVS"

typedef struct cat_struct
{
    char name[20];
    int age;
    int id;
} Cat;

void app_main()
{ // timer delay 1ms
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    // inicializa la flash
    ESP_ERROR_CHECK(nvs_flash_init_partition("MyNvs"));

    // crea el identificador
    nvs_handle_t handle;

    // reservamos el NAMESPACE dentro de NVS
    ESP_ERROR_CHECK(nvs_open_from_partition("MyNvs", "cat_store", NVS_READWRITE, &handle));

    // stats
    nvs_stats_t nvsStats;
    nvs_get_stats("MyNvs", &nvsStats);
    ESP_LOGI(TAG, "used: %d, free: %d, total: %d, namespace count: %d", nvsStats.used_entries, nvsStats.free_entries, nvsStats.total_entries, nvsStats.namespace_count);

    // espacio reservado para guardar objeto 15 bytes
    char catKey[15];

    Cat cat;
    size_t catSize = sizeof(Cat);
    for (int i = 0; i < 5; i++)
    {
        sprintf(catKey, "cat_%d", i);
        // revisa si hay data en la key-value especificada y lo amacena en resultado
        esp_err_t result = nvs_get_blob(handle, catKey, (void *)&cat, &catSize);

        // verifica si hay un valor almacenado resultado
        switch (result)
        {
        case ESP_ERR_NOT_FOUND:
        case ESP_ERR_NVS_NOT_FOUND:
            ESP_LOGE(TAG, "Value not set yet");
            break;
        case ESP_OK:
            ESP_LOGI(TAG, "cat name: %s, age %d, id %d", cat.name, cat.age, cat.id);
            break;
        default:
            ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(result));
            break;
        }
    }
    // incremente el valor almacenado
    for (int i = 0; i < 5; i++)
    {
        sprintf(catKey, "cat_%d", i);
        Cat newCat;
        sprintf(newCat.name, "Mr cat %d", i);
        newCat.age = i + 2;
        newCat.id = i;
        // asigna el nuevo valor
        ESP_ERROR_CHECK(nvs_set_blob(handle, catKey, (void *)&newCat, sizeof(Cat)));
        // realiza un commit
        ESP_ERROR_CHECK(nvs_commit(handle));
    }
    // cierra el stores nvs y libera los recursos.
    nvs_close(handle);
}
