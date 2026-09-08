#include "drivers/OLEDDriver.h"

#include "freertos/FreeRTOS.h"
#include "esp_log.h"

static const char* TAG = "OLEDDriver";

OLEDDriver::OLEDDriver(const OLEDConfig& config)
{
    _config = config;
    _is_active = false;
}

void OLEDDriver::init()
{
    if (isDeviceAttached(_config))
    {
        _is_active = true;

        // TODO: implement initializatoin logic for SSD1306 itself
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
    
    // TODO: logic for filling display
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