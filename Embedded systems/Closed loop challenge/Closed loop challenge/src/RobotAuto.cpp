#include "RobotAuto.hpp"
#include "../Interfaces/IDistanceSensor.hpp"
#include "../Interfaces/IPIDController.hpp"
#include "../Interfaces/IServoMotor.hpp"

RobotAuto::RobotAuto(IPIDController& pidController, IDistanceSensor& distanceSensor, IServoMotor& servoMotorA, IServoMotor& servoMotorB) 
: pidController(pidController), distanceSensor(distanceSensor), servoMotorA(servoMotorA), servoMotorB(servoMotorB) 
{
    this->pidController = pidController;
    this->distanceSensor = distanceSensor;
    this->servoMotorA = servoMotorA;
    this->servoMotorB = servoMotorB;
}

void RobotAuto::HandleInput(int choice) 
{
    int mapOutput;
    int mapOutput2;
    int distance;
    switch (choice)
    {
    case 1:
        servoMotorA.MoveServoA(250, false);
        servoMotorB.MoveServoB(250, false);
        break;
    case 2:
        servoMotorA.MoveServoA(0, false);
        servoMotorB.MoveServoB(0, false);
        break;
    case 3:
        //uImanager.handleUserInput(choice, &huart2);
        distance = this->distanceSensor.CalculateDistance();
        //uImanager.printDistance(distance, &huart2);
        this->pidController.Compute(double(distance));

        this->pidController.mapServosOutput(&mapOutput, &mapOutput2);

        TIM2->CCR1 = mapOutput;
        TIM2->CCR2 = mapOutput2;
        break;
    default:
        break;
    }
}


// Getter methods
IDistanceSensor& RobotAuto::getDistanceSensor() const
{
    return distanceSensor;
}

IPIDController& RobotAuto::getPIDController() const
{
    return pidController;
}

IServoMotor& RobotAuto::getServoMotorA() const
{
    return servoMotorA;
}

IServoMotor& RobotAuto::getServoMotorB() const
{
    return servoMotorB;
}