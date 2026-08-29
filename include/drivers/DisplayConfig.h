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

    uint16_t width;
    uint16_t height;
    uint16_t default_bg_color = 0x0000;
    uint16_t panel_y_offset = 0;
};
