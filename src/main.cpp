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
struct obstacleDistance {
    static uint8_t distance;
    boolean isObstacle;
} obstacle[8];
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
//void obstacle() {
//    if (digitalRead(lIr) == LOW && digitalRead(rIr) == LOW) {
//        brake();
//        delay(500);
//    } else if (digitalRead(lIr) == LOW) {
//        uint8_t temp = 0;
//        brake();
//        delay(400);
//        while (digitalRead(lIr) == LOW) {
//            temp++;
//            rightRotation1(rSpeed, 'b');
//        }
//        while (uint8_t left = leftDetection() < 30) {
//            temp++;
//            rightRotation1(rSpeed, 'b');
//        }
//        forward(lSpeed, rSpeed);
//        delay(800);
//        brake();
//        delay(400);
//        uint8_t temp2 = temp;
//        while (temp2 > 0) {
//            temp2--;
//            rightRotation1(rSpeed, 'f');
//        }
//        forward(lSpeed, rSpeed);
//        delay(500);
//        temp2 = temp;
//        brake();
//        delay(400);
//        while (temp2 > 0) {
//            temp2--;
//            rightRotation1(rSpeed, 'f');
//        }
//        forward(lSpeed, rSpeed);
//        delay(800);
//        temp2 = temp;
//        brake();
//        delay(400);
//        while (temp2 > 0) {
//            temp2--;
//            leftRotation1(lSpeed, 'f');
//        }
//    } else if (digitalRead(rIr) == LOW) {
//        uint8_t temp = 0;
//        brake();
//        delay(400);
//        while (digitalRead(rIr) == LOW) {
//            temp++;
//            leftRotation1(lSpeed, 'b');
//        }
//        while (uint8_t right = rightDetection() < 30) {
//            temp++;
//            leftRotation1(lSpeed, 'b');
//        }
//        forward(lSpeed, rSpeed);
//        delay(800);
//        brake();
//        delay(400);
//        uint8_t temp2 = temp;
//        while (temp2 > 0) {
//            temp2--;
//            leftRotation1(lSpeed, 'f');
//        }
//        forward(lSpeed, rSpeed);
//        delay(500);
//        temp2 = temp;
//        brake();
//        delay(400);
//        while (temp2 > 0) {
//            temp2--;
//            leftRotation1(lSpeed, 'f');
//        }
//        forward(lSpeed, rSpeed);
//        delay(800);
//        temp2 = temp;
//        brake();
//        delay(400);
//        while (temp2 > 0) {
//            temp2--;
//            rightRotation1(rSpeed, 'f');
//        }
//    }
//}

void obstacleAvoidance() {
    uint8_t front = frontDetection();
    char obstacleDirection;
    if (front < 30) {
        brake();
        delay(500);
        uint8_t left = leftDetection();
        uint8_t right = rightDetection();
        if (left > right) {
            rightMotor('b', rSpeed);
            delay(400);
            brake();
            delay(500);
            forward(lSpeed, rSpeed);
            delay(550);
            brake();
            delay(500);
            rightMotor('f', rSpeed);
            delay(400);
            brake();
            obstacleDirection = 'r';
            obstacleAvoidance();
        } else if (right > left) {
            leftMotor('b', lSpeed);
            delay(400);
            brake();
            delay(500);
            forward(lSpeed, rSpeed);
            delay(550);
            brake();
            delay(500);
            leftMotor('f', lSpeed);
            delay(400);
            brake();
            obstacleDirection = 'l';
            obstacleAvoidance();
        }
        if (digitalRead(lIr) == HIGH && digitalRead(lIr) == HIGH) {
            forward(lSpeed, rSpeed);
            delay(550);
            brake();
            delay(500);
            if (obstacleDirection == 'l') {

            } else {

            }
        }
    }
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
    obstacleAvoidance();
//    bluetoothRead(lSpeed, rSpeed);
//    speedCorrection(totalLeft, totalRight);
//    obstacleAvoidance();
}