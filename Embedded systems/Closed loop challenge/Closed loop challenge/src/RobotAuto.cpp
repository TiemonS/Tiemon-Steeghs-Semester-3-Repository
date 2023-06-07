#include "RobotAuto.hpp"

RobotAuto::RobotAuto(PIDController pidController, DistanceSensor distanceSensor, ServoMotor servoMotorA, ServoMotor servoMotorB)
{
    this->pidController = pidController;
    this->distanceSensor = distanceSensor;
    this->servoMotorA = servoMotorA;
    this->servoMotorB = servoMotorB;
}

// Getter methods
DistanceSensor RobotAuto::getDistanceSensor() const
{
    return distanceSensor;
}

PIDController RobotAuto::getPIDController() const
{
    return pidController;
}

ServoMotor RobotAuto::getServoMotorA() const
{
    return servoMotorA;
}

ServoMotor RobotAuto::getServoMotorB() const
{
    return servoMotorB;
}