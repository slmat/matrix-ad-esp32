#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>
#include <stdbool.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <esp_mac.h>
#include <string.h>
#include "../global/global.h"
#include "../text_cont/text_cont.h"

#define CLOCK_HEAP_SIZE 800
#define MINUTE 60000

typedef struct Time Time;

struct Time {
    uint8_t hours;
    uint8_t minutes;
    bool ready;
};

static struct Time CLOcK;

void clock_init();
void clock_set_time(TaskHandle_t *clockTask, uint8_t hours, uint8_t minutes);
bool clock_add_minute();
bool clock_add_hour();
void clock_addtime();
uint8_t clock_get_hours();
uint8_t clock_get_minutes();
void clock_loop(void *param);

#endif