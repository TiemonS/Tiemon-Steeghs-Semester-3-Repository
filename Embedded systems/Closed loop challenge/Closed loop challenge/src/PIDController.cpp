#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>

#include "PIDController.hpp"

PIDController::PIDController() {}
PIDController::~PIDController() {}

void PIDController::Compute() 
{
   // Calculate the error
   error = input - setpoint;

  // Calculate the proportional term
   proportional = kp * error;

  // Calculate the integral term
  integral += ki * error;

  // Anti-windup - limit the integral value within a range
  clamp(integral, -maxIntegral, maxIntegral);

  // Calculate the derivative term
    derivative = kd * (error - lastError);

  if (derivative < -3 && integral > 75)
  {
    integral -= 74;
  }

  // Calculate the control output
    output = proportional + integral + derivative;

  // Update the previous error
  lastError = error;
}

int PIDController::clamp(int value, int minValue, int maxValue) {
  if (value < minValue) {
    return minValue;
  } else if (value > maxValue) {
    return maxValue;
  } else {
    return value;
  }
}

// Mapping function
int PIDController::mapOutput(double output, int minSpeed, int maxSpeed, double inputMin, double inputMax)
{
  // Map the output range to the input range
  double mappedInput = (output - inputMin) * (1.0) / (inputMax - inputMin);

  // Map the input range (0 to 1) to the servo speed range (minSpeed to maxSpeed)
  int mappedOutput = static_cast<int>((mappedInput) * (maxSpeed - minSpeed) + minSpeed);

  // Constrain the mapped output within the servo speed range
  mappedOutput = clamp(mappedOutput, minSpeed, maxSpeed);

  return mappedOutput;
}

int PIDController::mapProportional(double proportional, double inputMin, double inputMax) {
  // Map the proportional value to the servo speed range
  int servoSpeed = 0;

  if (proportional < 0.0) {
    // Map the negative proportional values to the clockwise servo speed range
    servoSpeed = static_cast<int>((proportional / inputMin) * (1480 - 1280) + 1280);
  } else if (proportional > 0.0) {
    // Map the positive proportional values to the counter-clockwise servo speed range
    servoSpeed = static_cast<int>((proportional / inputMax) * (1720 - 1520) + 1520);
  } else {
    // Set the servo speed to the stop position
    servoSpeed = 1480;
  }

  return servoSpeed;
}

void PIDController::SetControllerParamters(double Kp, double Ki, double Kd) 
{
    kp = Kp;
    ki = Ki;
    kd = Kd;
}

// Getter methods
double PIDController::GetKp() const
{
    return kp;
}

double PIDController::GetKi() const
{
    return ki;
}

double PIDController::GetKd() const
{
    return kd;
}

double PIDController::GetSetpoint() const
{
    return setpoint;
}

double PIDController::GetOutput() const
{
    return output;
}

double PIDController::GetInput() const
{
    return input;
}

double PIDController::GetError() const
{
    return error;
}

double PIDController::GetProportional() const
{
    return proportional;
}

double PIDController::GetIntegral() const
{
    return integral;
}

double PIDController::GetDerivative() const
{
    return derivative;
}

// Setter methods
void PIDController::SetSetpoint(double setpoint)
{
    this->setpoint = setpoint;
}

void PIDController::SetOutput(double output)
{
    this->output = output;
}

void PIDController::SetInput(double input)
{
    this->input = input;
}