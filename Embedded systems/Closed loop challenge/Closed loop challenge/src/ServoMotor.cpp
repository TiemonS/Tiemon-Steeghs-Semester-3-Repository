#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>

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

int ServoMotor::mapOutput(int output, int fromLow, int fromHigh, int toLow, int toHigh)
{
    return (output - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}

void ServoMotor::MoveServoA(int Speed, bool ClockWise) 
{
    if (ClockWise)  // speed omzetten naar de waarde voor vooruit
    {
        TIM2->CCR1 = mapOutput(Speed, 0, 250, 1480, 1280);
        
    }
    else  // achteruit
    {
        TIM2->CCR1 = mapOutput(Speed, 0, 250, 1520, 1720);
    }
}

void ServoMotor::MoveServoB(int Speed, bool ClockWise) 
{
    if (ClockWise)  // speed omzetten naar de waarde voor vooruit
    {
        TIM2->CCR2 = mapOutput(Speed, 0, 250, 1520, 1720);
        
    }
    else  // achteruit
    {
        TIM2->CCR2 = mapOutput(Speed, 0, 250, 1480, 1280);
    }
}