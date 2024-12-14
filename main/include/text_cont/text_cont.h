#ifndef TEXT_CONT_H
#define TEXT_CONT_H

#include <stdint.h>
#include <stdio.h>
#include "../global/global.h"

// typedef struct Napis Napis;

// struct Napis{
//     char napis[TEXT_LENGTH];
//     uint8_t hour;
//     uint8_t minute;
// };

// static Napis napisy[TEXT_AMOUNT];

void text_create_empty();
void text_add(const char *text, uint8_t size, uint8_t hour, uint8_t minute);
void text_change(uint8_t place, const char *text, uint8_t size, uint8_t hour, uint8_t minute);
void text_rem(uint8_t place);
void text_change_time(uint8_t place, uint8_t hour, uint8_t minute);
char *text_find_current(uint8_t hour, uint8_t minute);
char *text_find_closest(uint8_t hour, uint8_t minute);
void text_convert_digit2char(uint8_t digit, char *buf);
uint8_t text_convert_char2digit(char *buf, uint8_t size);
void text_list();

#endif