#include <unity.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"
#include "driver/spi_master.h"
#include "driver/ledc.h"
#include "driver/gpio.h"

#include "boards/BoardConfig.h"
#include "drivers/DisplayDriver.h"
#include "drivers/Buzzer.h"
#include "modules/InputManager.h"
#include "drivers/Button.h"

static bool g_buzzer_was_initialized = false;

void setUp(void)
{
}

void tearDown(void)
{
    if (g_buzzer_was_initialized)
    {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

        g_buzzer_was_initialized = false;
    }
}

// ==========================================
//             TESTS DISPLAY (SPI)
// ==========================================
void test_dma_memory_allocation(void)
{
    DisplayConfig config = BoardConfig::DISPLAY_CONFIG;

    constexpr int kBytesPerPixel = 2;
    size_t chunk_bytes = config.width * DisplayDriver::ROWS_PER_CHUNK * kBytesPerPixel;

    void *buffer = heap_caps_malloc(chunk_bytes, MALLOC_CAP_DMA);
    
    TEST_ASSERT_NOT_NULL(buffer);
    
    heap_caps_free(buffer);
}

void test_spi_bus_low_level_smoke_check(void)
{
    DisplayConfig config = BoardConfig::DISPLAY_CONFIG;

    constexpr int kBytesPerPixel = 2;

    spi_bus_config_t buscfg = {};
    buscfg.mosi_io_num = config.pin_mosi;
    buscfg.miso_io_num = GPIO_NUM_NC;
    buscfg.sclk_io_num = config.pin_clk;
    buscfg.quadwp_io_num = GPIO_NUM_NC;
    buscfg.quadhd_io_num = GPIO_NUM_NC;
    buscfg.max_transfer_sz = (config.width * DisplayDriver::ROWS_PER_CHUNK * kBytesPerPixel) + 8;

    esp_err_t ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    TEST_ASSERT_EQUAL_MESSAGE(ESP_OK, ret, "SPI Bus init failed (check MOSI/CLK pins)");

    spi_device_interface_config_t devcfg = {};
    devcfg.clock_speed_hz = 40 * 1000 * 1000;
    devcfg.mode = 0;
    devcfg.spics_io_num = config.pin_cs;
    devcfg.queue_size = 7;

    spi_device_handle_t spi_handle;
    ret = spi_bus_add_device(SPI2_HOST, &devcfg, &spi_handle);
    TEST_ASSERT_EQUAL_MESSAGE(ESP_OK, ret, "SPI Device add failed (check CS pin)");

    spi_bus_remove_device(spi_handle);
    spi_bus_free(SPI2_HOST);
}

void test_display_driver_init_succeeds_on_hardware(void)
{
    DisplayConfig config = BoardConfig::DISPLAY_CONFIG;
    DisplayDriver display(config);

    display.init();

    TEST_ASSERT_EQUAL_MESSAGE(1, gpio_get_level(config.pin_backlight), "Backlight pin should be HIGH after init()");

    display.deinit();
}

// ==========================================
//             TESTS BUZZER (LEDC)
// ==========================================
void test_buzzer_initialization_succeeds_on_hardware(void)
{
    Buzzer buzzer(BoardConfig::PIN_BUZZER);

    buzzer.init();

    g_buzzer_was_initialized = true;
}

void test_buzzer_hardware_frequency_updates(void)
{
    Buzzer buzzer(BoardConfig::PIN_BUZZER);
    buzzer.init();

    g_buzzer_was_initialized = true;

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

// ==========================================
//             TESTS INPUT MANAGER (GPIO)
// ==========================================
void test_input_manager_simulated_button_press(void)
{
    InputManager input;
    input.init();

    bool a_button_pressed = false;

    input.bindButton(ButtonID::A, [&a_button_pressed]() {
        a_button_pressed = true;
    });

    gpio_set_direction(GPIO_NUM_5, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_5, 0);

    constexpr int kPollIntervalMs = 10;
    constexpr int kPollCount = (2 * InputManager::DEBOUNCE_TICKS * portTICK_PERIOD_MS) / kPollIntervalMs;

    for (int i = 0; i < kPollCount; i++)
    {
        input.update();
        vTaskDelay(pdMS_TO_TICKS(kPollIntervalMs));
    }

    gpio_set_direction(GPIO_NUM_5, GPIO_MODE_INPUT);

    TEST_ASSERT_TRUE_MESSAGE(a_button_pressed, "Callback for Button A should be triggered after simulated press");
}

void test_input_manager_pullup_resistors_active(void)
{
    InputManager input;
    input.init();

    const gpio_num_t button_pins[] = {
        GPIO_NUM_38, GPIO_NUM_41, GPIO_NUM_39, GPIO_NUM_40,
        GPIO_NUM_5,  GPIO_NUM_6,  GPIO_NUM_10, GPIO_NUM_9,
        GPIO_NUM_0,  GPIO_NUM_4
    };

    static_assert(std::size(button_pins) == InputManager::BUTTON_COUNT, "Pin list must match all button count");

    for (int i = 0; i < InputManager::BUTTON_COUNT; i++)
    {
        TEST_ASSERT_EQUAL_MESSAGE(1, gpio_get_level(button_pins[i]), "Button pin should be HIGH (unpressed) due to pull-up");
    }
}

// ==========================================
//             MAIN START
// ==========================================
extern "C" void app_main()
{
    vTaskDelay(pdMS_TO_TICKS(2000));

    UNITY_BEGIN();

    // Running display tests
    RUN_TEST(test_dma_memory_allocation);
    RUN_TEST(test_spi_bus_low_level_smoke_check);
    RUN_TEST(test_display_driver_init_succeeds_on_hardware);

    // Running buzzer tests
    RUN_TEST(test_buzzer_initialization_succeeds_on_hardware);
    RUN_TEST(test_buzzer_hardware_frequency_updates);

    // Running input manager tests
    RUN_TEST(test_input_manager_simulated_button_press);
    RUN_TEST(test_input_manager_pullup_resistors_active);

    UNITY_END();
}
