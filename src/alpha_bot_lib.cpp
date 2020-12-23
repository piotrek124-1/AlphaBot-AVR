#include "alpha_bot_lib.h"

AlphaBotLib::AlphaBotLib() {

    lMotorSpeedPin = 5; // ENA
    lMotorIn1 = A0; // left forward IN1
    lMotorIn2 = A1; // left back IN2

    rMotorSpeedPin = 6; // ENB
    rMotorIn1 = A3; // right forward IN4
    rMotorIn2 = A2; // right back IN3

    pinMode(lMotorSpeedPin, OUTPUT);
    pinMode(lMotorIn1, OUTPUT);
    pinMode(lMotorIn2, OUTPUT);
    pinMode(rMotorSpeedPin, OUTPUT);
    pinMode(rMotorIn1, OUTPUT);
    pinMode(rMotorIn2, OUTPUT);
}
AlphaBotLib::AlphaBotLib(int8_t lSpeedPin, int8_t lIn1, int8_t lIn2, int8_t rSpeedPin, int8_t rIn1, int8_t rIn2) {

    this -> lMotorSpeedPin = lSpeedPin;
    this -> lMotorIn1 = lIn1;
    this -> lMotorIn2 = lIn2;
    pinMode(lMotorSpeedPin, OUTPUT);
    pinMode(lIn1, OUTPUT);
    pinMode(lIn2, OUTPUT);

    this -> rMotorSpeedPin = rSpeedPin;
    this -> rMotorIn1 = rIn1;
    this -> rMotorIn2 = rIn2;
    pinMode(rSpeedPin, OUTPUT);
    pinMode(rSpeedPin, OUTPUT);
    pinMode(rIn1, OUTPUT);
    pinMode(rIn2, OUTPUT);
}

// left motor
void AlphaBotLib::leftMotor(char direction, uint8_t speed) {
    analogWrite(lMotorSpeedPin, speed);
    if (direction == 'f') {
        digitalWrite(lMotorIn2, LOW);
        digitalWrite(lMotorIn1, HIGH);
    } else if (direction == 'b'){
        digitalWrite(lMotorIn1, LOW);
        digitalWrite(lMotorIn2, HIGH);
    }
}
// right motor
void AlphaBotLib::rightMotor(char direction, uint8_t speed) {
    analogWrite(rMotorSpeedPin, speed);
    if (direction == 'f') {
        digitalWrite(rMotorIn2, LOW);
        digitalWrite(rMotorIn1, HIGH);
    } else if (direction == 'b') {
        digitalWrite(rMotorIn1, LOW);
        digitalWrite(rMotorIn2, HIGH);
    }
}

// drives forward
void AlphaBotLib::forward(uint8_t lSpeed, uint8_t rSpeed){
    leftMotor('f', lSpeed);
    rightMotor('f', rSpeed);
}
// drives backward
void AlphaBotLib::backward(uint8_t lSpeed, uint8_t rSpeed) {
    leftMotor('b', lSpeed);
    rightMotor('b', rSpeed);
}
// left motor off
void AlphaBotLib::leftBrake() {
    digitalWrite(lMotorSpeedPin, LOW);
}
// right motor off
void AlphaBotLib::rightBrake() {
    digitalWrite(rMotorSpeedPin, LOW);
}
// both motors off
void AlphaBotLib::brake() {
    leftBrake();
    rightBrake();
}
// 'l' -> only the left motor is working, 'r -> only the right motor is working
void AlphaBotLib::circle(char direction) {
    if (direction == 'r') {
        digitalWrite(rMotorSpeedPin, LOW);
        digitalWrite(lMotorSpeedPin, HIGH);
    } else if (direction == 'l'){
        digitalWrite(lMotorSpeedPin, LOW);
        digitalWrite(rMotorSpeedPin, HIGH);
    }
}
void AlphaBotLib::irSetup(int8_t lIr, int8_t rIr) {
    this -> lIr = 7;
    this -> rIr = 8;
    pinMode(lIr, INPUT);
    pinMode(rIr, INPUT);
}
void AlphaBotLib::lIrDetection() {
    digitalRead(lIr);
}
void AlphaBotLib::rIrDetection() {
    digitalRead(rIr);
}
void AlphaBotLib::ultrasonicConfig(uint8_t echo, uint8_t trig) {
    this -> echo = 12;
    this -> trig = 11;
    pinMode(echo, INPUT);
    pinMode(trig, OUTPUT);
}
// Sending ultrasonic wave modulated by pwm method (8 signals)
int AlphaBotLib::ultrasonicRange() {
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    return pulseIn(echo, HIGH) / 58;
}

// Bluetooth input (App -> Arduino)
// Bluetooth sends ASCII
volatile void AlphaBotLib::bluetoothRead(uint8_t lSpeed, uint8_t rSpeed) {
    int input[3]; // Serial.read() return int
    input[0] = Serial.read(); // 1st frame check
    input[1] = Serial.read(); // function
    input[2] = Serial.read(); // value
    input[3] = Serial.read(); // second frame check
    if (input[0] == 2 && input[3] == 3) {
        if (input[1] == 10) {
            forward(lSpeed, rSpeed);
        } else if (input[1] == 11) {
            leftMotor('f', lSpeed);
        } else if (input[1] == 12) {
            rightMotor('f', rSpeed);
        } else if (input[1] == 13) {
            backward(lSpeed, rSpeed);
        } else if (input[1] == 1) {
            brake();
            delay(400);
        }
    }
    if (input[1] == 10 || input[1] == 11 || input[1] == 12 || input[1] == 13) {
        bluetoothRead(lSpeed, rSpeed);
    }
}

// Servo
void AlphaBotLib::servoConfig(int8_t servoPin) {
    this -> servoPin = 9;
    pinMode(servoPin, OUTPUT);
}
// Servo rotation
void AlphaBotLib::servoRotation(uint8_t angle) {
    y = 111 * angle + 5000; // y = 1 / 90 * x + 1 / 2
    digitalWrite(9, HIGH);
    delayMicroseconds(y / 10);
    digitalWrite(9, LOW);
    delay(20 - (y / 10000)); // delay is unsigned long, delay microseconds is unsigned int
}

int8_t AlphaBotLib::frontDetection() {
    for (int i = 0; i < 5; ++i) {
        servoRotation(90);
    }
    delay(200);
    return abs(ultrasonicRange());
}

int8_t AlphaBotLib::leftDetection() {
    for (int i = 0; i < 15; ++i) {
        servoRotation(175);
    }
    delay(200);
    return abs(ultrasonicRange());
}
int8_t AlphaBotLib::rightDetection() {
    for (int i = 0; i < 15; ++i) {
        servoRotation(5);
    }
    delay(200);
    return abs(ultrasonicRange());
}

void AlphaBotLib::irObstacleDetection() {
    if (digitalRead(lIr) == HIGH && digitalRead(rIr) == HIGH) {
        rightMotor('f', 140);
        leftMotor('f', 120);
    } else if (digitalRead(lIr) == LOW) {
        brake();
        delay(300);
    }
}
//CHECK

void AlphaBotLib::obstacleAvoidance() {
    if (digitalRead(lIr) == LOW && digitalRead(rIr) == LOW) {

    } else if (digitalRead(lIr) == LOW) {
        while (digitalRead(lIr) == LOW) {
            //TO DO
        }
        leftDistance = leftDetection();
    } else if (digitalRead(rIr) == LOW) {
        rightDistance = rightDetection();
    } else {
        //TO DO
    }
}






