#pragma once

#include <stdint.h>

typedef int gpio_num_t;

#define GPIO_NUM_NC -1
#define GPIO_MODE_OUTPUT 1

inline void gpio_reset_pin(gpio_num_t pin)
{
}

inline void gpio_set_direction(gpio_num_t pin, int mode)
{
}

inline void gpio_set_level(gpio_num_t pin, uint32_t level)
{
}
