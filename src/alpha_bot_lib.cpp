#include "alpha_bot_lib.h"
// drives forward
uint8_t lSpeed = 150;
uint8_t rSpeed = 150;
boolean avoidanceStatus = true;
void forward(uint8_t lSpeed, uint8_t rSpeed) {
leftMotor('f', lSpeed);
rightMotor('f', rSpeed);
}
// drives backward
void backward(uint8_t lSpeed, uint8_t rSpeed) {
    leftMotor('b', lSpeed);
    rightMotor('b', rSpeed);
}
// left motor
void leftMotor(char direction, uint8_t speed) {
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
void rightMotor(char direction, uint8_t speed) {
    analogWrite(rMotorSpeedPin, speed);
    if (direction == 'f') {
        digitalWrite(rMotorIn2, LOW);
        digitalWrite(rMotorIn1, HIGH);
    } else if (direction == 'b') {
        digitalWrite(rMotorIn1, LOW);
        digitalWrite(rMotorIn2, HIGH);
    }
}
// left motor off
void leftBrake() {
    digitalWrite(lMotorSpeedPin, LOW);
}
// right motor off
void rightBrake() {
    digitalWrite(rMotorSpeedPin, LOW);
}
// both motors off
void brake() {
    leftBrake();
    rightBrake();
}
void irSetup() {
    pinMode(lIr, INPUT);
    pinMode(rIr, INPUT);
}
void lIrDetection() {
    digitalRead(lIr);
}
void rIrDetection() {
    digitalRead(rIr);
}
void ultrasonicConfig() {
    pinMode(echo, INPUT);
    pinMode(trig, OUTPUT);
}
// Sending ultrasonic wave modulated by pwm method (8 signals)
int ultrasonicRange() {
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    return pulseIn(echo, HIGH) / 58;
}

void bluetoothRead() {
    uint8_t input[4]; // Serial.read() return int
    input[0] = Serial.read(); // 1st frame check
    input[1] = Serial.read(); // function
    input[2] = Serial.read(); // value
    input[3] = Serial.read(); // second frame check
//    Serial.println(input[1]);
    if (input[0] == 2 && input[3] == 3) {
        if (input[1] == 10) {
            brake();
            forward(lSpeed, rSpeed);
        } else if (input[1] == 11) {
            brake();
            leftMotor('f', lSpeed);
        } else if (input[1] == 12) {
            brake();
            rightMotor('f', rSpeed);
        } else if (input[1] == 13) {
            brake();
            backward(lSpeed, rSpeed);
        } else if (input[1] == 0) {
            if (avoidanceStatus) {
                avoidanceStatus = false;
            } else {
                avoidanceStatus = true;
            }
        } else if (input[1] == 1) {
            brake();
            delay(400);
        } else if (input[1] == 2) {
            lSpeed = input[2];
            rSpeed = input[2];
        }
    }
}

// Servo
void servoConfig() {
    pinMode(servoPin, OUTPUT);
}
// Servo rotation
void servoRotation(uint8_t angle) {
    uint16_t y = 111 * angle + 5000; // y = 1 / 90 * x + 1 / 2
    digitalWrite(9, HIGH);
    delayMicroseconds(y / 10);
    digitalWrite(9, LOW);
    delay(20 - (y / 10000)); // delay is unsigned long, delay microseconds is unsigned int
}

uint8_t detection() {
    return abs(ultrasonicRange());
}
uint8_t frontDetection() {
    for (int i = 0; i < 5; ++i) {
        servoRotation(90);
    }
    delay(200);
    return abs(ultrasonicRange());
}

uint8_t leftDetection() {
    for (int i = 0; i < 15; ++i) {
        servoRotation(175);
    }
    delay(200);
    return abs(ultrasonicRange());
}
uint8_t rightDetection() {
    for (int i = 0; i < 15; ++i) {
        servoRotation(5);
    }
    delay(200);
    return abs(ultrasonicRange());
}







