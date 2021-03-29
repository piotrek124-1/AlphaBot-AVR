#include <Arduino.h>
#include "alpha_bot_lib.h"

volatile uint8_t lEncoder;
volatile uint8_t rEncoder;
volatile uint16_t totalLeft;
volatile uint16_t totalRight;

void leftEncoder() {
    lEncoder++;
    totalLeft++;
    if (lEncoder == 20) {
        lEncoder = 0;
    }
}

void rightEncoder() {
    rEncoder++;
    totalRight++;
    if (rEncoder == 20) {
        rEncoder = 0;
    }
}

void speedCorrection(uint8_t leftRotationCount, uint8_t rightRotationCount) {
    if (leftRotationCount != 0 && rightRotationCount != 0 && leftRotationCount > 5 && rightRotationCount > 5) {
        if (rightRotationCount < leftRotationCount) {
            rSpeed++;
        } else if (leftRotationCount < rightRotationCount) {
            rSpeed--;
        }
        totalRight = 0;
        totalLeft = 0;
    }
}

void setup() {
    Serial.begin(9600);
    irSetup();
    ultrasonicConfig();
    servoConfig();
    attachInterrupt(digitalPinToInterrupt(2), leftEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(3), rightEncoder, CHANGE);
    delay(3000);
}

void loop() {
    obstacleAvoidanceRotate();
    bluetoothRead();
    speedCorrection(totalLeft, totalRight);
}
