#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_chip_info.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

static const char *TAG = "ZEN_OS";

struct Button
{
    gpio_num_t pin;
    const char *name;
    int last_state;
};

Button buttons[] = {
    {GPIO_NUM_38, "UP",     1},
    {GPIO_NUM_41, "DOWN",   1},
    {GPIO_NUM_39, "LEFT",   1},
    {GPIO_NUM_40, "RIGHT",  1},
    {GPIO_NUM_5,  "A",      1},
    {GPIO_NUM_6,  "B",      1},
    {GPIO_NUM_10, "C",      1},
    {GPIO_NUM_9,  "D",      1},
    {GPIO_NUM_0,  "SELECT", 1},
    {GPIO_NUM_4,  "START",  1}
};

const int NUM_BUTTONS = sizeof(buttons) / sizeof(Button);

void setup_buttons()
{
    ESP_LOGI(TAG, "Initializing %d buttons...", NUM_BUTTONS);

    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        gpio_config_t io_conf = {};
        io_conf.pin_bit_mask = (1ULL << buttons[i].pin);
        io_conf.mode = GPIO_MODE_INPUT;
        io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
        io_conf.intr_type = GPIO_INTR_DISABLE;

        gpio_config(&io_conf);
    }

    ESP_LOGI(TAG, "Gamepad initialized successfully!");
}

void update_buttons()
{
    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        int current_state = gpio_get_level(buttons[i].pin);  // gpio_get_level повертає 1, якщо на піні 3.3V, і 0, якшо пін замкнутий на змелю

        if (current_state == 0 && buttons[i].last_state == 1)
        {
            ESP_LOGI(TAG, ">>> BUTTON [%s] PRESSED! <<<", buttons[i].name);
        }
        else if (current_state == 1 && buttons[i].last_state == 0)
        {
            ESP_LOGI(TAG, ">>> BUTTON [%s] RELEASED! <<<", buttons[i].name);
        }

        buttons[i].last_state = current_state;
    }
   
}

void setup_buzzer()
{
    ledc_timer_config_t ledc_timer_config_buzzer = { LEDC_LOW_SPEED_MODE, LEDC_TIMER_8_BIT, LEDC_TIMER_0, 1000, LEDC_USE_RC_FAST_CLK, false };
    ledc_timer_config(&ledc_timer_config_buzzer);

    ledc_channel_config_t ledc_channel_config_buzzer = { GPIO_NUM_11, LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, LEDC_INTR_DISABLE, LEDC_TIMER_0, 2, 0, LEDC_SLEEP_MODE_KEEP_ALIVE, 1 };
    ledc_channel_config(&ledc_channel_config_buzzer);
}

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

    setup_buttons();
    setup_buzzer();

    while (1)
    {// Головний цикл
        update_buttons();

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
