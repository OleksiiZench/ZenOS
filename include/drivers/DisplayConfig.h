#pragma once

#include "driver/gpio.h"

struct DisplayConfig
{
    gpio_num_t pin_backlight;
    gpio_num_t pin_mosi;
    gpio_num_t pin_clk;
    gpio_num_t pin_cs;
    gpio_num_t pin_dc;
    gpio_num_t pin_rst;
};
