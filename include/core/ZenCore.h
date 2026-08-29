#pragma once

#include <vector>
#include <memory>

#include "core/IModule.h"

class ZenCore
{
public:
    void registerModule(std::unique_ptr<IModule> module);

    void init();
    void update();

private:
    std::vector<std::unique_ptr<IModule>> _modules;

    void initAllModules();
    void updateAllModules();

    void logSystemInfo();
};
