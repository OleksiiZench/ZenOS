#include "core/SystemBuilder.h"

#include "drivers/Buzzer.h"
#include "modules/InputManager.h"
#include "drivers/DisplayDriver.h"
#include "boards/BoardConfig.h"
#include "modules/BootSplash.h"

void SystemBuilder::buildLilka(ZenCore& zenCore)
{
    BootSplash* bootSplash = new BootSplash(5);
    zenCore.registerModule(bootSplash);

    Buzzer* buzzer = new Buzzer;
    zenCore.registerModule(buzzer);

    InputManager* inputManager = new InputManager;
    if (inputManager)
    {
        inputManager->bindButton(ButtonID::A, [buzzer]() {
            if (buzzer) buzzer->makeSound();
        });

        inputManager->bindButton(ButtonID::B, [buzzer]() {
            if (buzzer) buzzer->mute();
        });
    }
    zenCore.registerModule(inputManager);

    DisplayDriver* display = new DisplayDriver(BoardConfig::DISPLAY_CONFIG);

    zenCore.registerModule(display);
}
