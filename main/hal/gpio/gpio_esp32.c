#include "driver/gpio.h"

void gpio_init()
{
    
}

void gpio_set_out(int pin)
{
    gpio_config_t conf = 
    {
        .pin_bit_mask = (((uint64_t)1)<<pin),
        .mode =          GPIO_MODE_OUTPUT,
        .pull_up_en =    GPIO_PULLUP_DISABLE,
        .pull_down_en =  GPIO_PULLDOWN_DISABLE,
        .intr_type =     GPIO_INTR_DISABLE,
    };
    gpio_config(&conf);
}

void gpio_set_in(int pin)
{
    gpio_config_t conf = 
    {
        .pin_bit_mask = (1<<pin),
        .mode =          GPIO_MODE_INPUT,
        .pull_up_en =    GPIO_PULLUP_DISABLE,
        .pull_down_en =  GPIO_PULLDOWN_DISABLE,
        .intr_type =     GPIO_INTR_DISABLE,
    };
    gpio_config(&conf);
}

void gpio_set_high(int pin)
{
    gpio_set_level(pin, 1);
}

void gpio_set_low(int pin)
{
    gpio_set_level(pin, 0);
}