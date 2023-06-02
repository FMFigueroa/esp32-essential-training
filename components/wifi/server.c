#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_http_server.h"
#include "toggleLed.h"
#include "pushBtn.h"
#include "cJSON.h"

#define TAG "SERVER"
static httpd_handle_t server = NULL;

static esp_err_t on_url_hit(httpd_req_t *req)
{
    ESP_LOGI(TAG, "url %s was hit", req->uri);
    char *message = "hello world!";
    httpd_resp_send(req, message, strlen(message));
    return ESP_OK;
}

static esp_err_t on_get_temperature(httpd_req_t *req)
{
    ESP_LOGI(TAG, "url %s was hit", req->uri);
    char *message = "{\"temperature\":22}";
    httpd_resp_send(req, message, strlen(message));
    return ESP_OK;
}

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

/********************Web Socket *******************/
// reserva de tamaño para recibir el paquete del cliente
#define WS_MAX_SIZE 1024
static int client_session_id;

esp_err_t send_ws_message(char *message)
{
    if (!client_session_id)
    {
        ESP_LOGE(TAG, "no client_session_id");
        return -1;
    }
    httpd_ws_frame_t ws_message = {
        .final = true,
        .fragmented = false,
        .len = strlen(message),
        .payload = (uint8_t *)message,
        .type = HTTPD_WS_TYPE_TEXT};
    return httpd_ws_send_frame_async(server, client_session_id, &ws_message);
}

static esp_err_t on_web_socket_set(httpd_req_t *req)
{
    client_session_id = httpd_req_to_sockfd(req);
    if (req->method == HTTP_GET)
        return ESP_OK;

    // recepción de paquete del cliente
    httpd_ws_frame_t ws_pkt;
    // limpia paquete estableciendolo en cero
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    // seleccion de puntero para reservar espacio en la memoria
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;
    // reserva espacio de memoria para pkt
    ws_pkt.payload = malloc(WS_MAX_SIZE);
    // recive pak
    httpd_ws_recv_frame(req, &ws_pkt, WS_MAX_SIZE);
    // imprime la longitud del pkt, payload
    printf("ws payload: %.*s\n", ws_pkt.len, ws_pkt.payload);
    // libera memoria
    free(ws_pkt.payload);

    // Crea la respuesta de connecion
    char *response = "connected OK 🚀";
    // type para respuesta
    httpd_ws_frame_t ws_responce = {
        .final = true,
        .fragmented = false,
        .type = HTTPD_WS_TYPE_TEXT,
        .payload = (uint8_t *)response,
        .len = strlen(response)};

    // envia respuesta o tipo de error en el caso.
    return httpd_ws_send_frame(req, &ws_responce);
}

//=======================================    Init Server    ==========================================
void init_Server_APIRest(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    ESP_LOGI(TAG, "starting server");
    if (httpd_start(&server, &config) != ESP_OK)
    {
        ESP_LOGE(TAG, "COULD NOT START SERVER");
    }

    //============    Localhost   =================
    httpd_uri_t first_end_point_config = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = on_url_hit};
    httpd_register_uri_handler(server, &first_end_point_config);

    //============    Temperature   ===============
    httpd_uri_t temperature_end_point_config = {
        .uri = "/api/temperature",
        .method = HTTP_GET,
        .handler = on_get_temperature};
    httpd_register_uri_handler(server, &temperature_end_point_config);

    //============    Toggle Led   ================
    httpd_uri_t led_end_point_config = {
        .uri = "/api/led",
        .method = HTTP_POST,
        .handler = on_led_set};
    httpd_register_uri_handler(server, &led_end_point_config);

    //============    Web Socket   ================
    httpd_uri_t web_socket_config = {
        .uri = "/ws",
        .method = HTTP_GET,
        .handler = on_web_socket_set,
        .is_websocket = true};
    httpd_register_uri_handler(server, &web_socket_config);
}