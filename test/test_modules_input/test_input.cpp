#include <unity.h>
#include "modules/InputManager.h"
#include "driver/gpio.h"
#include "freertos/task.h"

bool button_a_pressed = false;

void on_button_a_press()
{
    button_a_pressed = true;
}

void setUp(void)
{
    mock_tick_count = 0;
    reset_mock_gpio();
    button_a_pressed = false;
}

void tearDown(void)
{
}

void test_input_debouncing_successful_press(void)
{
    InputManager input;
    input.init();
    input.bindButton(ButtonID::A, on_button_a_press);

    input.update();
    TEST_ASSERT_FALSE(button_a_pressed);

    mock_gpio_levels[GPIO_NUM_5] = 0;
    input.update();
    TEST_ASSERT_FALSE(button_a_pressed);

    mock_tick_count = 10;
    input.update();
    TEST_ASSERT_FALSE(button_a_pressed);

    mock_tick_count = 20;
    input.update();
    TEST_ASSERT_TRUE(button_a_pressed);
}

void test_input_ignores_bouncing(void)
{
    InputManager input;
    input.init();
    input.bindButton(ButtonID::A, on_button_a_press);

    mock_gpio_levels[GPIO_NUM_5] = 0;
    input.update();

    mock_tick_count = 5;
    mock_gpio_levels[GPIO_NUM_5] = 1;
    input.update();

    mock_tick_count = 10;
    mock_gpio_levels[GPIO_NUM_5] = 0;
    input.update();

    mock_tick_count = 25;
    input.update();
    TEST_ASSERT_FALSE(button_a_pressed);

    mock_tick_count = 30;
    input.update();
    TEST_ASSERT_TRUE(button_a_pressed);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_input_debouncing_successful_press);
    RUN_TEST(test_input_ignores_bouncing);

    return UNITY_END();
}
