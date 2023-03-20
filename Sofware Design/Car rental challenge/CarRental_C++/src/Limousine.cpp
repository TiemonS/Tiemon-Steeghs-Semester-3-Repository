#include "Limousine.h"
#include "Car.h"
#include <stdexcept>
#include <iostream>
#include <string>

using namespace std;


Limousine::Limousine(string manufacturer, string model, int buildYear, string licensePlate, bool hasMinibar)
    : Car(manufacturer, model, buildYear, licensePlate, 2.5) 
{
    this->hasMinibar = hasMinibar; 
    this->needsCleaning = false; 
}

double Limousine::Return(int kilometers)
{
    double price = this->Car::Return(kilometers);
    needsCleaning = true;
    return price;
}

void Limousine::Clean()
{
    needsCleaning = false;
}

bool Limousine::NeedsCleaning() const 
{
    return needsCleaning;
}

bool Limousine::GetHasMinibar() const
{
    return hasMinibar;
}