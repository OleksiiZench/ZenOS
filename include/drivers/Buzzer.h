#pragma once

#include "driver/gpio.h"

#include "core/IModule.h"

class Buzzer : public IModule
{
public:
    explicit Buzzer(gpio_num_t pin);

    virtual void init() override;

    void makeSound();
    void mute();

private:
    gpio_num_t _pin;

    void setupBuzzer();
};
