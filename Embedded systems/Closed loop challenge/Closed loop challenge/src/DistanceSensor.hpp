#include "stm32f3xx_hal.h"
#include "../Interfaces/IDistanceSensor.hpp"

#ifndef DISTANCESENSOR_HPP
#define DISTANCESENSOR_HPP

class DistanceSensor : public IDistanceSensor
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

    int CalculateDistance(); 
    void InterruptHandler();

    unsigned long GetRisingEdgeTime() const;
};


#endif