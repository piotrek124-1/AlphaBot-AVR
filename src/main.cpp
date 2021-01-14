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
        } else if (lRotationCount < rightRotationCount) {
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
            AlphaBotLib().rightMotor('f', rightSpeed);
        }
    } else if (direction == 'b') {
        while (rEncoder < 5) {
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
        while (lEncoder < 5) {
            AlphaBotLib().leftMotor('f', leftSpeed);
        }
    } else if (direction == 'b') {
        while (lEncoder < 5) {
            AlphaBotLib().leftMotor('b', leftSpeed);
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

void avoidance() {
    front = AlphaBotLib().frontDetection();
    if (front < 30) {
        AlphaBotLib().brake();
        delay(500);
//        left = AlphaBotLib().leftDetection();
//        right = AlphaBotLib().rightDetection();
//        front = AlphaBotLib().frontDetection();
        if (digitalRead(lIr) == HIGH && digitalRead(rIr) == HIGH) {
        } else if (digitalRead(lIr) == LOW && digitalRead(rIr) == HIGH) {
            temp = 0;
            while (digitalRead(lIr) == LOW) {
                leftRotation1(lSpeed, 'f');
                temp++;
            }
            AlphaBotLib().forward(lSpeed, rSpeed);
            delay(500);
            AlphaBotLib().brake();
            delay(500);
            temp2 = temp;
            while (temp > 0) {
                rightRotation1(220, 'f');
                temp--;
            }
            temp = temp2;
            //ULTRASONIC
            AlphaBotLib().forward(lSpeed, rSpeed);
            delay(500);
            AlphaBotLib().brake();
            delay(500);
            temp2 = temp;
            while (temp2 > 0) {
                rightRotation1(220, 'f');
                temp2--;
            }
            AlphaBotLib().forward(lSpeed, rSpeed);
            delay(500);
            AlphaBotLib().brake();
            delay(500);
            while (temp > 0) {
                leftRotation1(220, 'f');
                temp--;
            }
        } else if (digitalRead(rIr) == LOW && digitalRead(lIr) == HIGH) {
            temp = 0;
            while (digitalRead(rIr) == LOW) {
                rightRotation1(220, 'f');
                temp++;
            }
        }
    }
}
void uAvoidance() {
    front = AlphaBotLib().frontDetection();
    if (front < 30) {
        AlphaBotLib().brake();
        delay(500);
        //Scan left right
        left = AlphaBotLib().ultrasonicRange();
        right = AlphaBotLib().ultrasonicRange();
        //Rotate
        //if
        if (left < right) {

        } else {
            left = AlphaBotLib().ultrasonicRange();
        }
        //Scan?

        //Forward

        //Scan obstacle

        //Turn

        // Forward


    }
}

void setup() {
    Serial.begin(9600);
    AlphaBotLib().irSetup(7, 8);
    AlphaBotLib().ultrasonicConfig(12, 11);
    AlphaBotLib().servoConfig(9);
    lSpeed = 150;
    rSpeed = 150;
    attachInterrupt(digitalPinToInterrupt(2), leftEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(3), rightEncoder, CHANGE);
    delay(3000);
}

void loop() {
AlphaBotLib().btTest(lSpeed, rSpeed);
speedCorrection(totalLeft, totalRight);
    front = AlphaBotLib().frontDetection();
    Serial.println(lSpeed);
    Serial.println(lRotationCount);
    Serial.println(rRotationCount);
    Serial.println(rSpeed);
}