#include <unity.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "drivers/DisplayConfig.h"

void setUp(void)
{
}
void tearDown(void)
{
}

void test_dma_memory_allocation(void)
{
    size_t chunk_bytes = 240 * 20 * 2; // 9600 байт
    void *buffer = heap_caps_malloc(chunk_bytes, MALLOC_CAP_DMA);
    
    TEST_ASSERT_NOT_NULL(buffer);
    
    free(buffer);
}

void test_spi_bus_initialization(void)
{
    spi_bus_config_t buscfg = {};
    buscfg.mosi_io_num = 3; // Твій пін MOSI
    buscfg.miso_io_num = GPIO_NUM_NC;
    buscfg.sclk_io_num = 4; // Твій пін CLK
    buscfg.quadwp_io_num = GPIO_NUM_NC;
    buscfg.quadhd_io_num = GPIO_NUM_NC;
    buscfg.max_transfer_sz = 9600 + 8;

    esp_err_t ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    TEST_ASSERT_EQUAL(0, ret);
    
    spi_bus_free(SPI2_HOST);
}

extern "C" void app_main()
{
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    UNITY_BEGIN();

    RUN_TEST(test_dma_memory_allocation);
    RUN_TEST(test_spi_bus_initialization);

    UNITY_END();
}
