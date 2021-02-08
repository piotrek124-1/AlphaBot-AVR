#ifndef _ALPHA_BOT_LIB_H_
#define _ALPHA_BOT_LIB_H_
#include "Arduino.h"
#include "alpha_bot_lib.h"
constexpr uint8_t lMotorSpeedPin = 5; // ENA
constexpr uint8_t lMotorIn1 = A0; // left forward IN1
constexpr uint8_t lMotorIn2 = A1; // left back IN2
constexpr uint8_t rMotorSpeedPin = 6; // ENB
constexpr uint8_t rMotorIn1 = A3; // right forward IN4
constexpr uint8_t rMotorIn2 = A2; // right back IN3
constexpr uint8_t lIr = 7;
constexpr uint8_t rIr = 8;
constexpr uint8_t echo = 12;
constexpr uint8_t trig = 11;
constexpr uint8_t servoPin = 9;

class AlphaBotLib {
uint8_t leftMotorSpeed {};
uint8_t rightMotorSpeed {};
public:
    AlphaBotLib();
    void btTest(uint8_t lSpeed, uint8_t rSpeed);
    uint8_t speedCorrection(uint8_t leftRotationCount, uint8_t rightRotationCount, uint8_t rSpeed);
    AlphaBotLib(int8_t lSpeedPin, int8_t lIn1, int8_t lIn2Motor, int8_t rSpeedPin, int8_t rIn1, int8_t rIn2);
    void leftMotor(char direction, uint8_t speed);
    void rightMotor(char direction, uint8_t speed);
    void forward(uint8_t lSpeed, uint8_t rSpeed);
    void backward(uint8_t lSpeed, uint8_t rSpeed);
    void leftBrake();
    void rightBrake();
    void brake();
    void circle(char direction); // l - left, f - right
    void irSetup(int8_t lIr, int8_t rIr);
    void lIrDetection();
    void rIrDetection();
    void ultrasonicConfig(uint8_t echo, uint8_t trig);
    int ultrasonicRange();
    volatile void bluetoothRead(uint8_t lSpeed, uint8_t rSpeed);
    void servoConfig(uint8_t servoPin);
    void servoRotation(uint8_t angle); //0-180 degrees
    int8_t frontDetection();
    void obstacleAvoidance();
    int8_t leftDetection();
    int8_t rightDetection();
    void irObstacleDetection();

private:
    int8_t rMotorSpeedPin; // ENA
    int8_t lMotorSpeedPin; // ENB
    int8_t rMotorIn1; //Forward In1
    int8_t lMotorIn1; //Forward In2
    int8_t rMotorIn2; //Backward In3
    int8_t lMotorIn2; //Backward In4
    int8_t lMotorSpeed; // Left motor speed
    int8_t rMotorSpeed; // Right motor speed
    uint8_t servoPin;
    int8_t lIr;
    int8_t rIr;
    uint8_t echo;
    uint8_t trig;
    uint16_t y;
    uint8_t leftDistance;
    uint8_t rightDistance;
    uint8_t frontDistance;
};
void leftMotor(char direction, uint8_t speed);
void rightMotor(char direction, uint8_t speed);
void forward(uint8_t lSpeed, uint8_t rSpeed);
void backward(uint8_t lSpeed, uint8_t rSpeed);
void leftBrake();
void rightBrake();
void brake();
void irSetup();
void lIrDetection();
void rIrDetection();
void ultrasonicConfig();
int ultrasonicRange();
void bluetoothRead(uint8_t lSpeed, uint8_t rSpeed);
void servoConfig();
void servoRotation(uint8_t angle);
uint8_t leftSpeed = 150;
uint8_t rightSpeed = 150;
uint8_t frontDetection();
uint8_t leftDetection();
uint8_t rightDetection();
uint8_t detection();
#endif //_ALPHA_BOT_LIB_H_