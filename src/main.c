#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_chip_info.h"
#include "driver/gpio.h"

static const char *TAG = "ZEN_OS";

#define BUTTON_PIN GPIO_NUM_5

void app_main(void)
{
    vTaskDelay(pdMS_TO_TICKS(2500));

    ESP_LOGI(TAG, "============================");
    ESP_LOGI(TAG, "    LILKA ZEN OS BOOTING    ");
    ESP_LOGI(TAG, "============================");

    // 1. Відображення інформації про пристрій
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    ESP_LOGI(TAG, "Chip model: ESP32-S3, Cores: %d", chip_info.cores);
    ESP_LOGI(TAG, "Silicon revision: %d", chip_info.revision);

    // 2. Налаштування піна (GPIO) - кнопка SELECT
    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = (1ULL << BUTTON_PIN);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.intr_type = GPIO_INTR_DISABLE;

    gpio_config(&io_conf);  // Застосовуємо налаштування

    ESP_LOGI(TAG, "Button driver initialized on GPIO %d", BUTTON_PIN);

    int last_state = 1;

    while (1)
    {// Головний цикл

        // 3.Читтання стану
        int current_state = gpio_get_level(BUTTON_PIN);  // gpio_get_level повертає 1, якщо на піні 3.3V, і 0, якшо пін замкнутий на змелю

        if (current_state == 0 && last_state == 1)
        {
            ESP_LOGI(TAG, ">>> SELECT BUTTON PRESSED! <<<");
        }
        else if (current_state == 1 && last_state == 0)
        {
            ESP_LOGI(TAG, ">>> SELECT BUTTON RELEASED! <<<");
        }

        last_state = current_state;

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
