#include "stm32f3xx_hal.h"

#ifndef DISTANCESENSOR_HPP
#define DISTANCESENSOR_HPP

class DistanceSensor
{
private:
    int TriggerPin;
    int EchoPin;
public:
    DistanceSensor(GPIO_TypeDef* TriggerPort, uint16_t TriggerPin);
    ~DistanceSensor();

    void SetupTriggerTimer();
    void SetupEchoTimer();
    void SetupEchoInterrupt();

    int CalculateDistance(int UnitOfLenght);
};


#endif