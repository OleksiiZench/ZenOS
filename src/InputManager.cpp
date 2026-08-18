#include "InputManager.h"

#include "esp_log.h"

#include "Button.h"

InputManager::InputManager()
{
    initializeArrayButtons();
    setupButtons();
}

void InputManager::bindButton(ButtonID id, void (*action)())
{
    if (id >= 0 && id < BTN_MAX)
    {
        _buttons[id].on_press = action;
    }
}

void InputManager::updateButtons()
{
    for (int i = 0; i < BTN_MAX; i++)
    {
        int current_state = gpio_get_level(_buttons[i].pin);  // gpio_get_level returns 1 if the pin is at 3.3V, and 0 if the pin is shorted to ground

        if (current_state == 0 && _buttons[i].last_state == 1)
        {
            ESP_LOGI("TEMP!!! Log wrapper needed", ">>> BUTTON [%s] PRESSED! <<<", _buttons[i].name);

            if (_buttons[i].on_press != nullptr)
            {
                _buttons[i].on_press();
            }
        }
        else if (current_state == 1 && _buttons[i].last_state == 0)
        {
            ESP_LOGI("TEMP!!! Log wrapper needed", ">>> BUTTON [%s] RELEASED! <<<", _buttons[i].name);
        }

        _buttons[i].last_state = current_state;
    }
}

void InputManager::initializeArrayButtons()
{
    _buttons[BTN_UP]     = { BTN_UP,       GPIO_NUM_38, "UP",     1, nullptr };
    _buttons[BTN_DOWN]   = { BTN_DOWN,     GPIO_NUM_41, "DOWN",   1, nullptr };
    _buttons[BTN_LEFT]   = { BTN_LEFT,     GPIO_NUM_39, "LEFT",   1, nullptr };
    _buttons[BTN_RIGHT]  = { BTN_RIGHT,    GPIO_NUM_40, "RIGHT",  1, nullptr };
    _buttons[BTN_A]      = { BTN_A,        GPIO_NUM_5,  "A",      1, nullptr };
    _buttons[BTN_B]      = { BTN_B,        GPIO_NUM_6,  "B",      1, nullptr };
    _buttons[BTN_C]      = { BTN_C,        GPIO_NUM_10, "C",      1, nullptr };
    _buttons[BTN_D]      = { BTN_D,        GPIO_NUM_9,  "D",      1, nullptr };
    _buttons[BTN_SELECT] = { BTN_SELECT,   GPIO_NUM_0,  "SELECT", 1, nullptr };
    _buttons[BTN_START]  = { BTN_START,    GPIO_NUM_4,  "START",  1, nullptr };
}

void InputManager::setupButtons()
{
    ESP_LOGI("TEMP!!! Log wrapper needed", "Initializing %d buttons...", BTN_MAX);

    for (int i = 0; i < BTN_MAX; i++)
    {
        gpio_config_t io_conf = {};
        io_conf.pin_bit_mask = (1ULL << _buttons[i].pin);
        io_conf.mode = GPIO_MODE_INPUT;
        io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
        io_conf.intr_type = GPIO_INTR_DISABLE;

        gpio_config(&io_conf);
    }

    ESP_LOGI("TEMP!!! Log wrapper needed", "Gamepad initialized successfully!");
}
