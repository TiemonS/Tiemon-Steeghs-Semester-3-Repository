#include "ServoMotor.hpp"
#include "CarTimer.h"

ServoMotor::ServoMotor() {}

ServoMotor::~ServoMotor() {}

void ServoMotor::SetupMotorA() 
{
    SetupTimer2Channel1(); //De pwm output timer van servo 1 
}

void ServoMotor::SetupMotorB() 
{
    SetupTimer2Channel2(); //De pwm output timer van servo 2
}