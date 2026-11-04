#include <Arduino.h>
#include <Wire.h>

#include "motor_driver/motor_driver.h"
#include "lcd_driver/lcd_driver.h"

int powerPercent = 0;

void setup() {
    Serial.begin(9600);

    motorInit(10, 7, 6);
    lcdInit();

    powerPercent = 70;

    setMotorPower(powerPercent);
    lcdShowStatus(powerPercent);
}

void loop() {
    if (Serial.available()) {
        powerPercent = Serial.parseInt();

        setMotorPower(powerPercent);
        lcdShowStatus(powerPercent);
    }
}