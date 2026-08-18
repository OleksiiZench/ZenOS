#pragma once

#include "driver/gpio.h"

#include "EnumTypes.h"

struct Button
{
    ButtonID id = BTN_NONE;
    gpio_num_t pin = GPIO_NUM_NC;
    const char *name = "UNKNOWN";
    int last_state = 1;
    
    void (*on_press)() = nullptr;
};
