#pragma once

#include "driver/i2c.h"

#include "drivers/OLEDConfig.h"

class OLEDDriver
{
public:
    static bool isDeviceAttached(const OLEDConfig& config);
};
