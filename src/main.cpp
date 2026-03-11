#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_chip_info.h"

#include "Buzzer.h"
#include "InputManager.h"
#include "EnumTypes.h"

void bindButtonsInInputManager();
void inputButtonA();
void inputButtonB();

static const char *TAG = "ZEN_OS";

Buzzer *buzzer = nullptr;
InputManager *inputManager = nullptr;

extern "C" void app_main(void)
{
    vTaskDelay(pdMS_TO_TICKS(2500));

    ESP_LOGI(TAG, "============================");
    ESP_LOGI(TAG, "    LILKA ZEN OS BOOTING    ");
    ESP_LOGI(TAG, "============================");

    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    ESP_LOGI(TAG, "Chip model: ESP32-S3, Cores: %d", chip_info.cores);
    ESP_LOGI(TAG, "Silicon revision: %d", chip_info.revision);

    buzzer = new Buzzer();

    inputManager = new InputManager();
    
    bindButtonsInInputManager();

    while (1)
    {// Головний цикл
        if (inputManager)
        {
            inputManager->updateButtons();
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void bindButtonsInInputManager()
{
    if (inputManager)
    {
        inputManager->bindButton(BTN_A, inputButtonA);
        inputManager->bindButton(BTN_B, inputButtonB);
    }
}

void inputButtonA()
{
    if (buzzer)
        buzzer->makeSound();
}

void inputButtonB()
{
    if (buzzer)
        buzzer->mute();
}
