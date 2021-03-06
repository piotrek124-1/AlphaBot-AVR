#include <Arduino.h>
#include "alpha_bot_lib.h"

volatile uint8_t lEncoder;
volatile uint8_t rEncoder;
uint16_t totalLeft;
uint16_t totalRight;
boolean avoidanceStatus = true;
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

void avoidanceLeft() {
    leftMotor('f', lSpeed);
    delay(350);
    brake();
    delay(500);
    forward(lSpeed, rSpeed);
    delay(350);
    brake();
    delay(500);
}
void avoidanceRight() {
    rightMotor('b', rSpeed);
    delay(350);
    brake();
    delay(500);
    forward(lSpeed, rSpeed);
    delay(350);
    brake();
    delay(500);
}

void obstacleAvoidance() {
    uint8_t front = frontDetection();
    Serial.print("Dystans: ");
    Serial.println(front);
    if (avoidanceStatus = true) {
        char obstacleDirection;
        if (digitalRead(lIr) == LOW || digitalRead(rIr) == LOW) {
            brake();
            delay(500);
            uint8_t left = leftDetection();
            uint8_t right = rightDetection();
            uint8_t scan[8];
            if (left > right) {
                leftMotor('b', lSpeed);
                delay(350);
                brake();
                delay(500);
                forward(lSpeed, rSpeed);
                delay(350);
                brake();
                delay(500);
                for (uint8_t i = 0; i < 15; ++i) {
                    servoRotation(175);
                }
                for (unsigned char &i : scan) {
                    servoRotation(5);
                    delay(200);
                    i = detection();
                    Serial.println(i);
                    if (i < 20) {
                        leftMotor('f', lSpeed);
                        delay(350);
                        brake();
                        delay(500);
                        backward(lSpeed, rSpeed);
                        delay(300);
                        brake();
                        delay(500);
                        obstacleAvoidance();
                        break;
                    }
                }
                if (digitalRead(lIr) == LOW || digitalRead(rIr) == LOW) {
                    obstacleAvoidance();
                }
                leftMotor('f', lSpeed);
                delay(350);
                brake();
                delay(500);
                obstacleDirection = 'l';
            } else if (right > left) {
                rightMotor('b', rSpeed);
                delay(350);
                brake();
                delay(500);
                forward(lSpeed, rSpeed);
                delay(350);
                brake();
                delay(500);
                for (uint8_t i = 0; i < 15; ++i) {
                    servoRotation(175);
                }
                for (unsigned char &i : scan) {
                    servoRotation(5);
                    delay(200);
                    i = detection();
                    Serial.println(i);
                    if (i < 20) {
                        rightMotor('f', rSpeed);
                        delay(350);
                        brake();
                        delay(500);
                        backward(lSpeed, rSpeed);
                        delay(300);
                        brake();
                        delay(500);
                        obstacleAvoidance();
                        break;
                    }
                }
                if (digitalRead(lIr) == LOW || digitalRead(rIr) == LOW) {
                    obstacleAvoidance();
                }
                rightMotor('f', rSpeed);
                delay(350);
                brake();
                delay(500);
                obstacleDirection = 'r';
            }
            if (digitalRead(lIr) == HIGH && digitalRead(rIr) == HIGH && frontDetection() > 30) {
                forward(lSpeed, rSpeed);
                delay(700);
                brake();
                delay(500);
                if (obstacleDirection == 'l') {
                    leftMotor('f', lSpeed);
                    delay(350);
                    brake();
                    delay(500);
                    forward(lSpeed, rSpeed);
                    delay(350);
                    brake();
                    delay(500);
                    rightMotor('f', rSpeed);
                    delay(350);
                    brake();
                    delay(500);
                } else {
                    rightMotor('f', rSpeed);
                    delay(350);
                    brake();
                    delay(500);
                    forward(lSpeed, rSpeed);
                    delay(350);
                    brake();
                    delay(500);
                    leftMotor('f', lSpeed);
                    delay(350);
                    brake();
                    delay(500);
                }
            }
        }
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
    obstacleAvoidance();
    bluetoothRead();
    speedCorrection(totalLeft, totalRight);
}