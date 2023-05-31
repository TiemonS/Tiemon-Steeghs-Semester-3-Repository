#ifndef PIDCONTROLLER_HPP
#define PIDCONTROLLER_HPP

class PIDController
{
private:

    unsigned long lastTime;
    // PID-controller parameters
    double kp = 5.0; //Proportional term control parameter (nu)
    double ki = 0.1; //Integral term control parameter (verleden)
    double kd = 0.5; //Derivative term control parameter (toekomst)

    // PID-controller variables
    double setpoint = 0.0; //de desiredoutput het doel wat de controller wilt berijken
    double output = 0.0; //de output van de PID controller
    int mappedOutput = 0; //de output van de PID controller gemapped naar een waarde voor de servo
    double input = 0.0; //de input van de controller, in dit geval de ultrasonische sensor

    double error = 0.0; //de error is de waarde die de output afwijkt van de desired output
    double lastError = 0.0; //de vorige error waarde wordt hier in opgeslagen

    double proportional = 0.0;
    double integral = 0.0;
    double derivative = 0.0;

    double maxIntegral = 200.0;
    
public:
    PIDController();
    ~PIDController();

    /*De PID controller update methode. Hierin wordt als eerste de error berekend en daarna de PID Output
    De PID output is wordt dan weer berekend aan de hand van de controller parameters.*/
    void Compute(); 
    void SetControllerParamters(double Kp, double Ki, double Kd);
    int mapOutput(double output, int minSpeed, int maxSpeed, double inputMin, double inputMax);
    int mapProportional(double proportional, double inputMin, double inputMax);

    // Getter methods
    double GetKp() const;
    double GetKi() const;
    double GetKd() const;
    double GetSetpoint() const;
    double GetOutput() const;
    double GetInput() const;
    double GetError() const;
    double GetProportional() const;
    double GetIntegral() const;
    double GetDerivative() const;

    // Setter methods
    void SetSetpoint(double setpoint);
    void SetOutput(double output);
    void SetInput(double input);
};

#endif