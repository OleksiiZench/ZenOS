#pragma once

#include "core/IModule.h"

class BootSplash : public IModule
{
public:
    explicit BootSplash(int delaySeconds);

    virtual void init() override;

private:
    int _delaySeconds;

    void startBootTimer();
};
