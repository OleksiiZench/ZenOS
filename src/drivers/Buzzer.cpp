#include "drivers/Buzzer.h"

#include "driver/ledc.h"

Buzzer::Buzzer(gpio_num_t pin)
{
    _pin = pin;
}

void Buzzer::init()
{
    setupBuzzer();
}

void Buzzer::update()
{
    if (!_is_playing)
        return;

    TickType_t now = xTaskGetTickCount();
    uint32_t elapsed_ms = (now - _last_update_tick) * portTICK_PERIOD_MS;

    if (_is_pause)
    {
        updatePauseState(now, elapsed_ms);
    }
    else
    {
        updateNoteState(now, elapsed_ms);
    }
}

void Buzzer::playMelody(const Melody& melody)
{
    if (_is_playing || melody.notes == nullptr || melody.length == 0)
        return;

    _current_melody_notes = melody.notes;
    _current_melodu_length = melody.length;

    _current_note_index = 0;
    _is_playing = true;
    _is_pause = false;
    _last_update_tick = xTaskGetTickCount();

    startTone(_current_melody_notes[0].freq);
}

void Buzzer::stop()
{
    _is_playing = false;
    mute();
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

void Buzzer::startTone(uint32_t freq)
{
    if (freq > 0)
    {
        ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0, freq);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 127);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    }
    else
    {
        mute();
    }
}

void Buzzer::mute()
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

void Buzzer::updatePauseState(TickType_t now, uint32_t elapsed_ms)
{
    if (elapsed_ms < PAUSE_DURATION_MS)
        return;

    _is_pause = false;
    _current_note_index++;

    if (_current_note_index >= _current_melodu_length)
    {
        stop();
        return;
    }

    _last_update_tick = now;
    startTone(_current_melody_notes[_current_note_index].freq);
}

void Buzzer::updateNoteState(TickType_t now, uint32_t elapsed_ms)
{
    if (elapsed_ms < _current_melody_notes[_current_note_index].duration)
        return;

    _is_pause = true;
    _last_update_tick = now;
    mute();
}
