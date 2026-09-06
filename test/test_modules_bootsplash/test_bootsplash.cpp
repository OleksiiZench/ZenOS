#pragma once

#include <unity.h>
#include "modules/BootSplash.h"
#include "freertos/task.h"

void setUp()
{
    mock_tick_count = 0;
}

void tearDown(void)
{
}

void test_bootsplash_advances_time_correctly(void)
{
    BootSplash splash(3);
    splash.init();

    TEST_ASSERT_EQUAL(3000, mock_tick_count);
}

void test_bootsplash_zero_delay_does_not_advance_time(void)
{
    BootSplash splash(0);
    splash.init();

    TEST_ASSERT_EQUAL(0, mock_tick_count);
}

int main(int argc, char** argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_bootsplash_advances_time_correctly);
    RUN_TEST(test_bootsplash_zero_delay_does_not_advance_time);

    return UNITY_END();
}
