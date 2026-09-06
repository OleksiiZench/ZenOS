#pragma once

#include "driver/spi_master.h"
#include "esp_log.h"

#include "core/IModule.h"
#include "drivers/DisplayConfig.h"

class DisplayDriver : public IModule
{
public:
    DisplayDriver(const DisplayConfig& config);

    virtual void init() override;

    bool fillScreen(uint16_t color);

    static constexpr uint16_t ROWS_PER_CHUNK = 20;

private:
    DisplayConfig _config;
    spi_device_handle_t _spi = nullptr;

    void setupBacklightPin();
    void turnOffBacklight();
    void initSPI();
    void initController();
    void clearScreen();
    void turnOnDisplay();
    void turnOnBacklight();

    void setAddrWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

    void sendCmd(uint8_t cmd);
    void sendData(const uint8_t* data, size_t len);
};
