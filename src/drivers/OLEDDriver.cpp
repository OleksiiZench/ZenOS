#include "drivers/OLEDDriver.h"

#include "freertos/FreeRTOS.h"
#include "esp_log.h"

static const char* TAG = "OLEDDriver";

OLEDDriver::OLEDDriver(const OLEDConfig& config)
{
    _config = config;
    _is_active = false;
    _is_screen_on = true;
    _bus_handle = nullptr;
    _dev_handle = nullptr;

    _buffer[0] = 0x40;
    clear();
}

OLEDDriver::~OLEDDriver()
{
    if (_dev_handle)
        i2c_master_bus_rm_device(_dev_handle);

    if (_bus_handle)
        i2c_del_master_bus(_bus_handle);
}

void OLEDDriver::init()
{
    if (isDeviceAttached(_config))
    {
        _is_active = true;

        setupI2CBus();
        sendInitSequence();
        drawLilkaLogo();
    }
    else
    {
        _is_active = false;
    }
}

void OLEDDriver::update()
{
    if (!_is_active)
        return;
    
    i2c_master_transmit(_dev_handle, _buffer, sizeof(_buffer), -1);
}

void OLEDDriver::clear(bool white)
{
    memset(&_buffer[1], white ? 0xFF : 0x00, sizeof(_buffer) - 1);
}

void OLEDDriver::drawPixel(int x, int y, bool white)
{
    if (x < 0 || x >= _config.width || y < 0 || y >= _config.height)
        return;
    
    int index = 1 + (x + (y / 8) * _config.width);

    if (white)
    {
        _buffer[index] |= (1 << (y % 8));
    }
    else
    {
        _buffer[index] &= ~(1 << (y % 8));
    }
}

void OLEDDriver::drawLilkaLogo()
{
    const uint8_t font[][5] = {
        { 0x7F, 0x40, 0x40, 0x40, 0x40 }, // L
        { 0x00, 0x44, 0x7D, 0x40, 0x00 }, // i
        { 0x00, 0x41, 0x7F, 0x40, 0x00 }, // l
        { 0x7F, 0x08, 0x14, 0x22, 0x41 }, // k
        { 0x20, 0x54, 0x54, 0x54, 0x78 }, // a
        { 0x00, 0x00, 0x00, 0x00, 0x00 }, // (space)
        { 0x00, 0x56, 0x36, 0x00, 0x00 }, // ;
        { 0x00, 0x41, 0x22, 0x1C, 0x00 }  // )
    };

    constexpr int CHAR_WIDTH = 5;
    constexpr int CHAR_HEIGHT = 8;
    constexpr int CHAR_SPACING = 12;
    constexpr int PIXEL_SCALE = 2;

    int current_x = 18;
    int current_y = 24;

    // Використання range-based for loop робить код надійнішим (не залежить від розміру масиву)
    for (const auto& char_bitmap : font)
    {
        for (int col = 0; col < CHAR_WIDTH; col++)
        {
            uint8_t line = char_bitmap[col];

            for (int row = 0; row < CHAR_HEIGHT; row++)
            {
                if (line & (1 << row))
                {
                    drawScaledPixel(current_x + col * PIXEL_SCALE,
                                    current_y + row * PIXEL_SCALE,
                                    PIXEL_SCALE);
                }
            }
        }
        current_x += CHAR_SPACING;
    }
}

void OLEDDriver::toggleDisplay()
{
    if (!_is_active)
        return;

    _is_screen_on = !_is_screen_on;

    if (_is_screen_on)
    {
        sendCommand(0xAF); // Command: Display ON
    }
    else
    {
        sendCommand(0xAE); // Command: Display OFF (Sleep Mode)
    }
}

bool OLEDDriver::isDeviceAttached(const OLEDConfig& config)
{
    i2c_master_bus_config_t bus_config = {};
    bus_config.i2c_port = config.i2c_port;
    bus_config.sda_io_num = config.pin_sda;
    bus_config.scl_io_num = config.pin_scl;
    bus_config.clk_source = I2C_CLK_SRC_DEFAULT;
    bus_config.glitch_ignore_cnt = 7;
    bus_config.flags.enable_internal_pullup = 1;

    i2c_master_bus_handle_t bus_handle;
    esp_err_t ret = i2c_new_master_bus(&bus_config, &bus_handle);

    if (ret != ESP_OK)
        return false;

    esp_err_t probe_ret = i2c_master_probe(bus_handle, config.address, pdMS_TO_TICKS(100));

    i2c_del_master_bus(bus_handle);

    if (probe_ret == ESP_OK)
    {
        ESP_LOGI(TAG, "OLED Display detected at address 0x%02X!", config.address);
        printf("\n");
        return true;
    }
    else
    {
        ESP_LOGW(TAG, "OLED Display NOT found.");
        printf("\n");
        return false;
    }
}

void OLEDDriver::sendCommand(uint8_t cmd)
{
    uint8_t data[2] = {0x00, cmd};
    i2c_master_transmit(_dev_handle, data, 2, -1);
}

void OLEDDriver::setupI2CBus()
{
    i2c_master_bus_config_t bus_config = {};
    bus_config.i2c_port = _config.i2c_port;
    bus_config.sda_io_num = _config.pin_sda;
    bus_config.scl_io_num = _config.pin_scl;
    bus_config.clk_source = I2C_CLK_SRC_DEFAULT;
    bus_config.glitch_ignore_cnt = 7;
    bus_config.flags.enable_internal_pullup = 1;

    i2c_new_master_bus(&bus_config, &_bus_handle);

    i2c_device_config_t dev_config = {};
    dev_config.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    dev_config.device_address = _config.address;
    dev_config.scl_speed_hz = _config.clock_speed_hz;

    i2c_master_bus_add_device(_bus_handle, &dev_config, &_dev_handle);
}

void OLEDDriver::sendInitSequence()
{
    static const uint8_t init_sequence[] = {
        0xAE, 0x20, 0x00, 0xB0, 0xC8, 0x00, 0x10, 0x40,
        0x81, 0xFF, 0xA1, 0xA6, 0xA8, 0x3F, 0xA4, 0xD3,
        0x00, 0xD5, 0x80, 0xD9, 0x22, 0xDA, 0x12, 0xDB,
        0x20, 0x8D, 0x14, 0xAF
    };

    for (uint8_t cmd : init_sequence)
    {
        sendCommand(cmd);
    }
}

void OLEDDriver::drawScaledPixel(int x, int y, int scale)
{
    for (int dx = 0; dx < scale; dx++) 
    {
        for (int dy = 0; dy < scale; dy++) 
        {
            drawPixel(x + dx, y + dy);
        }
    }
}
