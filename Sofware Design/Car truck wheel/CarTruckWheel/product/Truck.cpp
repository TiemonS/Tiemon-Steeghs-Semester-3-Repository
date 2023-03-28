#include "Truck.h"
#include <string>
#include <iostream>
#include <stdexcept>

Truck::Truck(const std::string& model, const std::string& material, int diameter, int nrWheels, int power) :
Car(model, material, diameter, nrWheels) 
{
    if(power < 1) 
    {
        throw std::out_of_range("power, Power needs to be greater than 0!");
    }
    
    this->power = new int(power);
}

Truck::~Truck() 
{
    delete(power);
    power = nullptr;
}

Truck::Truck(const Truck& other) : Car(other)
{
    const std::string& model = other.GetModel();
    power = new int(*other.power);
}

Truck& Truck::operator=(const Truck& other)
{
    if (&other != this)
    {
        Car::operator=(other);
        delete power;
        power = new int(*(other.power));
    }
    return *this;
}

int Truck::GetPower() const
{
    return *power;
}
