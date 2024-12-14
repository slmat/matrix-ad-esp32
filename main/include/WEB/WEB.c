#include "WEB.h"

static esp_err_t index_GET(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_sendstr(req, index_html);

    // if (httpd_req_get_hdr_value_len(req, "Host") == 0) {
    //     ESP_LOGI(TAGWEB, "Request headers lost");
    // }
    return ESP_OK;
}

static esp_err_t dummy(httpd_req_t *req)
{
    const char *context = (const char*)req->user_ctx;
    //ESP_LOGI(TAGWEB, "%s", context);

    if(context[0] == 'a')
    {
        text_create_empty();
    }
    else if (context[0] == 'r')
    {
        static char buf[3];
        int ret, len = req->content_len;
        ESP_LOGI(TAGWEB, "REQ SIZE: %d", len);
            if(len > 2) 
            {
                ESP_LOGI(TAGWEB, "za długie żądanie"); 
                return ESP_OK;
            }
            if ((ret = httpd_req_recv(req, buf, MIN(len, sizeof(buf)))) <= 0) 
            {
                if (ret == HTTPD_SOCK_ERR_TIMEOUT) 
                {
                    return ESP_OK;
                }
                return ESP_FAIL;
            }
            text_rem(text_convert_char2digit(buf, len));
            // ESP_LOGI(TAGWEB, "%d", text_convert_char2digit(buf, len));
            // ESP_LOGI(TAGWEB, "=========== RECEIVED DATA ==========");
            // ESP_LOGI(TAGWEB, "%.*s", ret, buf);
            // ESP_LOGI(TAGWEB, "====================================");
    }
    else if(context[0] == 't')
    {
        static char buf[5];
        int ret, len = req->content_len;
        ESP_LOGI(TAGWEB, "REQ SIZE: %d", len);
            if(len != 5) 
            {
                ESP_LOGI(TAGWEB, "dziwne żądanie"); 
                return ESP_OK;
            }
            if ((ret = httpd_req_recv(req, buf, MIN(len, sizeof(buf)))) <= 0) 
            {
                if (ret == HTTPD_SOCK_ERR_TIMEOUT) 
                {
                    return ESP_OK;
                }
                return ESP_FAIL;
            }
            //ESP_LOGI(TAGWEB, "=========== RECEIVED DATA ==========");
            //ESP_LOGI(TAGWEB, "%.*s", ret, buf);
            //ESP_LOGI(TAGWEB, "====================================");
            
        uint8_t hours = text_convert_char2digit(buf, 2);
        uint8_t minutes = text_convert_char2digit(buf+3, 2);
        ESP_LOGI(TAGWEB, "Nowy czas: %d:%d", hours, minutes);
        clock_set_time(&clockHandle, hours, minutes);
    }
    //index_GET(req);
    index_GET(req);
    return ESP_OK;
}

static esp_err_t update_GET(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send_chunk(req, "[", 1);
    char rsp[3];
    char numer[2];
    uint8_t count = 0;
    numer[1] = '\0';
    for(uint8_t i = 0; i < TEXT_AMOUNT; ++i)
    {
        if(napisy[i].napis[0] != '\0')
        {
            if(count++>0) httpd_resp_send_chunk(req, ",", 1);
            httpd_resp_send_chunk(req, name, sizeof(name)-1);
            if(i < 10) 
            {
                numer[0] = i+48;
                httpd_resp_send_chunk(req, numer, sizeof(numer)-1);
            }else
            {
                numer[0] = (i/10)+48;
                numer[1] = (i % 10)+48;
                httpd_resp_send_chunk(req, numer, sizeof(numer));
            }
            httpd_resp_send_chunk(req, value, sizeof(value)-1);
            httpd_resp_sendstr_chunk(req, napisy[i].napis);
            httpd_resp_send_chunk(req, TIMe, sizeof(TIMe)-1);
            text_convert_digit2char(napisy[i].hour, &rsp);
            httpd_resp_sendstr_chunk(req, rsp);
            httpd_resp_send_chunk(req, colon, 1);
            text_convert_digit2char(napisy[i].minute, &rsp);
            httpd_resp_sendstr_chunk(req, rsp);
            httpd_resp_send_chunk(req, "\"}", 2);
        }
    }
    httpd_resp_send_chunk(req, "]", 1);
    //httpd_resp_sendstr(req, json_dummy);
    httpd_resp_send_chunk(req, "", 0);
    return ESP_OK;
}

