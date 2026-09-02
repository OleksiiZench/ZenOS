#pragma once

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "core/IModule.h"

struct Note
{
    uint32_t freq;
    uint32_t duration;
};

struct Melody
{
    const Note* notes;
    size_t length;
};




class Buzzer : public IModule
{
public:
    explicit Buzzer(gpio_num_t pin);

    virtual void init() override;
    virtual void update() override;

    void playMelody(const Melody& melody);
    void stop();

private:
    gpio_num_t _pin;

    bool _is_playing = false;
    bool _is_pause = false;
    size_t _current_note_index = 0;
    TickType_t _last_update_tick = 0;

    const Note* _current_melody_notes = nullptr;
    size_t _current_melodu_length = 0;

    static constexpr uint32_t PAUSE_DURATION_MS = 20;

    void setupBuzzer();
    void startTone(uint32_t freq);
    void mute();

    void updatePauseState(TickType_t now, uint32_t elapsed_ms);
    void updateNoteState(TickType_t now, uint32_t elapsed_ms);
};
