#pragma once

#include "Button.h"

class InputManager
{
public:
    InputManager();

    void bindButton(ButtonID id, void (*action)());
    void updateButtons();

private:
    static constexpr int BUTTON_COUNT = static_cast<int>(ButtonID::Max);

    Button _buttons[BUTTON_COUNT];

    void initializeArrayButtons();
    void setupButtons();
};
