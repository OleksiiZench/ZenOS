#pragma once

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "core/IModule.h"

class Buzzer : public IModule
{
public:
    explicit Buzzer(gpio_num_t pin);

    virtual void init() override;
    virtual void update() override;

    void makeSound();
    void stop();

private:
    gpio_num_t _pin;

    bool _is_playing = false;
    bool _is_pause = false;
    size_t _current_note_index = 0;
    TickType_t _last_update_tick = 0;

    static constexpr uint32_t PAUSE_DURATION_MS = 20;

    void setupBuzzer();
    void startTone(uint32_t freq);
    void mute();
};
