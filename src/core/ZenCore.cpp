#include "core/ZenCore.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_chip_info.h"

#include "Buzzer.h"
#include "InputManager.h"
#include "drivers/DisplayDriver.h"
#include "BoardConfig.h"

static const char *TAG = "ZEN_OS";

ZenCore::~ZenCore()
{
    delete _buzzer;
    delete _inputManager;
    delete _display;
}

void ZenCore::init()
{
    startBootTimer();

    ESP_LOGI(TAG, "============================");
    ESP_LOGI(TAG, "    LILKA ZEN OS BOOTING    ");
    ESP_LOGI(TAG, "============================");
    printf("\n");

    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    ESP_LOGI(TAG, "Chip model: ESP32-S3, Cores: %d", chip_info.cores);
    ESP_LOGI(TAG, "Silicon revision: %d", chip_info.revision);
    printf("\n");

    _buzzer = new Buzzer();
    _buzzer->init();

    _inputManager = new InputManager();
    _inputManager->init();

    _display = new DisplayDriver(BoardConfig::DISPLAY_CONFIG);
    _display->init();
    _display->fillScreen(0x07E0);

    bindButtonsInInputManager();
}

void ZenCore::update()
{
    if (_inputManager)
    {
        _inputManager->update();
    }
}

void ZenCore::startBootTimer()
{
    for (int i = 5; i > 0; i--)
    {
        ESP_LOGI(TAG, "Starting in %d sec...", i);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    printf("\n");
}

void ZenCore::bindButtonsInInputManager()
{
    if (_inputManager)
    {
        _inputManager->bindButton(ButtonID::A, [this]() {
            if (this->_buzzer) this->_buzzer->makeSound();
        });

        _inputManager->bindButton(ButtonID::B, [this]() {
            if (this->_buzzer) this->_buzzer->mute();
        });
    }
}
