#include <stdio.h>

#include "driver/uart.h"

#include "mt_gpio.h"
#include "mt_spi.h"
#include "mt_radio.h"

#include "sx1262.h"

#include "board.h"

#define BUF_SIZE 1024

void board_uart_init()
{
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
    };
    // Configure UART parameters
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, BUF_SIZE * 2, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_0, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0, 43, 44, -1, -1));
}

void board_pins_init()
{
    gpio_set_out(LED_PIN);
    gpio_set_high(LED_PIN);

    gpio_set_out(LORA_RST);
    gpio_set_high(LORA_RST);
    gpio_set_in(LORA_DIO1);
    gpio_set_in(LORA_BUSY);
}

void board_init()
{
    sx1262_config radio_config =
    {

    };
    board_pins_init();
    spi_init();
    radio_config.spi_handle = spi_register_device(LORA_NSS,1000000,1);
    radio_init((void *)&radio_config);
}
