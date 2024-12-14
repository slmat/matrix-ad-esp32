#include "CLOcK.h"

void clock_init()
{
    CLOcK.hours = 10;
    CLOcK.minutes = 8;
    CLOcK.ready = 1;
    DisplayQueue = xQueueCreate(DISPQ_LENGTH, DISPQ_SIZE);
    //ClockQueue = xQueueCreate(2, sizeof(uint8_t[2]));
    text_add("Reklama",8, 0,0);
    char *temp = text_find_closest(23, 59);
    if(temp != NULL)
    {
        xQueueSend(DisplayQueue, temp, 10);
    }
}

void clock_set_time(TaskHandle_t *clockTask, uint8_t hours, uint8_t minutes)
{
    //vTaskSuspend(*(clockTask));
    vTaskDelete(*(clockTask));
    CLOcK.ready = 0;
    CLOcK.minutes = minutes;
    CLOcK.hours = hours;
    CLOcK.ready = 1;
    char *temp = text_find_closest(CLOcK.hours, CLOcK.minutes);
    if( temp != NULL)
    {
        xQueueSend(DisplayQueue, temp, pdMS_TO_TICKS(2));
    }
    xTaskCreatePinnedToCore(clock_loop,"clock", CLOCK_HEAP_SIZE, NULL, 5, clockTask, APP_CPU_NUM);
    //vTaskResume(*(clockTask));
}

bool clock_add_minute()
{
    if(CLOcK.minutes >= 59)
    {
        CLOcK.minutes = 0;
        return true;
    }
    CLOcK.minutes++;
    return false;
}

bool clock_add_hour()
{
    if(CLOcK.hours >= 23)
    {
        CLOcK.hours = 0;
        return true;
    }
    CLOcK.hours++;
    return false;
}

void clock_addtime()
{
    if(CLOcK.ready)
    {
        CLOcK.ready = 0;
        if(clock_add_minute())
        {
            clock_add_hour();
        }
        CLOcK.ready = 1;
    }
}

uint8_t clock_get_hours()
{
    return CLOcK.hours;
}

uint8_t clock_get_minutes()
{
    return CLOcK.minutes;
}

void clock_loop(void *param)
{
    TickType_t last;last = xTaskGetTickCount();
    while(1)
    {
        clock_addtime();
        char *temp = text_find_current(CLOcK.hours, CLOcK.minutes);
        if( temp != NULL)
        {
            xQueueSend(DisplayQueue, temp, pdMS_TO_TICKS(30000));
        }
        vTaskDelayUntil(&last, pdMS_TO_TICKS(MINUTE));
    }

}