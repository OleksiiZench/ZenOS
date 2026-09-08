#include "drivers/OLEDDriver.h"

#include "esp_log.h"

static const char* TAG = "OLEDDriver";

bool OLEDDriver::isDeviceAttached(const OLEDConfig& config)
{
    i2c_config_t conf = {};
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = config.pin_sda;
    conf.scl_io_num = config.pin_scl;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = config.clock_speed_hz;

    i2c_param_config(config.i2c_port, &conf);
    esp_err_t install_ret = i2c_driver_install(config.i2c_port, conf.mode, 0, 0, 0);

    if (install_ret != ESP_OK)
        return false;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (config.address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_stop(cmd);

    esp_err_t ping_ret = i2c_master_cmd_begin(config.i2c_port, cmd, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(cmd);

    i2c_driver_delete(config.i2c_port);

    if (ping_ret == ESP_OK)
    {
        ESP_LOGI(TAG, "OLED Display detected at address 0x%02X!", config.address);
        return true;
    }
    else
    {
        ESP_LOGW(TAG, "OLED Display NOT found.");
        return false;
    }
}