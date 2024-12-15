#include <Arduino.h>
#include "utils.h"
#include "robot.h"

#define BUZZER 10
#define LED 13
#define L_PWM 5
#define L_DIR 4
#define R_PWM 6
#define R_DIR 9

Robot robot(L_DIR, L_PWM, R_DIR, R_PWM, BUZZER, LED);

void setup()
{
    robot.Init();
    // Serial.begin(9600);
}

void loop()
{
    unsigned long delta = CalculateDeltaTime();
    ControllerState input = HandleInput(delta);
    robot.Update(input, delta);
    robot.Act(delta);
}