static esp_err_t modify_POST(httpd_req_t *req)
{
    static char buf[134];
    static char send[256];
    static uint8_t hours;
    static uint8_t minutes;
    uint8_t place = 99;
    int ret, len = req->content_len;
    ESP_LOGI(TAGWEB, "REQ SIZE: %d", len);

    if(len > 133)
    {
        return ESP_OK;
    } 
    if ((ret = httpd_req_recv(req, buf, MIN(len, sizeof(buf)))) <= 0) 
    {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
            return ESP_OK;
        }
        return ESP_FAIL;
    }
    buf[len] = '\0';
    // ESP_LOGI(TAGWEB, "%d", text_convert_char2digit(buf, len));
    ESP_LOGI(TAGWEB, "=========== RECEIVED DATA ==========");
    ESP_LOGI(TAGWEB, "%.*s", ret, buf);
    ESP_LOGI(TAGWEB, "====================================");
    hours = text_convert_char2digit(buf, 2);
    minutes = text_convert_char2digit(buf+3, 2);
    ESP_LOGI(TAGWEB, "H: %d", hours);
    ESP_LOGI(TAGWEB, "M: %d", minutes);
    if(buf[7] == '.')
    {
        place = text_convert_char2digit(buf+6, 1);
        ESP_LOGI(TAGWEB, "krotszy %s", buf+8);
        ESP_LOGI(TAGWEB, "int %d", *(buf+len));
        text_change(place, buf+8, len-7, hours, minutes);
        memcpy(send, buf+8, len+1);
    }
    else if(buf[8] == '.')
    {
        place = text_convert_char2digit(buf+6, 2);
        ESP_LOGI(TAGWEB, "dluzszy %s", buf+9);
        ESP_LOGI(TAGWEB, "int %d", *(buf+len));
        text_change(place, buf+9, len-8, hours, minutes);
        memcpy(send, buf+9, len-8);
    }

    ESP_LOGI(TAGWEB, "P: %d", place);
    ESP_LOGI(TAGWEB, "%s", send);
        // len -= ret;
        // ESP_LOGI(TAGWEB, "=========== RECEIVED DATA ==========");
        // ESP_LOGI(TAGWEB, "%s", buf+);
        // ESP_LOGI(TAGWEB, "====================================");

    index_GET(req);

    return ESP_OK;
    return ESP_OK;
}

esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Some 404 error message");
    return ESP_FAIL;
}

static httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;
    //text_create_empty();
    // Start the httpd server
    ESP_LOGI(TAGWEB, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        ESP_LOGI(TAGWEB, "Registering URI handlers");
        httpd_register_uri_handler(server, &index_get);
        httpd_register_uri_handler(server, &update_get);
        httpd_register_uri_handler(server, &add_get);
        httpd_register_uri_handler(server, &modify_post);
        httpd_register_uri_handler(server, &remove_post);
        httpd_register_uri_handler(server, &update_time);
        //httpd_register_uri_handler(server, &any);
        return server;
    }
    ESP_LOGI(TAGWEB, "Error starting server!");
    return NULL;
}

static esp_err_t stop_webserver(httpd_handle_t server)
{
    return httpd_stop(server);
}

static void disconnect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        ESP_LOGI(TAGWEB, "Stopping webserver");
        if (stop_webserver(*server) == ESP_OK) {
            *server = NULL;
        } else {
            ESP_LOGE(TAGWEB, "Failed to stop http server");
        }
    }
}

static void connect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        ESP_LOGI(TAGWEB, "Starting webserver");
        *server = start_webserver();
    }
}

void run_webserver()
{
    server = start_webserver();
}

void unrun_webserver()
{
    stop_webserver(server);
}
