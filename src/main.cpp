#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include "utils.h"
#include "robot.h"
#include <HM_10_BLE.h>
void bluetooth_handle();

#define BUZZER 10
#define LED 13
#define L_PWM 5
#define L_DIR 4
#define R_PWM 6
#define R_DIR 9
#define BLUETOOTH_RX 13
#define BLUETOOTH_TX 12
Servo serwo;
#define SERWO_PIN 11
#define SPEED_D_PIN 7
Robot robot(L_DIR, L_PWM, R_DIR, R_PWM, BUZZER, LED);
SoftwareSerial bluetooth(BLUETOOTH_RX, BLUETOOTH_TX);

void setup()
{
    // pinMode(BLUETOOTH_TX, OUTPUT);
    // digitalWrite(BLUETOOTH_TX, HIGH);
    // robot.Init();

    // pinMode(SPEED_D_PIN, INPUT);
    // serwo.attach(SERWO_PIN);
    Serial.begin(9600);
    Serial.print("Sketch:   ");
    Serial.println(__FILE__);
    Serial.print("Uploaded: ");
    Serial.println(__DATE__);
    Serial.println(" ");

    bluetooth.begin(9600);
    Serial.println("BTserial started at 9600");
}

void loop()
{
    // Serial.println(digitalRead(SPEED_D_PIN));

    // int i = 0;
    // // Obroty serwa w pętli co 1 stopień
    // for (i = 0; i < 180; i++)
    // {
    //     serwo.write(i); // Zmien pozycje
    //     Serial.println(i);
    //     delay(10); // Opoznienie dla lepszego efektu
    // }
    // unsigned long delta = CalculateDeltaTime();
    // ControllerState input = HandleInput(delta);
    // robot.Update(input, delta);
    // robot.Act(delta);
    bluetooth_handle();
}

char c = ' ';
boolean NL = true;
void bluetooth_handle()
{
    // Read from the Bluetooth module and send to the Arduino Serial Monitor
    if (bluetooth.available())
    {
        c = bluetooth.read();
        Serial.write(c);
    }

    // Read from the Serial Monitor and send to the Bluetooth module
    if (Serial.available())
    {
        c = Serial.read();

        // do not send line end characters to the HM-10
        if (c != 10 & c != 13)
        {
            bluetooth.write(c);
        }

        // Echo the user input to the main window.
        // If there is a new line print the ">" character.
        if (NL)
        {
            Serial.print("\r\n>");
            NL = false;
        }
        Serial.write(c);
        if (c == 10)
        {
            NL = true;
        }
    }
}