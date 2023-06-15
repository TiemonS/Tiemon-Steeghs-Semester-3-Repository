#ifndef ISERVOMOTOR_HPP
#define ISERVOMOTOR_HPP

class IServoMotor {
public:
    virtual ~IServoMotor() {}

    virtual void SetupMotorA() = 0;
    virtual void SetupMotorB() = 0;

    virtual void MoveServoA(int Speed, bool ClockWise) = 0; 
    virtual void MoveServoB(int Speed, bool ClockWise) = 0; 

    virtual int mapOutput(int output, int fromLow, int fromHigh, int toLow, int toHigh) = 0;
};

#endif