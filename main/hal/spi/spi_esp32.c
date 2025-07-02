
#include "driver/spi_master.h"

#include "board.h"

spi_device_handle_t devices[10];
int devicecount = 0;

int spi_init()
{
    spi_bus_config_t buscfg = 
    {
        .miso_io_num = LORA_MISO,
        .mosi_io_num = LORA_MOSI,
        .sclk_io_num = LORA_SCK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 32
    };
    


    return spi_bus_initialize(SPI1_HOST, &buscfg, SPI_DMA_CH_AUTO);
}

int spi_register_device(int cs, int speed, int mode)
{
    esp_err_t rv;
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = speed,
        .mode = mode,
        .spics_io_num = cs,
        .queue_size = 2,
        .pre_cb = 0,
    };
    rv = spi_bus_add_device(SPI1_HOST, &devcfg, &devices[devicecount]);
    if(rv == ESP_OK)
    {
        return devicecount++;
    }
    return -1;
}

int spi_transact(int handle, uint8_t *data, uint8_t len)
{
    spi_device_acquire_bus(devices[handle], portMAX_DELAY);
    spi_transaction_t t = 
    {
        .length = len,
        .tx_buffer = data,
        .rx_buffer = data,
        .user = 0,
        .flags = 0,
    };
    spi_device_polling_transmit(devices[handle], &t);

    spi_device_release_bus(devices[handle]);
    return -1;
}