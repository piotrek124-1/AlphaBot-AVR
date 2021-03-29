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
void bluetoothRead();
void servoConfig();
void servoRotation(uint8_t angle);
extern uint8_t lSpeed;
extern uint8_t rSpeed;
uint8_t frontDetection();
uint8_t leftDetection();
uint8_t rightDetection();
uint8_t detection();

#endif //_ALPHA_BOT_LIB_H_
