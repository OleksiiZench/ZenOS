#pragma once

#include "core/IModule.h"
#include "Button.h"

class InputManager : public IModule
{
public:
    virtual void init() override;
    virtual void update() override;

    void bindButton(ButtonID id, void (*action)());
private:
    static constexpr int BUTTON_COUNT = static_cast<int>(ButtonID::Max);

    Button _buttons[BUTTON_COUNT];

    void initializeArrayButtons();
    void setupButtons();

    void updateButtons();
};
