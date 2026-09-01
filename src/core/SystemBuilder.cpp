#include "core/SystemBuilder.h"

#include <memory>

#include "drivers/Buzzer.h"
#include "modules/InputManager.h"
#include "drivers/DisplayDriver.h"
#include "boards/BoardConfig.h"
#include "modules/BootSplash.h"

void SystemBuilder::buildLilka(ZenCore& zenCore)
{
    std::unique_ptr<BootSplash> bootSplash = std::make_unique<BootSplash>(5);
    zenCore.registerModule(std::move(bootSplash));

    std::unique_ptr<Buzzer> buzzer = std::make_unique<Buzzer>(BoardConfig::PIN_BUZZER);
    Buzzer* buzzerPtr = buzzer.get();
    zenCore.registerModule(std::move(buzzer));

    std::unique_ptr<InputManager> inputManager = std::make_unique<InputManager>();
    if (inputManager)
    {
        inputManager->bindButton(ButtonID::A, [buzzerPtr]() {
            if (buzzerPtr) buzzerPtr->makeSound();
        });

        inputManager->bindButton(ButtonID::B, [buzzerPtr]() {
            if (buzzerPtr) buzzerPtr->stop();
        });
    }
    zenCore.registerModule(std::move(inputManager));

    std::unique_ptr<DisplayDriver> display = std::make_unique<DisplayDriver>(BoardConfig::DISPLAY_CONFIG);

    zenCore.registerModule(std::move(display));
}
