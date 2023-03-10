#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
/*IDF v4.x
#include "esp_system.h"
#include "esp_spi_flash.h"
*/
// IDF v5.x
#include "esp_chip_info.h"
#include "esp_flash.h"
#include <inttypes.h>
#include "esp_mac.h"

    void
    app_main()
{

    printf("Hello world!\n");

    /* Print Chip Information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU cores, WiFi%s%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_BT) ? "|Bluetooth" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "-BLE" : "",
           (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

    // Get Silicon Revision
    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    printf("Silicon Revision v%d.%d, ", major_rev, minor_rev);

    // Get Size Flash
    uint32_t flash_size;
    if (esp_flash_get_size(NULL, &flash_size) != ESP_OK)
    {
        printf("Get flash size failed");
        return;
    }
    printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());
    // Get ESP-IDF Version
    printf("ESP-IDF RTOS SDK %s\n", esp_get_idf_version());

    // Get Base MAC Address from EFUSE BLK0(default option)
    uint8_t chipId[6];
    esp_efuse_mac_get_default(chipId);
    printf("Base MAC Address: %x-%x-%x-%x-%x-%x\n", chipId[0], chipId[1], chipId[2], chipId[3], chipId[4], chipId[5]);

    for (int i = 10; i >= 0; i--)
    {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}