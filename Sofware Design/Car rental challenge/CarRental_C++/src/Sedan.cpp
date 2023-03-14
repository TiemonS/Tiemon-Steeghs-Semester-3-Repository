#include "Sedan.h"
#include "Car.h"
#include <stdexcept>

using namespace std;

Sedan::Sedan(string manufacturer, string model, int buildYear, string licencePlate, bool hasTowbar)
: Car(manufacturer, model, buildYear, licencePlate, pricePerKm)
{
    this->hasTowbar = hasTowbar;
    lastCleanedAtKm = 0;
}

void Sedan::Clean()
{
    lastCleanedAtKm = Car::GetKilometers();
}