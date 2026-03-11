#pragma once

#include "driver/gpio.h"

#include "EnumTypes.h"

struct Button
{
    ButtonID id;
    gpio_num_t pin;
    const char *name;
    int last_state;
    
    void (*on_press)();
};
