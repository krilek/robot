#ifndef ROBOT_H
#define ROBOT_H
class Robot
{
private:
    uint8_t leftDirectionPin;
    uint8_t leftPwmPin;
    uint8_t rightDirectionPin;
    uint8_t rightPwmPin;
    uint8_t buzzerPin;
    uint8_t ledPin;
    const float baseHalf = 7.5 / 2;
    const float wheelRadius = 6.5 / 2;
    const float acceleration = 0.2;
    float velocity = 0;
    const float maxVelocity = 330;
    const float angularAcceleration = 0.05;
    const float angularDeceleration = 0.05;
    float angularVelocity = 0;
    const float maxAngularVelocity = 87;
    float honkTimeMs = 0;
    float rightWheelVelocity = 0;
    float leftWheelVelocity = 0;
    void leftMotor(int V);
    void rightMotor(int V);

public:
    Robot(uint8_t leftDirectionPin, uint8_t leftPwmPin, uint8_t rightDirectionPin, uint8_t rightPwmPin, uint8_t buzzerPin, uint8_t ledPin);

    void Init();
    void SettleAngularVelocity(unsigned long deltaTime);
    void MoveForward(unsigned long deltaTime);
    void MoveBackward(unsigned long deltaTime);
    void RotateLeft(unsigned long deltaTime);
    void RotateRight(unsigned long deltaTime);
    void Stop();
    void Honk(unsigned long deltaTime);
    void Update(ControllerState controllerState, unsigned long deltaTime);
    void Act(unsigned long deltaTime);
};
#endif
