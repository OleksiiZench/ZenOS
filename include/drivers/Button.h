#pragma once

#include <functional>

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

enum class ButtonID
{
    None = -1,

    Up = 0,
    Down,
    Left,
    Right,
    A,
    B,
    C,
    D,
    Select,
    Start,

    Max
};




struct Button
{
    ButtonID id = ButtonID::None;
    gpio_num_t pin = GPIO_NUM_NC;
    const char *name = "UNKNOWN";

    int raw_state = 1;
    int stable_state = 1;
    TickType_t last_change_tick = 0;

    std::function<void()> on_press = nullptr;
};
