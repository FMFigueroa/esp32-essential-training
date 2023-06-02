#include <stdio.h>      //Para definir la entrada del directorio.
#include "esp_spiffs.h" //Administrador de archivos
#include <dirent.h>     //directorio de entrada
#include <sys/unistd.h> // standar Unix,Tiene algunas definiciones de tipos particulares que podríamos estar utilizando.
#include <sys/stat.h>   //estadísticas sobre un archivo en particular.
#include "esp_log.h"
#include "cJSON.h"

#define TAG "SPIFFS"

void app_main(void)
{
    esp_vfs_spiffs_conf_t config = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true};
    esp_vfs_spiffs_register(&config);

    // Abre el directorio
    DIR *dir = opendir("/spiffs");

    // crea un puntero al directorio de entrada que se le asigne
    struct dirent *entry;

    // Lee el directorio al que apunta
    while ((entry = readdir(dir)) != NULL)
    {
        // fullPath almacena la ruta completa del directorio
        char fullPath[300];
        // imprime la ruta completa del directoria al que apunta
        sprintf(fullPath, "/spiffs/%s", entry->d_name);

        // crea la estructra para explorar estadísticas del directorio
        struct stat entryStat;

        if (stat(fullPath, &entryStat) == -1)
        {
            ESP_LOGE(TAG, "error getting stats for %s", fullPath);
        }
        else
        { // imprime cada archivo con fullpath y tamaño en la memoria NVS
            ESP_LOGW(TAG, "full path = %s, file size = %ld", fullPath, entryStat.st_size);
        }
    }

    // informacion de los archivos administrados por SPIFFS
    size_t total = 0, used = 0;
    esp_spiffs_info(NULL, &total, &used);
    // Imprime total y usado de memoria utilizada y reservada en NVS Prtition Storage.
    ESP_LOGI(TAG, "total = %d, used = %d", total, used);

    // Abre el archivo en la ruta especificada
    FILE *file = fopen("/spiffs/data.json", "r");
    if (file == NULL)
    {
        ESP_LOGE(TAG, "could not open file");
        return;
    }
    else
    {
        // array de 255 bytes para almacenar la linea leída.
        char line[255];
        // fgets lee linea por linea
        while (fgets(line, sizeof(line), file) != NULL)
        {
            printf(line);
        }

        fclose(file);
    }
    // desmonta la particion en la flash llamada storage y desabilita SPIFFS
    esp_vfs_spiffs_unregister(NULL);
}