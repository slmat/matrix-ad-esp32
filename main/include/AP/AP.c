#include "AP.h"

static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) 
    {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAGAP, "sta("MACSTR")join, AID=%d", MAC2STR(event->mac), event->aid);
        //if(server == NULL)
        //{ 
            ESP_LOGI(TAGAP, "starting webserver...");
            run_webserver();
        //}
    } 
    else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) 
    {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAGAP, "sta("MACSTR")leave, AID=%d, reason=%d", MAC2STR(event->mac), event->aid, event->reason);

        //wifi_sta_list_t stations_connected;                
        //if(esp_wifi_ap_get_sta_list(&stations_connected) == ESP_OK)
        //{
            //if(stations_connected.num == 0) 
            //{
                ESP_LOGI(TAGAP, "stopping webserver...");
                unrun_webserver();
            //}
       // }
    }
}

void wifi_init_softap(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = SSID,
            .ssid_len = strlen(SSID),
            .channel = CHANNEL,
            .password = PASSWORD,
            .max_connection = MAX_CONNECTIONS,
            .authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    if (strlen(PASSWORD) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAGAP, "SSID:%s pass:%s channel:%d", SSID, PASSWORD, CHANNEL);
}