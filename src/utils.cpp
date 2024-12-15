#include <RC5.h>
#include <Arduino.h>
#include "utils.h"

#define TSOP_PIN 3
unsigned long oldTime = 0;

unsigned long CalculateDeltaTime()
{
    unsigned long currentTime = millis();
    unsigned long deltaTime = currentTime - oldTime;
    oldTime = currentTime;
    return deltaTime;
}

RC5 rc5(TSOP_PIN);
enum ControllerState currentCommand = ControllerState::NONE;
float sinceLastCommandMs = 0;

#define KEY_1 1
#define KEY_2 2
#define KEY_3 3
#define KEY_4 4
#define KEY_5 5
#define KEY_6 6
#define KEY_7 7
#define KEY_8 8
#define KEY_9 9
#define KEY_0 0

#define KEY_POWER 12
#define KEY_OK 87
#define KEY_ARROW_UP 80
#define KEY_ARROW_RIGHT 86
#define KEY_ARROW_LEFT 85
#define KEY_ARROW_DOWN 81
ControllerState HandleInput(unsigned long delta)
{
    static byte address;
    static byte command;
    static byte toggle;
    if (rc5.read(&toggle, &address, &command))
    {
        sinceLastCommandMs = 0;
        switch (command)
        {
        case KEY_2:
        case KEY_ARROW_UP:
            currentCommand = ControllerState::FORWARD;
            break;
        case KEY_8:
        case KEY_ARROW_DOWN:
            currentCommand = ControllerState::BACKWARD;
            break;
        case KEY_5:
        case KEY_OK:
            currentCommand = ControllerState::STOP;
            break;
        case KEY_4:
        case KEY_ARROW_LEFT:
            currentCommand = ControllerState::LEFT;
            break;
        case KEY_6:
        case KEY_ARROW_RIGHT:
            currentCommand = ControllerState::RIGHT;
            break;
        case KEY_POWER:
            currentCommand = ControllerState::HONK;
            break;
        }
    }
    sinceLastCommandMs += delta;
    if (sinceLastCommandMs >= 144)
    {
        sinceLastCommandMs = 144;
        currentCommand = ControllerState::NONE;
    }

    return currentCommand;
}