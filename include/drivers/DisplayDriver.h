#pragma once

#include "driver/gpio.h"
#include "driver/spi_master.h"

class DisplayDriver
{
public:
    DisplayDriver();

    void turnOnBacklight();
    void fillGreen();

private:
    static constexpr gpio_num_t PIN_BACKLIGHT = GPIO_NUM_46;
    static constexpr gpio_num_t PIN_MOSI      = GPIO_NUM_17;
    static constexpr gpio_num_t PIN_CLK       = GPIO_NUM_18;
    static constexpr gpio_num_t PIN_CS        = GPIO_NUM_7;
    static constexpr gpio_num_t PIN_DC        = GPIO_NUM_15;
    static constexpr gpio_num_t PIN_RST       = GPIO_NUM_NC;

    void initSPI();
    void initController();
};
