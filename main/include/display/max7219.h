#ifndef MAX7219H
#define MAX7219H
#include <esp_log.h>
#include <driver/spi_master.h>
#include <string.h>
#include "defines.h"

extern spi_device_handle_t SPi;

#define DECODE_MODE     0x09
#define INTENSITY       0x0A
#define SCAN_LIMIT      0x0B
#define SHUTDOWN_MODE   0x0C
#define DISPLAY_TEST    0x0F
#define FIRST_ROW       0x01
#define EMPTY           0x00


esp_err_t SPI_init();
esp_err_t max7219_send(const uint8_t reg, const uint8_t data);
esp_err_t max7219_send_line(const uint8_t reg, const uint8_t *data);
esp_err_t max7219_send_line_32(const uint8_t reg, const uint32_t *data);
esp_err_t max7219_send_line_64(const uint8_t reg, const uint64_t *data);
esp_err_t max7219_init();
esp_err_t max7219_clear();

#endif
