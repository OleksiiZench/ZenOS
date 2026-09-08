#pragma once

#include "driver/gpio.h"
#include "driver/i2c_master.h"

struct OLEDConfig
{
    gpio_num_t pin_sda;
    gpio_num_t pin_scl;
    i2c_port_t i2c_port;
    uint32_t clock_speed_hz;
    uint8_t address;
    uint16_t width;
    uint16_t height;
};
