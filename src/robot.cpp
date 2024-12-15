#include "utils.h"
#include <Arduino.h>
#include "robot.h"

void Robot::leftMotor(int leftMotorVelocity)
{
    const int PWM_L_MAX = 165;
    digitalWrite(leftDirectionPin, leftMotorVelocity > 0 ? 0 : 1);
    int pwm = map(abs(leftMotorVelocity), 0, 100, 0, PWM_L_MAX);
    analogWrite(leftPwmPin, pwm);
}
void Robot::rightMotor(int rightMotorVelocity)
{
    const int PWM_R_MAX = 165;
    digitalWrite(rightDirectionPin, rightMotorVelocity > 0 ? 0 : 1);
    int pwm = map(abs(rightMotorVelocity), 0, 100, 0, PWM_R_MAX);
    analogWrite(rightPwmPin, pwm);
}

Robot::Robot(uint8_t leftDirectionPin, uint8_t leftPwmPin, uint8_t rightDirectionPin, uint8_t rightPwmPin, uint8_t buzzerPin, uint8_t ledPin)
    : leftDirectionPin(leftDirectionPin), leftPwmPin(leftPwmPin), rightDirectionPin(rightDirectionPin), rightPwmPin(rightPwmPin), buzzerPin(buzzerPin), ledPin(ledPin)
{
}

void Robot::Init()
{
    pinMode(leftDirectionPin, OUTPUT);
    pinMode(leftPwmPin, OUTPUT);
    pinMode(rightDirectionPin, OUTPUT);
    pinMode(rightPwmPin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
    digitalWrite(buzzerPin, 0);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, 0);
    Stop();
}

void Robot::SettleAngularVelocity(unsigned long deltaTime)
{
    if (angularVelocity > 0)
    {
        angularVelocity -= angularDeceleration * deltaTime;
        angularVelocity = max(angularVelocity, 0.0f);
    }
    else if (angularVelocity < 0)
    {
        angularVelocity += angularDeceleration * deltaTime;
        angularVelocity = min(angularVelocity, 0.0f);
    }
}

void Robot::MoveForward(unsigned long deltaTime)
{
    velocity += acceleration * deltaTime;
}
void Robot::MoveBackward(unsigned long deltaTime)
{
    velocity -= acceleration * deltaTime;
}
void Robot::RotateLeft(unsigned long deltaTime)
{
    angularVelocity -= angularAcceleration * deltaTime;
}
void Robot::RotateRight(unsigned long deltaTime)
{
    angularVelocity += angularAcceleration * deltaTime;
}
void Robot::Stop()
{
    angularVelocity = 0;
    velocity = 0;
}
void Robot::Honk(unsigned long deltaTime)
{
    honkTimeMs = 150;
}
void Robot::Update(ControllerState controllerState, unsigned long deltaTime)
{
    switch (controllerState)
    {
    case ControllerState::STOP:
        Stop();
        break;
    case ControllerState::LEFT:
        RotateLeft(deltaTime);
        break;
    case ControllerState::RIGHT:
        RotateRight(deltaTime);
        break;
    case ControllerState::FORWARD:
        MoveForward(deltaTime);
        break;
    case ControllerState::BACKWARD:
        MoveBackward(deltaTime);
        break;
    case ControllerState::HONK:
        Honk(deltaTime);
        break;
    case ControllerState::NONE:
        break;
    }
    velocity = max(min(velocity, maxVelocity), -maxVelocity);
    angularVelocity = max(min(angularVelocity, maxAngularVelocity), -maxAngularVelocity);
    if (controllerState != ControllerState::LEFT && controllerState != ControllerState::RIGHT)
    {
        SettleAngularVelocity(deltaTime);
    }

    rightWheelVelocity = (velocity - angularVelocity * baseHalf) / wheelRadius;
    leftWheelVelocity = (velocity + angularVelocity * baseHalf) / wheelRadius;
    rightWheelVelocity = max(min(rightWheelVelocity, 100.0f), -100.0f);
    leftWheelVelocity = max(min(leftWheelVelocity, 100.0f), -100.0f);
    honkTimeMs -= deltaTime;
    honkTimeMs = max(honkTimeMs, 0);
}
void Robot::Act(unsigned long deltaTime)
{
    leftMotor(leftWheelVelocity);
    rightMotor(rightWheelVelocity);
    digitalWrite(buzzerPin, honkTimeMs > 0);
    digitalWrite(ledPin, honkTimeMs > 0);
}