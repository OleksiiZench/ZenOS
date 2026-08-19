#include "drivers/DisplayDriver.h"

DisplayDriver::DisplayDriver()
{
    turnOnBacklight();
}

void DisplayDriver::turnOnBacklight()
{
    if (PIN_BACKLIGHT == GPIO_NUM_NC)
        return;

    gpio_reset_pin(PIN_BACKLIGHT);
    gpio_set_direction(PIN_BACKLIGHT, GPIO_MODE_OUTPUT);
    gpio_set_level(PIN_BACKLIGHT, 1);
}

void DisplayDriver::fillGreen()
{
}

void DisplayDriver::initSPI()
{
}

void DisplayDriver::initController()
{
}
