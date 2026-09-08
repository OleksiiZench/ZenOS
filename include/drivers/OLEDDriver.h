#pragma once

#include "driver/i2c_master.h"

#include "core/IModule.h"
#include "drivers/OLEDConfig.h"

class OLEDDriver : public IModule
{
public:
    OLEDDriver(const OLEDConfig& config);
    ~OLEDDriver() override;

    virtual void init() override;
    virtual void update() override;

    void clear(bool white = false);
    void drawPixel(int x, int y, bool white = true);

    void drawLilkaLogo();

private:
    bool isDeviceAttached(const OLEDConfig& config);

    void sendCommand(uint8_t cmd);

    void setupI2CBus();
    void sendInitSequence();

    void drawScaledPixel(int x, int y, int scale);

    OLEDConfig _config;
    bool _is_active;

    i2c_master_bus_handle_t _bus_handle;
    i2c_master_dev_handle_t _dev_handle;

    uint8_t _buffer[1025];
};
