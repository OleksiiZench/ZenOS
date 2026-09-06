#pragma once

#include <stdint.h>
#include <stddef.h>

#define SPI2_HOST 1
#define SPI_DMA_CH_AUTO 1

typedef void* spi_device_handle_t;

typedef struct
{
    int mosi_io_num;
    int miso_io_num;
    int sclk_io_num;
    int quadwp_io_num;
    int quadhd_io_num;
    int max_transfer_sz;
} spi_bus_config_t;

typedef struct
{
    int clock_speed_hz;
    int mode;
    int spics_io_num;
    int queue_size;
} spi_device_interface_config_t;

typedef struct
{
    size_t length;
    const void* tx_buffer;
} spi_transaction_t;

inline int spi_bus_initialize(int host_id, const spi_bus_config_t* bus_config, int dma_chan)
{
    return 0;
}

inline int spi_bus_add_device(int hist_id, const spi_device_interface_config_t* dev_config, spi_device_handle_t* handle)
{
    return 0;
}

inline uint8_t mock_last_spi_cmd = 0;
inline size_t mock_spi_data_bytes_sent = 0;

inline void reset_mock_spi()
{
    mock_last_spi_cmd = 0;
    mock_spi_data_bytes_sent = 0;
}

inline int spi_device_polling_transmit(spi_device_handle_t handle, spi_transaction_t* trans_desc)
{
    if (trans_desc->tx_buffer && trans_desc->length == 8)
    {
        mock_last_spi_cmd = *((uint8_t*)trans_desc->tx_buffer);
    }

    return 0;
}

inline int spi_device_transmit(spi_device_handle_t handle, spi_transaction_t* trans_desc)
{
    mock_spi_data_bytes_sent += (trans_desc->length / 8);

    return 0;
}
