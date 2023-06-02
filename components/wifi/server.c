#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_http_server.h"
#include "toggleLed.h"
#include "cJSON.h"

#define TAG "SERVER"

// Method GET -> http : // 0.0.0.0/
static esp_err_t on_url_hit(httpd_req_t *req)
{
    ESP_LOGI(TAG, "url %s was hit", req->uri);
    char *message = "hello world!";
    httpd_resp_send(req, message, strlen(message));
    return ESP_OK;
}

// Method GET -> http : // 0.0.0.0/api/temperature
static esp_err_t on_get_temperature(httpd_req_t *req)
{
    ESP_LOGI(TAG, "url %s was hit", req->uri);
    char *message = "{\"temperature\":22}";
    httpd_resp_send(req, message, strlen(message));
    return ESP_OK;
}
// Method POST  ->  http:// 0.0.0.0/api/led
static esp_err_t on_led_set(httpd_req_t *req)
{
    ESP_LOGI(TAG, "url %s was hit", req->uri);
    char buffer[150];
    memset(&buffer, 0, sizeof(buffer));
    httpd_req_recv(req, buffer, req->content_len);
    cJSON *payload = cJSON_Parse(buffer);
    cJSON *is_on_json = cJSON_GetObjectItem(payload, "is_on");
    bool is_on = cJSON_IsTrue(is_on_json);
    cJSON_Delete(payload);
    toggle_led(is_on);
    httpd_resp_set_status(req, "200 OK");
    httpd_resp_send(req, NULL, 0);
    return ESP_OK;
}

void init_Server_APIRest(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    ESP_LOGI(TAG, "starting server");
    if (httpd_start(&server, &config) != ESP_OK)
    {
        ESP_LOGE(TAG, "COULD NOT START SERVER");
    }

    httpd_uri_t first_end_point_config = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = on_url_hit};
    httpd_register_uri_handler(server, &first_end_point_config);

    httpd_uri_t temperature_end_point_config = {
        .uri = "/api/temperature",
        .method = HTTP_GET,
        .handler = on_get_temperature};
    httpd_register_uri_handler(server, &temperature_end_point_config);

    httpd_uri_t led_end_point_config = {
        .uri = "/api/led",
        .method = HTTP_POST,
        .handler = on_led_set};
    httpd_register_uri_handler(server, &led_end_point_config);
}