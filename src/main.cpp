#include <Arduino.h>
#include "alpha_bot_lib.h"

boolean avoidanceStatus = true;
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
    uint8_t front[3];
    front[0] = frontDetection();
    delay(100);
    front[1] = frontDetection();
    delay(100);
    front[2] = frontDetection();
    char obstacleDirection;
    if (front[0] < 30 && front[1] < 30 && front[2] < 30) {
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
        if (digitalRead(lIr) == HIGH && digitalRead(lIr) == HIGH && frontDetection() > 30) {
            forward(lSpeed, rSpeed);
            delay(550);
            brake();
            delay(500);
            if (obstacleDirection == 'l') {
                leftMotor('f', lSpeed);
                delay(400);
                brake();
                delay(500);
                forward(lSpeed, rSpeed);
                delay(500);
                brake();
                delay(500);
                leftMotor('b', lSpeed);
                delay(400);
                brake();
            } else {
                rightMotor('f', rSpeed);
                delay(400);
                brake();
                delay(500);
                forward(lSpeed, rSpeed);
                delay(500);
                brake();
                delay(500);
                rightMotor('b', rSpeed);
                delay(400);
                brake();
            }
        }
    }
}
void obstacleAvoidance1() {
    uint8_t front = frontDetection();
    Serial.print("Dystans: ");
    Serial.println(front);
    char obstacleDirection;
    if (digitalRead(lIr) == LOW || digitalRead(rIr) == LOW) {
        brake();
        delay(500);
        uint8_t left = leftDetection();
        uint8_t right = rightDetection();
        uint8_t scan[8];
        if (left > right) {
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
            for (unsigned char & i : scan) {
                servoRotation(5);
                delay(700);
                i = detection();
                if (i < 20) {
                    forward(lSpeed, rSpeed);
                    delay(300);
                    brake();
                    delay(500);
                    break;
                }
            }
            rightMotor('f', rSpeed);
            delay(350);
            brake();
            delay(500);
            obstacleDirection = 'r';
            if (digitalRead(lIr) == LOW || digitalRead(rIr) == LOW) {
                obstacleAvoidance1();
            }
        } else if (right > left) {
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
                delay(700);
                i = detection();
                if (i < 20) {
                    forward(lSpeed, rSpeed);
                    delay(200);
                    brake();
                    delay(500);
                    break;
                }
            }
            if (digitalRead(lIr) == LOW || digitalRead(rIr) == LOW) {
                obstacleAvoidance1();
            }
            leftMotor('f', lSpeed);
            delay(350);
            brake();
            obstacleDirection = 'l';
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
    obstacleAvoidance1();
    bluetoothRead();
    speedCorrection(totalLeft, totalRight);

}