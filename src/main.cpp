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
    
    void (*on_press)();
};

void make_sound_buzzer()
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 127);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

void mute_buzzer()
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

Button buttons[] = {
    {GPIO_NUM_38, "UP",     1, nullptr},
    {GPIO_NUM_41, "DOWN",   1, nullptr},
    {GPIO_NUM_39, "LEFT",   1, nullptr},
    {GPIO_NUM_40, "RIGHT",  1, nullptr},
    {GPIO_NUM_5,  "A",      1, make_sound_buzzer},
    {GPIO_NUM_6,  "B",      1, mute_buzzer},
    {GPIO_NUM_10, "C",      1, nullptr},
    {GPIO_NUM_9,  "D",      1, nullptr},
    {GPIO_NUM_0,  "SELECT", 1, nullptr},
    {GPIO_NUM_4,  "START",  1, nullptr}
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

            if (buttons[i].on_press != nullptr)
            {
                buttons[i].on_press();
            }
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
    // Примусово робимо пін виходом і подаємо 0V (GND)
    // Це має "заспокоїти" пін перед тим, як LEDC візьме його під контроль
    gpio_reset_pin(GPIO_NUM_11);
    gpio_set_direction(GPIO_NUM_11, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_11, 0);

    ledc_timer_config_t ledc_timer = {};
    ledc_timer.speed_mode       = LEDC_LOW_SPEED_MODE;
    ledc_timer.duty_resolution  = LEDC_TIMER_8_BIT;
    ledc_timer.timer_num        = LEDC_TIMER_0;
    ledc_timer.freq_hz          = 1000;
    ledc_timer.clk_cfg          = LEDC_AUTO_CLK;
    
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {};
    ledc_channel.gpio_num       = GPIO_NUM_11;
    ledc_channel.speed_mode     = LEDC_LOW_SPEED_MODE;
    ledc_channel.channel        = LEDC_CHANNEL_0;
    ledc_channel.timer_sel      = LEDC_TIMER_0;
    ledc_channel.intr_type      = LEDC_INTR_DISABLE;
    ledc_channel.duty           = 0;
    ledc_channel.hpoint         = 0;
    
    ledc_channel_config(&ledc_channel);
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
