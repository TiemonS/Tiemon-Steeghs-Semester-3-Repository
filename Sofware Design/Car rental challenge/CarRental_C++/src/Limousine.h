#include <stdexcept>
#include <string>
#include "Car.h"
using namespace std;

#ifndef LIMOUSINE_H
#define LIMOUSINE_H

class Limousine : public Car
{
private:
    double pricePerKm = 2.5;
    bool needsCleaning;
    bool hasMinibar; 
public:
    Limousine(string manufacturer, string model, int buildYear, 
    string licensePlate, bool hasMinibar);
  
    double Return(int kilometers);
    void Clean();


    bool NeedsCleaning() const;
    bool GetHasMinibar() const;
};

#endif


