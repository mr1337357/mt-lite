#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "board.h"

#include "mt_gpio.h"
#include "mt_spi.h"

#include "sx1262.h"

int radio_init(void *cfg)
{
    sx1262_config *config = (sx1262_config *)cfg;
    gpio_set_low(LORA_RST);
    vTaskDelay(100 * portTICK_PERIOD_MS);
    gpio_set_high(LORA_RST);
    return -1;
}

static int sx1262_send_cmd(uint8_t cmd, uint8_t *arg, int len)
{

    return 0;
}