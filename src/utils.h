#ifndef UTILS_H
#define UTILS_H

enum class ControllerState
{
    NONE = 0,
    STOP = 1 << 1,
    LEFT = 1 << 2,
    RIGHT = 1 << 3,
    FORWARD = 1 << 4,
    BACKWARD = 1 << 5,
    HONK = 1 << 6
};

unsigned long CalculateDeltaTime();
ControllerState HandleInput(unsigned long delta);

#endif
