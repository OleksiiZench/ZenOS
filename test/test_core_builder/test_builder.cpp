#include <unity.h>
#include "core/ZenCore.h"
#include "core/SystemBuilder.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/spi_master.h"
#include "freertos/task.h"

void setUp(void)
{
    reset_mock_gpio();
    reset_mock_spi();
    mock_tick_count = 0;
    mock_ledc_duty = 0;
    mock_ledc_freq = 0;
}

void tearDown(void)
{
}

void test_system_builder_wiring_button_a_plays_melody(void)
{
    ZenCore core;

    SystemBuilder::buildLilka(core);

    core.init();

    TEST_ASSERT_EQUAL(0, mock_ledc_duty);

    mock_gpio_levels[GPIO_NUM_5] = 0;

    for (int i = 0; i <= 25; i++)
    {
        mock_tick_count = i;
        core.update();
    }

    TEST_ASSERT_EQUAL(127, mock_ledc_duty);
}

void test_system_builder_wiring_button_b_stops_melody(void)
{
    ZenCore core;
    SystemBuilder::buildLilka(core);
    core.init();

    mock_gpio_levels[GPIO_NUM_5] = 0;

    for (int i = 0; i <= 25; i++)
    {
        mock_tick_count++;
        core.update();
    }

    TEST_ASSERT_EQUAL(127, mock_ledc_duty);

    mock_gpio_levels[GPIO_NUM_6] = 0;

    for (int i = 0; i <= 25; i++)
    {
        mock_tick_count++;
        core.update();
    }

    TEST_ASSERT_EQUAL(0, mock_ledc_duty);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_system_builder_wiring_button_a_plays_melody);
    RUN_TEST(test_system_builder_wiring_button_b_stops_melody);

    return UNITY_END();
}
