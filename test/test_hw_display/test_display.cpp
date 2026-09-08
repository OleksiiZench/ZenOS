#include <unity.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "drivers/DisplayConfig.h"
#include "boards/BoardConfig.h"
#include "drivers/DisplayDriver.h"

void setUp(void)
{
}
void tearDown(void)
{
}

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

extern "C" void app_main()
{
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    UNITY_BEGIN();

    RUN_TEST(test_dma_memory_allocation);
    RUN_TEST(test_spi_bus_low_level_smoke_check);
    RUN_TEST(test_display_driver_init_succeeds_on_hardware);

    UNITY_END();
}
