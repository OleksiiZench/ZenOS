#pragma once

#include "driver/gpio.h"

class Buzzer
{
public:
    Buzzer();

    void makeSound();
    void mute();

private:
    void setupBuzzer();
};
