#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>

#include "PIDController.hpp"

void PIDController::Compute() 
{
    /*How long since we last calculated*/
   unsigned long now = HAL_GetTick();
   double timeChange = (double)(now - lastTime);
  
   /*Compute all the working error variables*/
   double error = setpoint - input;
   integral += (error * timeChange);
   derivative = (error - lastError) / timeChange;
  
   /*Compute PID Output*/
   output = kp * error + ki * integral + kd * derivative;
  
   /*Remember some variables for next time*/
   lastError = error;
   lastTime = now;
}

void PIDController::SetControllerParamters(double Kp, double Ki, double Kd) 
{
    kp = Kp;
    ki = Ki;
    kd = Kd;
}