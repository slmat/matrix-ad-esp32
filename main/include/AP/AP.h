#ifndef AP_H
#define AP_H

#include <string.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <esp_event.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_http_server.h>
#include <esp_wifi.h>
#include <lwip/err.h>
#include <lwip/sys.h>
#include <esp_mac.h>

#include "../WEB/WEB.h"
#include "AP_secret.h" 

static const char *TAGAP = "AP";

static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void wifi_init_softap(void);

#endif