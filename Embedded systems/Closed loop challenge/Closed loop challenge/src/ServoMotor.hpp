#include "../Interfaces/IServoMotor.hpp"

#ifndef SERVOMOTOR_HPP
#define SERVOMOTOR_HPP

//De ServoMotor class
// 1280 - 1480 Clockwise
// 1480 - 1520 Stop
// 1520 - 1720 Counter-clockwise 
class ServoMotor : public IServoMotor
{
private:
    /* data */
public:
    ServoMotor();
    ~ServoMotor();
    void SetupMotorA();
    void SetupMotorB();

    void MoveServoA(int Speed, bool ClockWise); 
    void MoveServoB(int Speed, bool ClockWise); 

    int mapOutput(int output, int fromLow, int fromHigh, int toLow, int toHigh);
};


#endif