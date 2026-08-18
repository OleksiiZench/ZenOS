#include "Buzzer.h"

#include "driver/ledc.h"

Buzzer::Buzzer()
{
    setupBuzzer();
}

void Buzzer::makeSound()
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 127);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

void Buzzer::mute()
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

void Buzzer::setupBuzzer()
{
    gpio_reset_pin(GPIO_NUM_11);
    gpio_set_direction(GPIO_NUM_11, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_11, 0);

    ledc_timer_config_t ledc_timer = {};
    ledc_timer.speed_mode       = LEDC_LOW_SPEED_MODE;
    ledc_timer.duty_resolution  = LEDC_TIMER_8_BIT;
    ledc_timer.timer_num        = LEDC_TIMER_0;
    ledc_timer.freq_hz          = 1000;
    ledc_timer.clk_cfg          = LEDC_AUTO_CLK;
    
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {};
    ledc_channel.gpio_num       = GPIO_NUM_11;
    ledc_channel.speed_mode     = LEDC_LOW_SPEED_MODE;
    ledc_channel.channel        = LEDC_CHANNEL_0;
    ledc_channel.timer_sel      = LEDC_TIMER_0;
    ledc_channel.intr_type      = LEDC_INTR_DISABLE;
    ledc_channel.duty           = 0;
    ledc_channel.hpoint         = 0;
    
    ledc_channel_config(&ledc_channel);
}
