#pragma once

#include "drivers/DisplayConfig.h"
#include "drivers/OLEDConfig.h"

namespace BoardConfig
{
    constexpr gpio_num_t PIN_BUZZER = GPIO_NUM_11;

    constexpr DisplayConfig DISPLAY_CONFIG = {
        .pin_backlight    = GPIO_NUM_46,
        .pin_mosi         = GPIO_NUM_17,
        .pin_clk          = GPIO_NUM_18,
        .pin_cs           = GPIO_NUM_7,
        .pin_dc           = GPIO_NUM_15,
        .pin_rst          = GPIO_NUM_NC,
        .width            = 240,
        .height           = 280,
        .default_bg_color = 0x07E0,
        .panel_y_offset = 20
    };

    constexpr OLEDConfig OLED_CONFIG = {
        .pin_sda        = GPIO_NUM_13,
        .pin_scl        = GPIO_NUM_12,
        .i2c_port       = I2C_NUM_0,
        .clock_speed_hz = 400000,
        .address        = 0x3C,
        .width          = 128,
        .height         = 64
    };
}
