#include "stm32f3xx_hal.h"

#ifndef DISTANCESENSOR_HPP
#define DISTANCESENSOR_HPP

class DistanceSensor
{
private:
    int TriggerPin;
    int EchoPin;

    unsigned long RisingEdgeTime = 0;
    unsigned long PulsTravelTime = 0;

public:
    DistanceSensor();
    ~DistanceSensor();

    void SetupTriggerTimer();
    void SetupEchoTimer();
    void SetupEchoInterrupt();

    int CalculateDistance(int PulsTravelTime); 
    void InterruptHandler();
};


#endif