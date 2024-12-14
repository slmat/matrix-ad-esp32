#include "display.h"

esp_err_t display_harvester()
{
    if(MAX_COUNT != 8)
    {
        return ESP_ERR_NOT_ALLOWED;
    }

    uint64_t data[8] = { 1,1,1,1,1,1,1,1 };

    TickType_t last = xTaskGetTickCount();
    while(1)
    {
        for(uint8_t i = 0; i < ROWS; ++i)
        {
            if(max7219_send_line_64(FIRST_ROW+i, &data[i]))
                return ESP_FAIL;
        };
        for(uint8_t i = 0; i < 8; ++i)
            data[i] <<= 1;
        if(data[0] == 0)
        { 
            data[0] = data[1] = data[2] = data[3] = data[4] = data[5] = data[6] = data[7] = 1;
        }
        vTaskDelayUntil(&last, pdMS_TO_TICKS(100));
    }
    return ESP_ERR_INVALID_STATE;
}

esp_err_t display_shift()
{
    for(uint8_t i = 0; i < ROWS; ++i)
    {
        display.screen[i] <<= 1;
    }
    return ESP_OK;
}

esp_err_t display_insert_column(const uint8_t column)
{
    for(uint8_t i = 0; i < ROWS; ++i)
    {
        if((column & 1 << i) != 0)
        {
            display.screen[i] |= 1;
        }
    }
    return ESP_OK;
}

esp_err_t display_init()
{
    esp_err_t ret;
    display.ready = 0;
    ret = SPI_init(); if(ret) return ret;
    ret = max7219_init(); if(ret) return ret;
    ret = max7219_clear(); if(ret) return ret;
    display.screen[0] = display.screen[1] = display.screen[2] = display.screen[3] = display.screen[4] = display.screen[5] = display.screen[6] = display.screen[7] = 0;
    display.ready = 1;
    ESP_LOGI("DISPLAY", "%d", (int)display.screen[0]);
    return ret;
}

esp_err_t display_display()
{
    for(uint8_t i = 0; i < ROWS; ++i)
    {
        if(max7219_send_line_64(FIRST_ROW+i, &display.screen[i]))
            return ESP_FAIL;
    };
   return ESP_OK;
}

esp_err_t display_message(const char* message, uint8_t size)
{
    TickType_t last = xTaskGetTickCount();

    uint8_t column;
    for(uint8_t r = 0; r < REPEAT; ++r)
    {
        for(uint8_t i = 0; i < size - 1; ++i)
        {
            char lastChar = message[i];

            display_shift();
            BLOCK_UNTIL(if(display_display()) return ESP_FAIL;,display.ready);
            vTaskDelayUntil(&last, pdMS_TO_TICKS(SCROLL_TIME));
            switch (lastChar)
            {
            case (0xC3):
                lastChar = message[++i] - 10;
            break;
            case (0xC4):
                lastChar = message[++i] - 3;
            break;
            case (0xC5):
                lastChar = message[++i] + 4;
            break;
            
            default:
                break;
            }

            for(uint8_t j = 0; j < 5; ++j)
            {
                column = charMapReadColumn(decode_utf(lastChar),j);

                if(column == 0x00 && j < 2) {  continue; }

                if( column == 0x00 && j > 2 && lastChar != 32) break;

                display_shift();
                display_insert_column(column);

                BLOCK_UNTIL(if(display_display()) return ESP_FAIL;,display.ready);
                vTaskDelayUntil(&last, pdMS_TO_TICKS(SCROLL_TIME));
            }
        }
        
        for(uint8_t i = 0; i < 64; ++i)
        {
            display_shift();
            BLOCK_UNTIL(if(display_display()) return ESP_FAIL;,display.ready);
            vTaskDelayUntil(&last, pdMS_TO_TICKS(SCROLL_TIME));
        }
    }
    return ESP_OK;
}

void display_loop(void *param)
{
    uint8_t column;
    static char text[MAX_TEXT_LENGTH];
    TickType_t last = xTaskGetTickCount();


    while(1){
        xQueueReceive(DisplayQueue, (void *)text, 0);
        for(uint8_t i = 0; text[i] != '\0'; ++i)
        {
            display_shift();
            display_display();
            vTaskDelayUntil(&last, pdMS_TO_TICKS(SCROLL_TIME));

            char lastChar = text[i];
            switch (lastChar)
            {
                case (0xC3):
                    lastChar = text[++i] - 10;
                break;
                case (0xC4):
                    lastChar = text[++i] - 3;
                break;
                case (0xC5):
                    lastChar = text[++i] + 4;
                break;
                
                default:
                break;
            }

            for(uint8_t j = 0; j < 5; ++j)
            {
                column = charMapReadColumn(decode_utf(lastChar),j);

                if(column == 0x00 && j < 2) {  continue; }

                if( column == 0x00 && j > 2 && lastChar != 32) break;

                display_shift();
                display_insert_column(column);
                display_display();
                vTaskDelayUntil(&last, pdMS_TO_TICKS(SCROLL_TIME));
            }

        }
        for(uint8_t i = 0; i < MAX_COUNT*8; ++i)
        {
            display_shift();
            display_display();
            vTaskDelayUntil(&last, pdMS_TO_TICKS(SCROLL_TIME));
        }
    }

}