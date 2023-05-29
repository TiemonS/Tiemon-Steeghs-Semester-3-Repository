#ifndef PIDCONTROLLER_HPP
#define PIDCONTROLLER_HPP

class PIDController
{
private:

    unsigned long lastTime;
    // PID-controller parameters
    double kp = 1.0; //Proportional term control parameter (nu)
    double ki = 0.2; //Integral term control parameter (verleden)
    double kd = 0.5; //Derivative term control parameter (toekomst)

    // PID-controller variables
    double setpoint = 0.0; //de desiredoutput het doel wat de controller wilt berijken
    double output = 0.0; //de output van de PID controller
    double input = 0.0; //de input van de controller, in dit geval de ultrasonische sensor

    double error = 0.0; //de error is de waarde die de output afwijkt van de desired output
    double lastError = 0.0; //de vorige error waarde wordt hier in opgeslagen

    double proportional = 0.0;
    double integral = 0.0;
    double derivative = 0.0;
    
public:
    PIDController();
    ~PIDController();

    /*De PID controller update methode. Hierin wordt als eerste de error berekend en daarna de PID Output
    De PID output is wordt dan weer berekend aan de hand van de controller parameters.*/
    void Compute(); 
    void SetControllerParamters(double Kp, double Ki, double Kd);
};

#endif