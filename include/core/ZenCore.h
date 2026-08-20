#pragma once

#include <vector>

class IModule;

class ZenCore
{
public:
    ~ZenCore();

    void registerModule(IModule* module);

    void init();
    void update();

private:
    std::vector<IModule*> _modules;

    void initAllModules();
    void updateAllModules();

    void startBootTimer();
};
