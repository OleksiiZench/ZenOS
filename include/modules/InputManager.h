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
    void bindButtonRelease(ButtonID id, std::function<void()> action);
    void bindButtonHold(ButtonID id, std::function<void()> action);

    static constexpr int BUTTON_COUNT = static_cast<int>(ButtonID::Max);
    static constexpr TickType_t DEBOUNCE_TICKS = pdMS_TO_TICKS(20);
private:
    Button _buttons[BUTTON_COUNT];

    static constexpr TickType_t HOLD_DELAY_TICKS = pdMS_TO_TICKS(500);
    static constexpr TickType_t HOLD_REPEAT_TICKS = pdMS_TO_TICKS(100);

    void initializeArrayButtons();
    void setupButtons();

    void updateButtons();
    void processButton(Button& btn, TickType_t now);
    void handleEdgeEvent(Button& btn, int new_state, TickType_t now);
    void handleHoldEvent(Button& btn, TickType_t now);
};
