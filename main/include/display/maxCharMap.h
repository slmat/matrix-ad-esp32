#ifndef MAXCHARMAP_H_
#define MAXCHARMAP_H_

#include <stdint.h>

uint8_t charMapReadColumn(uint8_t c, uint8_t col);

uint8_t decode_utf(uint8_t c);

uint8_t get_char_width(uint8_t c);

#endif /* MAXCHARMAP_H_ */