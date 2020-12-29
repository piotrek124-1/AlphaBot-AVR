#include <Arduino.h>
#include "alpha_bot_lib.h"

uint8_t lSpeed;
uint8_t rSpeed;
volatile uint8_t lEncoder;
uint8_t lRotationCount;
volatile uint8_t rEncoder;
uint8_t rRotationCount;
uint8_t lIr = 7;
uint8_t rIr = 8;
uint8_t temp;
uint8_t temp2;
uint8_t left;
uint8_t right;
uint8_t front;

void leftEncoder() {
    lEncoder++;
    if (lEncoder == 20) {
        lEncoder = 0;
        lRotationCount++;
    }
}

void rightEncoder() {
    rEncoder++;
    if (rEncoder == 20) {
        rEncoder = 0;
        rRotationCount++;
    }
}

void speedCorrection(uint8_t leftRotationCount, uint8_t rightRotationCount) {
    if (leftRotationCount != 0 && rightRotationCount != 0) {
        if (rightRotationCount < leftRotationCount) {
            rSpeed++;
            rRotationCount = 0;
            lRotationCount = 0;
        } else if (lRotationCount < rightRotationCount) {
            rSpeed--;
            rRotationCount = 0;
            lRotationCount = 0;
        }
    }
}

void rightRotation1(uint8_t rightSpeed, char direction) {
    rEncoder = 0;
    if (direction == 'f') {
        while (rEncoder < 13) {
            AlphaBotLib().rightMotor('f', rightSpeed);
        }
    } else if (direction == 'b') {
        while (rEncoder < 13) {
            AlphaBotLib().rightMotor('b', rightSpeed);
        }
    }
    rEncoder = 0;
    lEncoder = 0;
    AlphaBotLib().brake();
    delay(500);
}

void leftRotation1(uint8_t leftSpeed, char direction) {
    lEncoder = 0;
    if (direction == 'f') {
        while (lEncoder < 13) {
            AlphaBotLib().leftMotor('f', leftSpeed);
        }
    } else if (direction == 'b') {
        while (rEncoder < 13) {
            AlphaBotLib().rightMotor('b', leftSpeed);
        }
    }
    lEncoder = 0;
    rEncoder = 0;
    AlphaBotLib().brake();
    delay(500);
}

void obstacle() {
    if (digitalRead(lIr) == LOW && digitalRead(rIr) == LOW) {

    } else if (digitalRead(lIr) == LOW) {
        temp = 0;
        AlphaBotLib().brake();
        delay(400);
        while (digitalRead(lIr) == LOW) {
            temp++;
            leftRotation1(rSpeed, 'f');
        }
        AlphaBotLib().forward(lSpeed, rSpeed);
        delay(800);
        AlphaBotLib().brake();
        delay(400);
        temp2 = temp;
        while (temp2 > 0) {
            temp2--;
            leftRotation1(rSpeed, 'b');
        }
        AlphaBotLib().forward(lSpeed, rSpeed);
        delay(500);
        temp2 = temp;
        AlphaBotLib().brake();
        delay(400);
        while (temp2 > 0) {
            temp2--;
            leftRotation1(rSpeed, 'b');
        }
        AlphaBotLib().forward(lSpeed, rSpeed);
        delay(800);
        temp2 = temp;
        AlphaBotLib().brake();
        delay(400);
        while (temp2 > 0) {
            temp2--;
            leftRotation1(rSpeed, 'f');
        }
    }
}
//void ultrasonic() {
//    front = AlphaBotLib().frontDetection();
//    if (front < 30) {
//        AlphaBotLib().brake();
//        delay(300);
//        left = AlphaBotLib().leftDetection();
//        right = AlphaBotLib().rightDetection();
//        Serial.println(left);
//        Serial.println(right);
//        if (left < right) {
//            AlphaBotLib().backward(lSpeed, rSpeed);
//            delay(400);
//            AlphaBotLib().brake();
//            delay(300);
//            AlphaBotLib().rightMotor(lSpeed, 'f');
//            delay(400);
//            AlphaBotLib().brake();
//            delay(300);
//        } else if (right < left) {
//            AlphaBotLib().backward(lSpeed, rSpeed);
//            delay(400);
//            AlphaBotLib().brake();
//            delay(300);
//            AlphaBotLib().rightMotor(rSpeed, 'f');
//            delay(400);
//            AlphaBotLib().brake();
//            delay(300);
//        }
//    }
//}
void setup() {
    Serial.begin(9600);
    AlphaBotLib().irSetup(7, 8);
    AlphaBotLib().ultrasonicConfig(12, 11);
    AlphaBotLib().servoConfig(9);
    lSpeed = 130;
    rSpeed = 130;
    attachInterrupt(digitalPinToInterrupt(2), leftEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(3), rightEncoder, CHANGE);
    delay(3000);
}

void loop() {
    front = AlphaBotLib().frontDetection();
    AlphaBotLib().bluetoothRead(lSpeed, rSpeed);
    speedCorrection(lRotationCount, rRotationCount);
    Serial.print("Left: ");
    Serial.println(lRotationCount);
    Serial.print("Right: ");
    Serial.println(rRotationCount);
//    if (front < 30) {
//        AlphaBotLib().brake();
//        delay(400);
//    }
}