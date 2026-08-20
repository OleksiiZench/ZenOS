#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "core/ZenCore.h"
#include "core/SystemBuilder.h"

void setup();
void loop();

ZenCore _zenCore;

extern "C" void app_main(void)
{
    setup();

    while (1)
    {
        loop();
    }
}




void setup()
{
    SystemBuilder::buildLilka(_zenCore);

    _zenCore.init();
}

void loop()
{
    _zenCore.update();
    
    vTaskDelay(pdMS_TO_TICKS(50));
}
