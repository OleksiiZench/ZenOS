#pragma once

#include <stdint.h>

typedef int gpio_num_t;

#define GPIO_NUM_NC -1
#define GPIO_NUM_0 0
#define GPIO_NUM_4 4
#define GPIO_NUM_5 5
#define GPIO_NUM_6 6
#define GPIO_NUM_9 9
#define GPIO_NUM_10 10
#define GPIO_NUM_38 38
#define GPIO_NUM_39 39
#define GPIO_NUM_40 40
#define GPIO_NUM_41 41

#define GPIO_MODE_OUTPUT 1
#define GPIO_MODE_INPUT 2
#define GPIO_PULLUP_ENABLE 1
#define GPIO_PULLDOWN_DISABLE 0
#define GPIO_INTR_DISABLE 0

typedef struct 
{
    uint64_t pin_bit_mask;
    int mode;
    int pull_up_en;
    int pull_down_en;
    int intr_type;
} gpio_config_t;

inline int gpio_config(const gpio_config_t* pGPIOConfig)
{
    return 0;
}

inline int mock_gpio_levels[45];

inline void reset_mock_gpio()
{
    for (int i = 0; i < 45; i++)
    {
        mock_gpio_levels[i] = 1;
    }
}

inline int gpio_get_level(gpio_num_t pin)
{
    if (pin >= 0 && pin < 45)
        return mock_gpio_levels[pin];

    return 1;
}

inline void gpio_reset_pin(gpio_num_t pin)
{
}

inline void gpio_set_direction(gpio_num_t pin, int mode)
{
}

inline void gpio_set_level(gpio_num_t pin, uint32_t level)
{
}
