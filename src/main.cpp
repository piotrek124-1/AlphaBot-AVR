#include <Arduino.h>
#include "alpha_bot_lib.h"

uint8_t lSpeed;
uint8_t rSpeed;
volatile uint8_t lEncoder;
uint8_t lRotationCount;
volatile uint8_t rEncoder;
uint8_t rRotationCount;
uint16_t totalLeft;
uint16_t totalRight;

void leftEncoder() {
    lEncoder++;
    totalLeft++;
    if (lEncoder == 20) {
        lEncoder = 0;
//        lRotationCount++;
    }
}

void rightEncoder() {
    rEncoder++;
    totalRight++;
    if (rEncoder == 20) {
        rEncoder = 0;
//        rRotationCount++;
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

void rightRotation1(uint8_t rightSpeed, char direction) {
    rEncoder = 0;
    if (direction == 'f') {
        while (rEncoder < 5) {
            rightMotor('f', rightSpeed);
        }
    } else if (direction == 'b') {
        while (rEncoder < 5) {
            rightMotor('b', rightSpeed);
        }
    }
    rEncoder = 0;
    lEncoder = 0;
    brake();
    delay(500);
}

void leftRotation1(uint8_t leftSpeed, char direction) {
    lEncoder = 0;
    if (direction == 'f') {
        while (lEncoder < 5) {
            leftMotor('f', leftSpeed);
        }
    } else if (direction == 'b') {
        while (lEncoder < 5) {
            leftMotor('b', leftSpeed);
        }
    }
    lEncoder = 0;
    rEncoder = 0;
    brake();
    delay(500);
}

void obstacleAvoidance() {
    front = frontDetection();
}
void setup() {
    Serial.begin(9600);
    irSetup();
    ultrasonicConfig();
    servoConfig();
    lSpeed = 150;
    rSpeed = 150;
    attachInterrupt(digitalPinToInterrupt(2), leftEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(3), rightEncoder, CHANGE);
    delay(3000);
}

void loop() {
    bluetoothRead(lSpeed, rSpeed);
    speedCorrection(totalLeft, totalRight);
    front = frontDetection();
}