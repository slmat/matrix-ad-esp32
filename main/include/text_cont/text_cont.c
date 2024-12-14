#include "text_cont.h"

void text_create_empty()
{
    for(uint8_t i = 0; i < TEXT_AMOUNT; ++i)
    {
       if(napisy[i].hour == 99 &&
          napisy[i].minute == 99 &&
          napisy[i].napis[0] != '\0') return;
    }

    for(uint8_t i = 0; i < TEXT_AMOUNT; ++i)
    {
        
        if(napisy[i].napis[0] == '\0')
        {
            napisy[i].napis[0] = 'N';
            napisy[i].napis[1] = 'o';
            napisy[i].napis[2] = 'w';
            napisy[i].napis[3] = 'y';
            napisy[i].napis[4] = '\0';
            napisy[i].hour = 99;
            napisy[i].minute = 99;
            return;
        }
    }
}

void text_add(const char *text, uint8_t size, uint8_t hour, uint8_t minute)
{
    if(hour > 23 || minute > 59) 
    {
        return;
    }

    for(uint8_t i = 0; i < TEXT_AMOUNT; ++i)
    {
        if(napisy[i].napis[0] != '\0')
        {
            if(napisy[i].hour == hour)
            {
                if(napisy[i].minute == minute)
                {
                    return;
                }
            }
        }
    }
    for(uint8_t i = 0; i < TEXT_AMOUNT; ++i)
    {
        if(napisy[i].napis[0] == '\0')
        {
            for(uint8_t j = 0; j < size; ++j)
            {
                napisy[i].napis[j] = *(text+j);
            }
            napisy[i].hour = hour;
            napisy[i].minute = minute;
            return;
        }
    }
}

void text_change(uint8_t place, const char *text, uint8_t size, uint8_t hour, uint8_t minute)
{
    for(uint8_t i = 0; i < TEXT_AMOUNT; ++i)
    {
        if(napisy[i].napis[0] != '\0')
        {
            if(napisy[i].hour == hour)
            {
                if(napisy[i].minute == minute)
                {
                    if(place != i) return;
                }
            }
        }
    }
    
    if(place > TEXT_AMOUNT)
    {
        return;
    }

    for(uint8_t i = 0; i < size; ++i)
    {
        napisy[place].napis[i] = *(text+i);
    }
    napisy[place].hour = hour;
    napisy[place].minute = minute;
}

void text_rem(uint8_t place)
{
    if(place > TEXT_AMOUNT)
    {
        return;
    }

    napisy[place].napis[0] = '\0';
}

void text_change_time(uint8_t place, uint8_t hour, uint8_t minute)
{
    if(place > TEXT_AMOUNT)
    {
        return;
    }

    napisy[place].hour = hour;
    napisy[place].minute = minute;
}

char *text_find_current(uint8_t hour, uint8_t minute)
{
    if(hour > 23 || minute > 59) return NULL;

    for(uint8_t i = 0; i <TEXT_AMOUNT; ++i)
    {
        if(napisy[i].hour == hour)
        {
            if(napisy[i].minute == minute)
            {
                return (char*)(napisy[i].napis);
            }
        }
    }
    return NULL;
}

char *text_find_closest(uint8_t hour, uint8_t minute)
{
    if(hour > 23 || minute > 59) return NULL;
    
    uint8_t temp_h = 100;
    uint8_t temp_m = 100;
    char *which = NULL;
    for (uint8_t i = 0; i < TEXT_AMOUNT; ++i) 
    {
        if (napisy[i].napis[0] != '\0')
        {
            uint8_t h = napisy[i].hour;
            uint8_t m = napisy[i].minute;

            if ((h < hour) || (h == hour && m <= minute)) 
            {
                if(temp_h > 23 || temp_m > 59) 
                {
                    temp_h = h; 
                    temp_m = m;
                    which = napisy[i].napis;
                }
                if ((h > temp_h) || (h == temp_h && m > temp_m)) 
                {
                    temp_h = h;
                    temp_m = m;
                    which = napisy[i].napis;
                }
            }
        }
    }
    return which;
}

void text_convert_digit2char(uint8_t digit, char *buf)
{
    if(buf == NULL) return;
    if(digit > 99) digit = 99;

    buf[2] = '\0';
    buf[0] = ((digit / 10)+48);
    buf[1] = ((digit % 10)+48);
}

uint8_t text_convert_char2digit(char *buf, uint8_t size)
{
    if(buf == NULL) return 99;
    uint8_t tens = buf[0]-48;
    if(size == 1) 
    {
        if( tens > 9) return 99;
        return tens;
    }
    if(size == 2)
    {
        uint8_t ones = buf[1]-48;
        if( tens > 5) return 99;
        if( ones > 9) return 99;
        return tens*10+ones;
    } 
    return 99;
}

void text_list()
{
    for(uint8_t i = 0; i < TEXT_AMOUNT;  ++i)
    {
        if(napisy[i].napis[0] != '\0')
        {
            printf("napis[%d]: ", i);
            printf(napisy[i].napis);
            printf(" %d:%d\n", napisy[i].hour, napisy[i].minute);
        }
    }
}
