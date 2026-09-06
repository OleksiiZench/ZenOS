#pragma once

#include <stdint.h>

#define ESP_ERROR_CHECK(x) do { (void)(x); } while(0)

typedef int ledc_mode_t;
#define LEDC_LOW_SPEED_MODE 0

typedef int ledc_timer_t;
#define LEDC_TIMER_0 0

typedef int ledc_timer_bit_t;
#define LEDC_TIMER_8_BIT 8

typedef int ledc_clk_cfg_t;
#define LEDC_AUTO_CLK 0

typedef int ledc_channel_t;
#define LEDC_CHANNEL_0 0

typedef int ledc_intr_type_t;
#define LEDC_INTR_DISABLE 0

typedef struct
{
    ledc_mode_t speed_mode;
    ledc_timer_bit_t duty_resolution;
    ledc_timer_t timer_num;
    uint32_t freq_hz;
    ledc_clk_cfg_t clk_cfg;
} ledc_timer_config_t;

typedef struct
{
    int gpio_num;
    ledc_mode_t speed_mode;
    ledc_channel_t channel;
    ledc_intr_type_t intr_type;
    ledc_timer_t timer_sel;
    uint32_t duty;
    int hpoint;
} ledc_channel_config_t;

inline uint32_t mock_ledc_freq = 0;
inline uint32_t mock_ledc_duty = 0;

inline int ledc_timer_config(const ledc_timer_config_t* timer_conf)
{
    return 0;
}

inline int ledc_channel_config(const ledc_channel_config_t* ledc_conf)
{
    return 0;
}

inline int ledc_set_freq(ledc_mode_t speed_mode, ledc_timer_t timer_num, uint32_t freq_hz)
{
    mock_ledc_freq = freq_hz;

    return 0;
}

inline int ledc_set_duty(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t duty)
{
    mock_ledc_duty = duty;
    
    return 0;
}

inline int ledc_update_duty(ledc_mode_t speed_mode, ledc_channel_t channel)
{
    return 0;
}
