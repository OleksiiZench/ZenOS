#include <unity.h>
#include "drivers/Buzzer.h"
#include "driver/ledc.h"
#include "freertos/task.h"

void setUp(void)
{
    mock_tick_count = 0;
    mock_ledc_freq = 0;
    mock_ledc_duty = 0;
}

void tearDown(void)
{

}

void test_buzzer_plays_first_note_immediately(void)
{
    Buzzer buzzer(static_cast<gpio_num_t>(5));
    buzzer.init();

    Note notes[] = { { 440, 100 } };
    Melody melody = { notes, 1, 20 };

    buzzer.playMelody(melody);

    // CHECK: The frequency should be 440, and PWM should be enabled (duty > 0)
    TEST_ASSERT_EQUAL(440, mock_ledc_freq);
    TEST_ASSERT_EQUAL(127, mock_ledc_duty);
}

void test_buzzer_state_machine_transitions(void)
{
    Buzzer buzzer(static_cast<gpio_num_t>(5));
    buzzer.init();

    // Melody: 440 Hz note (100 ms) -> Pause (20 ms) -> 880 Hz note (100 ms)
    Note notes[] = { { 440, 100 }, { 880, 100 } };
    Melody melody = { notes, 2, 20 };

    buzzer.playMelody(melody);
    TEST_ASSERT_EQUAL(440, mock_ledc_freq);

    // 1. Fast-forward 99 ms (note should still be playing)
    mock_tick_count = 99;
    buzzer.update();
    TEST_ASSERT_EQUAL(127, mock_ledc_duty);

    // 2. Fast_forward to 100 ms (PAUSE should begin)
    mock_tick_count = 100;
    buzzer.update();
    TEST_ASSERT_EQUAL(0, mock_ledc_duty);

    // 3. Fast-forward another 19 ms of pause
    mock_tick_count = 119;
    buzzer.update();
    TEST_ASSERT_EQUAL(0, mock_ledc_duty);

    // 4. Fast-forward to end of pause - SECOND note should begin
    mock_tick_count = 120;
    buzzer.update();
    TEST_ASSERT_EQUAL(880, mock_ledc_freq);
    TEST_ASSERT_EQUAL(127, mock_ledc_duty);
}

int main (int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_buzzer_plays_first_note_immediately);
    RUN_TEST(test_buzzer_state_machine_transitions);

    return UNITY_END();
}
