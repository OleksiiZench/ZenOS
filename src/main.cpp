#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_chip_info.h"

#include "Buzzer.h"
#include "InputManager.h"

void startBootTimer();
void bindButtonsInInputManager();
void inputButtonA();
void inputButtonB();

static const char *TAG = "ZEN_OS";

Buzzer *buzzer = nullptr;
InputManager *inputManager = nullptr;

extern "C" void app_main(void)
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

    buzzer = new Buzzer();

    inputManager = new InputManager();
    
    bindButtonsInInputManager();

    while (1)
    {// Main loop
        if (inputManager)
        {
            inputManager->updateButtons();
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}




void startBootTimer()
{
    for (int i = 5; i > 0; i--)
    {
        ESP_LOGI(TAG, "Starting in %d sec...", i);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    printf("\n");
}

void bindButtonsInInputManager()
{
    if (inputManager)
    {
        inputManager->bindButton(ButtonID::A, inputButtonA);
        inputManager->bindButton(ButtonID::B, inputButtonB);
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
