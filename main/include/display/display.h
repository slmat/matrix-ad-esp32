#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <stdint.h>
#include "max7219.h"
#include "defines.h"
#include <esp_log.h>
#include "maxCharMap.h"
#include "../global/global.h"

#define MAX_TEXT_LENGTH 256
#define QUEUE_SIZE 2

struct Display{
#if MAX_COUNT == 8
    uint64_t screen[8];
#elif MAX_COUNT == 4
    uint32_t screen[8];
#endif
    bool ready;
    bool pending_restart;
};

static struct Display display;
//QueueHandle_t displayQueue;

esp_err_t display_harvester();
esp_err_t display_shift();
esp_err_t display_insert_column(const uint8_t column);
esp_err_t display_init();
esp_err_t display_display();
esp_err_t display_message(const char* message, uint8_t size);
void display_loop(void *param);

#endif