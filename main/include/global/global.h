#ifndef GLOBAL_H
#define GLOBAL_H

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#define TIMEQ_LENGTH    3
#define DISPQ_LENGTH    2
#define DISPQ_SIZE      sizeof(char[256])
#define TIMEQ_SIZE      sizeof(uint8_t[2])

#define TEXT_AMOUNT     24
#define TEXT_LENGTH     256

//extern QueueHandle_t ClockQueue;
extern TaskHandle_t clockHandle;
extern QueueHandle_t DisplayQueue;

typedef struct Napis Napis;

struct Napis{
    char napis[TEXT_LENGTH];
    uint8_t hour;
    uint8_t minute;
};

extern Napis napisy[TEXT_AMOUNT];



#endif
