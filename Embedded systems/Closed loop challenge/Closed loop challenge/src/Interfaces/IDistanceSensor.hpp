#ifndef IDISTANCESENSOR_HPP
#define IDISTANCESENSOR_HPP

class IDistanceSensor {
public:
    virtual ~IDistanceSensor() {}

    virtual int CalculateDistance() = 0;
    virtual void SetupEchoInterrupt() = 0;
    virtual void InterruptHandler() = 0;
    virtual void SetupTriggerTimer() = 0;
    virtual void SetupEchoTimer() = 0;

    virtual unsigned long GetRisingEdgeTime() const = 0;

};

#endif
