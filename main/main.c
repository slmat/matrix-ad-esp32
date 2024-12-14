#include "include/AP/AP.h"
#include "include/global/global.h"
#include "include/clock/clock.h"
#include "include/display/display.h"

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    wifi_init_softap();
    clock_init();
    xTaskCreatePinnedToCore(clock_loop, "clock task", CLOCK_HEAP_SIZE, NULL, 5, &clockHandle, APP_CPU_NUM);
    display_init();
    xTaskCreatePinnedToCore(display_loop, "display task", 1024+512, NULL, 1, NULL, APP_CPU_NUM);
}

