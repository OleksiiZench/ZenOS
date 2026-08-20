#include "core/SystemBuilder.h"

#include "Buzzer.h"
#include "InputManager.h"
#include "drivers/DisplayDriver.h"
#include "BoardConfig.h"

void SystemBuilder::buildLilka(ZenCore& zenCore)
{
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
    display->fillScreen(0x07E0);

    zenCore.registerModule(display);
}
