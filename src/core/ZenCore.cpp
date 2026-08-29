#include "core/ZenCore.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_chip_info.h"

static const char *TAG = "ZEN_OS";

void ZenCore::registerModule(std::unique_ptr<IModule> module)
{
    _modules.push_back(std::move(module));
}

void ZenCore::init()
{
    logSystemInfo();

    initAllModules();
}

void ZenCore::update()
{
    updateAllModules();
}

void ZenCore::initAllModules()
{
    for (std::unique_ptr<IModule>& module : _modules)
    {
        module->init();
    }
}

void ZenCore::updateAllModules()
{
    for (std::unique_ptr<IModule>& module : _modules)
    {
        module->update();
    }
}

void ZenCore::logSystemInfo()
{
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    ESP_LOGI(TAG, "Chip model: ESP32-S3, Cores: %d", chip_info.cores);
    ESP_LOGI(TAG, "Silicon revision: %d", chip_info.revision);
    printf("\n");
}
