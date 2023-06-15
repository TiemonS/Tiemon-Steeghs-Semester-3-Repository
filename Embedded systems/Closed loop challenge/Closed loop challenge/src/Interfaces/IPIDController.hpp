#ifndef IPIDCONTROLLER_HPP
#define IPIDCONTROLLER_HPP

class IPIDController {
public:
    virtual ~IPIDController() {}

    virtual void Compute(double input) = 0;
    virtual void SetControllerParameters(double Kp, double Ki, double Kd) = 0;
    virtual int mapOutput(double output, int minSpeed, int maxSpeed, double inputMin, double inputMax) = 0;
    virtual int mapServosOutput(int *servoAValue, int *servoBValue) = 0;

    // Getter methods
    virtual double GetKp() const = 0;
    virtual double GetKi() const = 0;
    virtual double GetKd() const = 0;
    virtual double GetSetpoint() const = 0;
    virtual double GetOutput() const = 0;
    virtual double GetInput() const = 0;
    virtual double GetError() const = 0;

    // Setter methods
    virtual void SetSetpoint(double setpoint) = 0;
    virtual void SetOutput(double output) = 0;
};

#endif
