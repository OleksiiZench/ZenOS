#pragma once

#include "driver/gpio.h"

class Buzzer
{
public:
    Buzzer();

    void make_sound();
    void mute();

private:
    void setup_buzzer();
};