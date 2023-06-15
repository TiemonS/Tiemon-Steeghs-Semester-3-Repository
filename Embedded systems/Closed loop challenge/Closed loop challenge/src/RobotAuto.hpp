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
    IPIDController& pidController;
    IDistanceSensor& distanceSensor;
    IServoMotor& servoMotorA;
    IServoMotor& servoMotorB;
public:
    RobotAuto(IPIDController& pidController, IDistanceSensor& distanceSensor, IServoMotor& servoMotorA, IServoMotor& servoMotorB);
    ~RobotAuto();

    // Getter methods
    IDistanceSensor& getDistanceSensor() const;
    IPIDController& getPIDController() const;
    IServoMotor& getServoMotorA() const;
    IServoMotor& getServoMotorB() const;

    void HandleInput(int choice) ;
};


#endif