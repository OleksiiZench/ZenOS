#include "modules/InputManager.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "drivers/Button.h"

static const char* TAG = "InputManager";

InputManager::InputManager()
{
    initializeArrayButtons();
}

void InputManager::init()
{
    setupButtons();
}

void InputManager::update()
{
    updateButtons();
}

void InputManager::bindButton(ButtonID id, std::function<void()> action)
{
    if (id >= ButtonID::Up && id < ButtonID::Max)
    {
        _buttons[static_cast<int>(id)].on_press = action;
    }
}

void InputManager::initializeArrayButtons()
{
    _buttons[static_cast<int>(ButtonID::Up)]     = { ButtonID::Up,       GPIO_NUM_38, "UP",     1, 1, 0, nullptr };
    _buttons[static_cast<int>(ButtonID::Down)]   = { ButtonID::Down,     GPIO_NUM_41, "DOWN",   1, 1, 0, nullptr };
    _buttons[static_cast<int>(ButtonID::Left)]   = { ButtonID::Left,     GPIO_NUM_39, "LEFT",   1, 1, 0, nullptr };
    _buttons[static_cast<int>(ButtonID::Right)]  = { ButtonID::Right,    GPIO_NUM_40, "RIGHT",  1, 1, 0, nullptr };
    _buttons[static_cast<int>(ButtonID::A)]      = { ButtonID::A,        GPIO_NUM_5,  "A",      1, 1, 0, nullptr };
    _buttons[static_cast<int>(ButtonID::B)]      = { ButtonID::B,        GPIO_NUM_6,  "B",      1, 1, 0, nullptr };
    _buttons[static_cast<int>(ButtonID::C)]      = { ButtonID::C,        GPIO_NUM_10, "C",      1, 1, 0, nullptr };
    _buttons[static_cast<int>(ButtonID::D)]      = { ButtonID::D,        GPIO_NUM_9,  "D",      1, 1, 0, nullptr };
    _buttons[static_cast<int>(ButtonID::Select)] = { ButtonID::Select,   GPIO_NUM_0,  "SELECT", 1, 1, 0, nullptr };
    _buttons[static_cast<int>(ButtonID::Start)]  = { ButtonID::Start,    GPIO_NUM_4,  "START",  1, 1, 0, nullptr };
}

void InputManager::setupButtons()
{
    ESP_LOGI(TAG, "Initializing %d buttons...", BUTTON_COUNT);

    for (int i = 0; i < BUTTON_COUNT; i++)
    {
        gpio_config_t io_conf = {};
        io_conf.pin_bit_mask = (1ULL << _buttons[i].pin);
        io_conf.mode = GPIO_MODE_INPUT;
        io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
        io_conf.intr_type = GPIO_INTR_DISABLE;

        ESP_ERROR_CHECK(gpio_config(&io_conf));
    }

    ESP_LOGI(TAG, "Gamepad initialized successfully!");
    printf("\n");
}

void InputManager::updateButtons()
{
    TickType_t now = xTaskGetTickCount();

    for (int i = 0; i < BUTTON_COUNT; i++)
    {
        Button& btn = _buttons[i];

        int raw = gpio_get_level(btn.pin);

        if (raw != btn.raw_state)
        {
            btn.raw_state = raw;
            btn.last_change_tick = now;
            continue;
        }

        if (raw != btn.stable_state && (now - btn.last_change_tick) >= DEBOUNCE_TICKS)
        {
            btn.stable_state = raw;

            if (btn.stable_state == 0)
            {
                ESP_LOGI(TAG, ">>> BUTTON [%s] PRESSED! <<<", _buttons[i].name);

                if (btn.on_press != nullptr)
                {
                    btn.on_press();
                }
            }
            else
            {
                ESP_LOGI(TAG, ">>> BUTTON [%s] RELEASED! <<<", _buttons[i].name);
            }
        }
    }
}
