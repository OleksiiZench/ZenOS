#include "drivers/Buzzer.h"

#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

Buzzer::Buzzer(gpio_num_t pin)
{
    _pin = pin;
}

void Buzzer::init()
{
    setupBuzzer();
}

void Buzzer::makeSound()
{
    struct Note
    {
        uint32_t freq;
        uint32_t duration;
    };

    // Melody: Tetris (Korobeiniki)
    Note melody[] = {
        { 659, 250 }, { 494, 125 }, { 523, 125 }, { 587, 250 },
        { 523, 125 }, { 494, 125 }, { 440, 250 }, { 440, 125 },
        { 523, 125 }, { 659, 250 }, { 587, 125 }, { 523, 125 },
        { 494, 375 }, { 523, 125 }, { 587, 250 }, { 659, 250 },
        { 523, 250 }, { 440, 250 }, { 440, 500 }
    };

    for (const Note& note : melody)
    {
        playTone(note.freq, note.duration);
        vTaskDelay(pdMS_TO_TICKS(30));
    }
}

void Buzzer::mute()
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

void Buzzer::setupBuzzer()
{
    gpio_reset_pin(_pin);
    gpio_set_direction(_pin, GPIO_MODE_OUTPUT);
    gpio_set_level(_pin, 0);

    ledc_timer_config_t ledc_timer = {};
    ledc_timer.speed_mode       = LEDC_LOW_SPEED_MODE;
    ledc_timer.duty_resolution  = LEDC_TIMER_8_BIT;
    ledc_timer.timer_num        = LEDC_TIMER_0;
    ledc_timer.freq_hz          = 1000;
    ledc_timer.clk_cfg          = LEDC_AUTO_CLK;

    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    ledc_channel_config_t ledc_channel = {};
    ledc_channel.gpio_num       = _pin;
    ledc_channel.speed_mode     = LEDC_LOW_SPEED_MODE;
    ledc_channel.channel        = LEDC_CHANNEL_0;
    ledc_channel.timer_sel      = LEDC_TIMER_0;
    ledc_channel.intr_type      = LEDC_INTR_DISABLE;
    ledc_channel.duty           = 0;
    ledc_channel.hpoint         = 0;

    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void Buzzer::playTone(uint32_t freq, uint32_t duration_ms)
{
    if (freq > 0)
    {
        ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0, freq);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 250);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    }
    else
    {
        mute();
    }

    vTaskDelay(pdMS_TO_TICKS(duration_ms));
    mute();
}
