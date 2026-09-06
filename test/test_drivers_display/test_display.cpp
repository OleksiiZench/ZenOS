#include <unity.h>
#include "drivers/DisplayDriver.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "freertos/task.h"
#include "drivers/ST7789Commands.h"

void setUp(void)
{
    reset_mock_gpio();
    reset_mock_spi();
    mock_tick_count = 0;
}

void tearDown(void)
{
}

void test_display_fillscreen_calculates_chunks_correctly()
{
    constexpr uint16_t kWidth = 240;
    constexpr uint16_t kHeight = 240;
    constexpr int kBytesPerPixel = 2;

    DisplayConfig config = {};
    config.width = kWidth;
    config.height = kHeight;
    config.pin_backlight = static_cast<gpio_num_t>(15);
    config.pin_dc = static_cast<gpio_num_t>(2);
    config.pin_mosi = static_cast<gpio_num_t>(3);
    config.pin_clk = static_cast<gpio_num_t>(4);
    config.pin_cs = static_cast<gpio_num_t>(5);
    config.default_bg_color = 0x0000;

    DisplayDriver display(config);

    bool result = display.fillScreen(0xF800);

    TEST_ASSERT_TRUE(result);

    constexpr int kChunkPixels = kWidth * DisplayDriver::ROWS_PER_CHUNK;
    constexpr int kChunkBytes = kChunkPixels * kBytesPerPixel;
    constexpr int kTotalChunk = kHeight / DisplayDriver::ROWS_PER_CHUNK;
    constexpr int kAddrWindowBytes    = 4 /* CASET */ + 4 /* RASET */;
    constexpr int kExpectedTotalBytes = kAddrWindowBytes + (kChunkBytes * kTotalChunk);

    TEST_ASSERT_EQUAL(kExpectedTotalBytes, mock_spi_data_bytes_sent);
}

void test_display_init_turns_on_backlight(void)
{
    DisplayConfig config = {};
    config.width = 240;
    config.height = 240;
    config.pin_backlight = static_cast<gpio_num_t>(15);
    config.default_bg_color = 0x0000;

    DisplayDriver display(config);
    display.init();

    TEST_ASSERT_EQUAL(1, mock_gpio_levels[config.pin_backlight]);
    TEST_ASSERT_EQUAL(ST7789Cmd::DISPON, mock_last_spi_cmd);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_display_fillscreen_calculates_chunks_correctly);
    RUN_TEST(test_display_init_turns_on_backlight);

    return UNITY_END();
}
