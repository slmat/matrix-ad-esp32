#ifndef WEB_H
#define WEB_H

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include <esp_netif.h>
#include <esp_http_server.h>
#include <esp_event.h>
#include <esp_check.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <lwip/err.h>
#include <lwip/sys.h>
#include <esp_eth.h>
#include <esp_mac.h>

#include "html_resps.h"
#include "../global/global.h"
#include "../text_cont/text_cont.h"
#include "../clock/clock.h"

static const char* TAGWEB = "WEB";

static httpd_handle_t server = NULL;

static esp_err_t dummy(httpd_req_t *req);
static esp_err_t index_GET(httpd_req_t *req);
static esp_err_t update_GET(httpd_req_t *req);
static esp_err_t modify_POST(httpd_req_t *req);
esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err);

static const httpd_uri_t index_get = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_GET,
    .user_ctx  = NULL
};

static const httpd_uri_t add_get = {
    .uri       = "/add",
    .method    = HTTP_ANY,
    .handler   = dummy,
    .user_ctx  = "a"
};

static const httpd_uri_t update_get = {
    .uri       = "/update",
    .method    = HTTP_GET,
    .handler   = update_GET,
    .user_ctx  = NULL
};

static const httpd_uri_t modify_post = {
    .uri       = "/modify",
    .method    = HTTP_POST,
    .handler   = modify_POST,
    .user_ctx  = "modify"
};

static const httpd_uri_t remove_post = {
    .uri       = "/remove",
    .method    = HTTP_POST,
    .handler   = dummy,
    .user_ctx  = "r"
};

static const httpd_uri_t update_time = {
    .uri       = "/time",
    .method    = HTTP_POST,
    .handler   = dummy,
    .user_ctx  = "t"
};

static httpd_handle_t start_webserver(void);
static esp_err_t stop_webserver(httpd_handle_t server);
static void disconnect_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data);
static void connect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void run_webserver();
void unrun_webserver();


#endif