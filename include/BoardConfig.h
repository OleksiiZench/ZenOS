#pragma once

#include "drivers/DisplayConfig.h"

namespace BoardConfig
{
    constexpr DisplayConfig DISPLAY_CONFIG = {
        .pin_backlight    = GPIO_NUM_46,
        .pin_mosi         = GPIO_NUM_17,
        .pin_clk          = GPIO_NUM_18,
        .pin_cs           = GPIO_NUM_7,
        .pin_dc           = GPIO_NUM_15,
        .pin_rst          = GPIO_NUM_NC,
        .width            = 240,
        .height           = 280,
        .default_bg_color = 0x07E0
    };
}
