#include "motor_driver.h"
#include <Arduino.h>

int ENA, IN1, IN2;

void motorInit(int enaPin, int in1Pin, int in2Pin) {
    ENA = enaPin;
    IN1 = in1Pin;
    IN2 = in2Pin;

    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
}

void setMotorPower(int power) {
    int pwm = map(abs(power), 0, 100, 0, 255);

    if (power > 0) {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
    } else if (power < 0) {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
    } else {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
    }

    analogWrite(ENA, pwm);
}