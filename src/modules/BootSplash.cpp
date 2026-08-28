#include "modules/BootSplash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char* TAG = "BootSplash";

BootSplash::BootSplash(int delaySeconds)
{
    _delaySeconds = delaySeconds;
}

void BootSplash::init()
{
    startBootTimer();

    ESP_LOGI(TAG, "============================");
    ESP_LOGI(TAG, "    LILKA ZEN OS BOOTING    ");
    ESP_LOGI(TAG, "============================");
    printf("\n");
}

void BootSplash::startBootTimer()
{
    for (int i = _delaySeconds; i > 0; i--)
    {
        ESP_LOGI(TAG, "Starting in %d sec...", i);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    printf("\n");
}
