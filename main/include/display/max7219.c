#include "max7219.h"

spi_device_handle_t SPi;

esp_err_t SPI_init()
{
    spi_bus_config_t bus = {
        .mosi_io_num = MOSI,                    //< mosi pin
        .miso_io_num = -1,                      //< miso pin
        .sclk_io_num = CLK,                     //< clk pin
        .quadwp_io_num = -1,                    // Write Protect dla Quad mode
        .quadhd_io_num = -1,                    // Hold signal dla Quad mode
        .max_transfer_sz = MAX_DATA_SIZE_BYTES  // Maximum transfer size in bytes (dalem 4 * 2 bo 4 wyswietlacze po rejestr+dane(czyli 1 + 1 bajtow))
    };

    spi_device_interface_config_t dev = {
        .mode = 0,                        // 0 = zegar normalnie w stanie niskim i zmiana na opadajacym zboczu, 1 = zegar normalnie w stanie niskim i zmiana na wzrastajacym zboczu, 3 = zegar normalnie w stanie wysokim i zmiana na wzrastajacym zboczu, 4 = zegar normalnie w stanie wysokim i zmiana na opadajacym zboczu,
        .clock_speed_hz = 5000000,        // 5 MHz (ten spi chyba ma max 10MHz a max7219 ma max 8MHz jesli dobrze pamietam)
        .spics_io_num = CS,               
        .queue_size = 1,                  // ile transakcji moze byc zakolejkowanych (nie skonczonych)
    };

    esp_err_t ret;
#ifdef WROOM
    ret = spi_bus_initialize(VSPI_HOST, &bus, SPI_DMA_CH_AUTO);
    if(ret) return ret;
    ret = spi_bus_add_device(VSPI_HOST, &dev, &SPi);
    if(ret) return ret;    
#endif
#ifdef MINI
    ret = spi_bus_initialize(SPI2_HOST, &bus, SPI_DMA_CH_AUTO);
    if(ret) return ret;
    ret = spi_bus_add_device(SPI2_HOST, &dev, &SPi);
    if(ret) return ret;
#endif
    return ret;
}

esp_err_t max7219_send(const uint8_t reg, const uint8_t data)
{

    uint8_t tx_data[MAX_COUNT*2];
    for(uint8_t i = 0; i < MAX_COUNT; ++i)
    {
        tx_data[i*2] = reg; 
        tx_data[i*2+1] = data;
    }
        spi_transaction_t trans;

    memset(&trans, 0, sizeof(trans));

    trans.length = MAX_COUNT * 8 * 2;
    trans.tx_buffer = tx_data;
    trans.rx_buffer = NULL;
    trans.flags = 0;

    return spi_device_transmit(SPi, &trans);
}

esp_err_t max7219_send_line(uint8_t reg, const uint8_t * data)
{
    uint8_t tx_data[MAX_COUNT*2];
    for(uint8_t i = 0; i < MAX_COUNT; ++i)
    {
        tx_data[i*2] = reg; 
        tx_data[i*2+1] = *(data+i);
    }

    spi_transaction_t trans;

    memset(&trans, 0, sizeof(trans));
    trans.length = MAX_COUNT * 8 * 2;
    trans.tx_buffer = tx_data;
    trans.rx_buffer = NULL;
    trans.flags = 0;
    
    return spi_device_transmit(SPi, &trans);
}

esp_err_t max7219_send_line_32(const uint8_t reg,const uint32_t *data)
{
    if(MAX_COUNT != 4)
    {
        return ESP_ERR_NOT_ALLOWED;
    }

    uint8_t temp[4];

    temp[0] = (uint8_t)(*(data)>>24);
    temp[1] = (uint8_t)(*(data)>>16);
    temp[2] = (uint8_t)(*(data)>>8);
    temp[3] = (uint8_t)(*(data));

    return max7219_send_line(reg, temp);
}

esp_err_t max7219_send_line_64(const uint8_t reg,const uint64_t *data)
{
    if(MAX_COUNT != 8)
    {
        return ESP_ERR_NOT_ALLOWED;
    }

    uint8_t temp[8];

    temp[0] = (uint8_t)(*(data)>>56);
    temp[1] = (uint8_t)(*(data)>>48);
    temp[2] = (uint8_t)(*(data)>>40);
    temp[3] = (uint8_t)(*(data)>>32);
    temp[4] = (uint8_t)(*(data)>>24);
    temp[5] = (uint8_t)(*(data)>>16);
    temp[6] = (uint8_t)(*(data)>>8);
    temp[7] = (uint8_t)(*(data));

    return max7219_send_line(reg, temp);
}

esp_err_t max7219_init()
{
    esp_err_t ret;
    ret = max7219_send(DECODE_MODE, 0x00);    // decode mode wylaczony (bez BCD dla poszczegolnych bitow)
    if(ret) return ret;

#ifdef DEBUG
    ESP_LOGI("MAX", "decode off");
#endif

    ret = max7219_send(INTENSITY, 0x01);    // ustawienie jasnosci na najnizsza wartosc 0xXF najwiecej
    if(ret) return ret;

#ifdef DEBUG
    ESP_LOGI("MAX", "jasnosc");
#endif

    ret = max7219_send(SCAN_LIMIT, 0x07);    // scan limit czyli ile diod ma byc wykorzystywane my chcemy wsystkie czyli 0xX7
    if(ret) return ret;

#ifdef DEBUG
    ESP_LOGI("MAX", "scan limit");
#endif

    ret = max7219_send(SHUTDOWN_MODE, 0x01);    // 1 - Normal mode (dziaua)  0 - Shutdown mode (nie dziaua)
    if(ret) return ret;

#ifdef DEBUG
    ESP_LOGI("MAX", "shutdown");
#endif

#ifdef TEST_DISPLAY
    ret = max7219_send(DISPLAY_TEST, 0x01);    // display test na odwrut niz shutdown mode jak jest 1 - robi test czy sie swieca wszystkie lampki
    if(ret) return ret;

#ifdef DEBUG
    ESP_LOGI("MAX", "display test");
#endif

#else
    ret = max7219_send(DISPLAY_TEST, 0x00);
    if(ret) return ret;

#ifdef DEBUG
    ESP_LOGI("MAX", "no display test");
#endif

#endif

    return ret;
}

esp_err_t max7219_clear()
{
    esp_err_t ret;
    for(uint8_t i = 0; i < ROWS; ++i)
    {
        ret = max7219_send(0x01+i, 0x00);
        if(ret) return ret;
    }

#ifdef DEBUG
        ESP_LOGI("MAX", "Clear");
#endif

    return ret;
}


