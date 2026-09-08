#pragma once

#include "driver/i2c_master.h"

#include "core/IModule.h"
#include "drivers/OLEDConfig.h"

class OLEDDriver : public IModule
{
public:
    OLEDDriver(const OLEDConfig& config);

    virtual void init() override;
    virtual void update() override;

private:
    bool isDeviceAttached(const OLEDConfig& config);

    OLEDConfig _config;
    bool _is_active;
};
