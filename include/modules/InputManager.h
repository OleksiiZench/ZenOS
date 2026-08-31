#pragma once

#include <functional>

#include "core/IModule.h"
#include "drivers/Button.h"

class InputManager : public IModule
{
public:
    InputManager();

    virtual void init() override;
    virtual void update() override;

    void bindButton(ButtonID id, std::function<void()> action);
private:
    static constexpr int BUTTON_COUNT = static_cast<int>(ButtonID::Max);
    static constexpr TickType_t DEBOUNCE_TICKS = pdMS_TO_TICKS(20);

    Button _buttons[BUTTON_COUNT];

    void initializeArrayButtons();
    void setupButtons();

    void updateButtons();
};
