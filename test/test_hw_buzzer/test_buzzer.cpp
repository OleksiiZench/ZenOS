#include <unity.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

#include "boards/BoardConfig.h"
#include "drivers/Buzzer.h"

void setUp(void)
{
}

void tearDown(void)
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

void test_buzzer_initialization_succeeds_on_hardware(void)
{
    Buzzer buzzer(BoardConfig::PIN_BUZZER);

    buzzer.init();
}

void test_buzzer_hardware_frequency_updates(void)
{
    Buzzer buzzer(BoardConfig::PIN_BUZZER);
    buzzer.init();

    uint32_t target_freq = 440;

    buzzer.startTone(target_freq);
    vTaskDelay(pdMS_TO_TICKS(10));

    uint32_t actual_freq = ledc_get_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0);

    TEST_ASSERT_NOT_EQUAL(0, actual_freq);

    buzzer.mute();
    vTaskDelay(pdMS_TO_TICKS(10));

    uint32_t actual_duty = ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    TEST_ASSERT_EQUAL_MESSAGE(0, actual_duty, "Mute should set duty cycle to 0");
}

extern "C" void app_main()
{
    vTaskDelay(pdMS_TO_TICKS(2000));

    UNITY_BEGIN();

    RUN_TEST(test_buzzer_initialization_succeeds_on_hardware);
    RUN_TEST(test_buzzer_hardware_frequency_updates);

    UNITY_END();
}
