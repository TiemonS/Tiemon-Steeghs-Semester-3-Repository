#include "Sedan.h"
#include "Car.h"
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

Sedan::Sedan(string manufacturer, string model, int buildYear, string licencePlate, bool hasTowbar)
: Car(manufacturer, model, buildYear, licencePlate, 0.29)
{
    this->hasTowbar = hasTowbar;
    lastCleanedAtKm = 0;
}

void Sedan::Clean()
{
    lastCleanedAtKm = Car::GetKilometers();
}

bool Sedan::NeedsCleaning() const 
{
    return (Car::GetKilometers() - lastCleanedAtKm) >= 1000;
}