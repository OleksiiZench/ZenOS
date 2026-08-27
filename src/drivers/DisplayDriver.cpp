#include "drivers/DisplayDriver.h"

#include "esp_heap_caps.h"
#include <freertos/FreeRTOS.h>
#include "freertos/task.h"

DisplayDriver::DisplayDriver(const DisplayConfig& config)
{
    _config = config;
}

void DisplayDriver::init()
{
    initSPI();
    initController();
    turnOnBacklight();

    fillScreen(_config.default_bg_color);
}

void DisplayDriver::fillScreen(uint16_t color)
{
    setAddrWindow(0, 0, _config.width - 1, _config.height - 1);

    // Memory allocation
    const size_t chunk_pixels = _config.width * ROWS_PER_CHUNK;
    const size_t chunk_bytes = chunk_pixels * 2;
    uint8_t *buffer = (uint8_t *)heap_caps_malloc(chunk_bytes, MALLOC_CAP_DMA);

    // Filling the buffer with colour
    uint8_t color_hi = color >> 8;
    uint8_t color_lo = color & 0xFF;

    for (size_t i = 0; i < chunk_bytes; i += 2)
    {
        buffer[i] = color_hi;
        buffer[i + 1] = color_lo;
    }

    // Sending chunks
    int total_chunks = _config.height / ROWS_PER_CHUNK;

    for (int i = 0; i < total_chunks; i++)
    {
        sendData(buffer, chunk_bytes);
    }

    free(buffer);
}

void DisplayDriver::initSPI()
{
    // DC pin settings
    gpio_reset_pin(_config.pin_dc);
    gpio_set_direction(_config.pin_dc, GPIO_MODE_OUTPUT);

    // SPI bus configuration
    spi_bus_config_t buscfg = {};
    buscfg.mosi_io_num = _config.pin_mosi;
    buscfg.miso_io_num = GPIO_NUM_NC;
    buscfg.sclk_io_num = _config.pin_clk;
    buscfg.quadwp_io_num = GPIO_NUM_NC;
    buscfg.quadhd_io_num = GPIO_NUM_NC;
    buscfg.max_transfer_sz = _config.width * ROWS_PER_CHUNK * 2 + 8; // Size of chunk

    spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);

    // Connecting the device to the bus
    spi_device_interface_config_t devcfg = {};
    devcfg.clock_speed_hz = 40 * 1000 * 1000; // 40 Mhz
    devcfg.mode = 0; // SPI mode 0
    devcfg.spics_io_num = _config.pin_cs;
    devcfg.queue_size = 7;

    spi_bus_add_device(SPI2_HOST, &devcfg, &_spi);
}

void DisplayDriver::initController()
{
    sendCmd(0x01);
    vTaskDelay(pdMS_TO_TICKS(150));

    sendCmd(0x11);
    vTaskDelay(pdMS_TO_TICKS(50));

    sendCmd(0x3A);
    uint8_t colmod = 0x55;
    sendData(&colmod, 1);

    sendCmd(0x36);
    uint8_t madctl = 0x00;
    sendData(&madctl, 1);

    sendCmd(0x21);
    vTaskDelay(pdMS_TO_TICKS(10));

    sendCmd(0x29);
    vTaskDelay(pdMS_TO_TICKS(50));
}

void DisplayDriver::turnOnBacklight()
{
    if (_config.pin_backlight == GPIO_NUM_NC)
        return;

    gpio_reset_pin(_config.pin_backlight);
    gpio_set_direction(_config.pin_backlight, GPIO_MODE_OUTPUT);
    gpio_set_level(_config.pin_backlight, 1);
}

void DisplayDriver::setAddrWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    y1 += 20;
    y2 += 20;

    sendCmd(0x2A);
    uint8_t caset[4] = {
        static_cast<uint8_t>(x1 >> 8), static_cast<uint8_t>(x1 & 0xFF),
        static_cast<uint8_t>(x2 >> 8), static_cast<uint8_t>(x2 & 0xFF)
    };
    sendData(caset, 4);

    sendCmd(0x2B);
    uint8_t raset[4] = {
        static_cast<uint8_t>(y1 >> 8), static_cast<uint8_t>(y1 & 0xFF),
        static_cast<uint8_t>(y2 >> 8), static_cast<uint8_t>(y2 & 0xFF)
    };
    sendData(raset, 4);

    sendCmd(0x2C);
}

void DisplayDriver::sendCmd(uint8_t cmd)
{
    gpio_set_level(_config.pin_dc, 0); // DC 0 - for commands
    spi_transaction_t t = {};
    t.length = 8; // 8 bit
    t.tx_buffer = &cmd;
    spi_device_polling_transmit(_spi, &t);
}

void DisplayDriver::sendData(const uint8_t *data, size_t len)
{
    if (len == 0)
        return;

    gpio_set_level(_config.pin_dc, 1); // DC 1 - for data
    spi_transaction_t t = {};
    t.length = len * 8; // Length in bits
    t.tx_buffer = data;
    spi_device_polling_transmit(_spi, &t);
}
