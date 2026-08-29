#include "core/SystemBuilder.h"

#include <memory>

#include "drivers/Buzzer.h"
#include "modules/InputManager.h"
#include "drivers/DisplayDriver.h"
#include "boards/BoardConfig.h"
#include "modules/BootSplash.h"

void SystemBuilder::buildLilka(ZenCore& zenCore)
{
    auto bootSplash = std::make_unique<BootSplash>(5);
    zenCore.registerModule(std::move(bootSplash));

    auto buzzer = std::make_unique<Buzzer>();
    Buzzer* buzzerPtr = buzzer.get();
    zenCore.registerModule(std::move(buzzer));

    auto inputManager = std::make_unique<InputManager>();
    if (inputManager)
    {
        inputManager->bindButton(ButtonID::A, [buzzerPtr]() {
            if (buzzerPtr) buzzerPtr->makeSound();
        });

        inputManager->bindButton(ButtonID::B, [buzzerPtr]() {
            if (buzzerPtr) buzzerPtr->mute();
        });
    }
    zenCore.registerModule(std::move(inputManager));

    auto display = std::make_unique<DisplayDriver>(BoardConfig::DISPLAY_CONFIG);

    zenCore.registerModule(std::move(display));
}
