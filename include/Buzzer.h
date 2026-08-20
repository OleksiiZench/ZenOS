#pragma once

#include "driver/gpio.h"

#include "core/IModule.h"

class Buzzer : public IModule
{
public:
    virtual void init() override;

    void makeSound();
    void mute();

private:
    void setupBuzzer();
};
