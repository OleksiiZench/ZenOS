#pragma once

#include "EnumTypes.h"
#include "Button.h"

class InputManager
{
public:
    InputManager();

    void bindButton(ButtonID id, void (*action)());
    void updateButtons();

private:
    Button _buttons[BTN_MAX];

    void initializeArrayButtons();
    void setupButtons();
};
