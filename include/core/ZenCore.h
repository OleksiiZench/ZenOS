#pragma once

class Buzzer;
class InputManager;
class DisplayDriver;

class ZenCore
{
public:
    ~ZenCore();

    void init();
    void update();

private:
    Buzzer* _buzzer = nullptr;
    InputManager* _inputManager = nullptr;
    DisplayDriver* _display = nullptr;

    void startBootTimer();
    void bindButtonsInInputManager();
};
