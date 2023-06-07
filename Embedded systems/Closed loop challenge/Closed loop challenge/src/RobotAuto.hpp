#include "CarTimer.h"
#include "PIDController.hpp"
#include "ServoMotor.hpp"
#include "DistanceSensor.hpp"
#include "UIManager.hpp"

#ifndef ROBOTAUTO_HPP
#define ROBOTAUTO_HPP

class RobotAuto
{
private:
    PIDController pidController;
    DistanceSensor distanceSensor;
    ServoMotor servoMotorA;
    ServoMotor servoMotorB;
public:
    RobotAuto(PIDController pidController, DistanceSensor distanceSensor, ServoMotor servoMotorA, ServoMotor servoMotorB);
    ~RobotAuto();

    // Getter methods
    DistanceSensor getDistanceSensor() const;
    PIDController getPIDController() const;
    ServoMotor getServoMotorA() const;
    ServoMotor getServoMotorB() const;
};


#endif