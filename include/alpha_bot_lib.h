#ifndef _ALPHA_BOT_LIB_H_
#define _ALPHA_BOT_LIB_H_
#include "Arduino.h"
#include "alpha_bot_lib.h"

class AlphaBotLib {

public:
    AlphaBotLib();
    AlphaBotLib(int8_t lSpeedPin, int8_t lIn1, int8_t lIn2Motor, int8_t rSpeedPin, int8_t rIn1, int8_t rIn2);
    void leftMotor(char direction, uint8_t speed);
    void rightMotor(char direction, uint8_t speed);
    void forward(uint8_t lSpeed, uint8_t rSpeed);
    void backward(uint8_t lSpeed, uint8_t rSpeed);
    void leftBrake();
    void rightBrake();
    void left(uint8_t speed);
    void right(uint8_t speed);
    void brake();
    void circle(char direction); // l - left, f - right
    void rSpeed(uint8_t speed);
    void lSpeed(uint8_t speed);
    void speed(uint8_t lSpeed, uint8_t rSpeed); // both
    void irSetup(int8_t lIr, int8_t rIr);
    void lIrDetection();
    void rIrDetection();
    void ultrasonicConfig(uint8_t echo, uint8_t trig);
    int ultrasonicRange();
    void bluetoothRead();
    void servoConfig(int8_t servoPin);
    void servoRotation(uint8_t angle); //0-180 degrees
    int8_t frontDetection();
    int absolute(int value);
    void obstacleAvoidance();
    int8_t leftDetection();
    int8_t rightDetection();
    void irObstacleDetection();
    void rightRotation(uint8_t rightEncoder, uint8_t rightSpeed);
    void leftRotation(uint8_t leftEncoder, uint8_t leftSpeed);

private:
    int8_t rMotorSpeedPin; // ENA
    int8_t lMotorSpeedPin; // ENB
    int8_t rMotorIn1; //Forward In1
    int8_t lMotorIn1; //Forward In2
    int8_t rMotorIn2; //Backward In3
    int8_t lMotorIn2; //Backward In4
    int8_t lMotorSpeed; // Left motor speed
    int8_t rMotorSpeed; // Right motor speed
    int8_t lIr;
    int8_t rIr;
    uint8_t echo;
    uint8_t trig;
    int8_t servoPin;
    int8_t servo1;
    int8_t servo2;
    uint16_t y;
    uint8_t leftDistance;
    uint8_t rightDistance;
    uint8_t frontDistance;

};
// TO DO
//struct onOff {
//    int8_t function;
//    boolean isOn;
//};
//onOff rIrDetection;
//onOff lIrDetection;
//rIrDetection.function = 3;
//lIrDetection.function = 4;
//rIrDetection.isOn = false;
//lIrDetection.isOn = false;
#endif //_ALPHA_BOT_LIB_H_