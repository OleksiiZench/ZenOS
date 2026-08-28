#include "core/ZenCore.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_chip_info.h"

#include "core/IModule.h"

static const char *TAG = "ZEN_OS";

ZenCore::~ZenCore()
{
    for (IModule* module : _modules)
    {
        delete module;
    }
}

void ZenCore::registerModule(IModule* module)
{
    _modules.push_back(module);
}

void ZenCore::init()
{
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    ESP_LOGI(TAG, "Chip model: ESP32-S3, Cores: %d", chip_info.cores);
    ESP_LOGI(TAG, "Silicon revision: %d", chip_info.revision);
    printf("\n");

    initAllModules();
}

void ZenCore::update()
{
    updateAllModules();
}

void ZenCore::initAllModules()
{
    for (IModule* module : _modules)
    {
        module->init();
    }
}

void ZenCore::updateAllModules()
{
    for (IModule* module : _modules)
    {
        module->update();
    }
}
