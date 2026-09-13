#pragma once

#include <stdint.h>
#include <stddef.h>

typedef int esp_err_t;
#define ESP_OK 0

typedef int i2c_port_t;

#define I2C_NUM_0 0
#define I2C_NUM_1 1

#define I2C_CLK_SRC_DEFAULT 0
#define I2C_ADDR_BIT_LEN_7 0

typedef void* i2c_master_bus_handle_t;
typedef void* i2c_master_dev_handle_t;

typedef struct
{
    int i2c_port;
    int sda_io_num;
    int scl_io_num;
    int clk_source;
    size_t glitch_ignore_cnt;
    struct
    {
        uint32_t enable_internal_pullup : 1;
    } flags;
} i2c_master_bus_config_t;

typedef struct
{
    uint16_t device_address;
    uint32_t scl_speed_hz;
    int dev_addr_length;
} i2c_device_config_t;

inline esp_err_t i2c_new_master_bus(const i2c_master_bus_config_t* bus_config, i2c_master_bus_handle_t* bus_handle)
{
    return ESP_OK;
}

inline esp_err_t i2c_master_bus_add_device(i2c_master_bus_handle_t bus_handle, const i2c_device_config_t* dev_config, i2c_master_dev_handle_t* dev_handle)
{
    return ESP_OK;
}

inline esp_err_t i2c_master_probe(i2c_master_bus_handle_t bus_handle, uint16_t address, int timeout_ms)
{
    return ESP_OK;
}

inline esp_err_t i2c_master_transmit(i2c_master_dev_handle_t dev_handle, const uint8_t* data, size_t data_size, int timeout_ms)
{
    return ESP_OK;
}

inline esp_err_t i2c_master_bus_rm_device(i2c_master_dev_handle_t dev_handle)
{
    return ESP_OK;
}

inline esp_err_t i2c_del_master_bus(i2c_master_bus_handle_t bus_handle)
{
    return ESP_OK;
}
