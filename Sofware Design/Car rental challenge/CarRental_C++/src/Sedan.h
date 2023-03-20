#include <stdexcept>
#include <string>
#include "Car.h"
using namespace std;

#ifndef SEDAN_H
#define SEDAN_H

class Sedan : public Car
{
private:
    int lastCleanedAtKm;
    double pricePerKm = 0.29;
    bool hasTowbar;
    bool needscleaning;
public:
    Sedan(string manufacturer, string model, int buildYear, 
    string licencePlate, bool hasTowbar); 
    void Clean();
    bool NeedsCleaning() const;
};

#endif

