#include "Car.h"
#include "Wheel.h"

#include <iostream>
#include <stdexcept>

#ifdef SHOW_CTORS
    #define TRACE(txt) std::cout << txt << std::endl
#else
    #define TRACE(txt)
#endif


Car::Car(const std::string& model, const std::string& material, int diameter,
         int nrWheels)
{
    TRACE("car constructor called!");
    
    if (diameter < 1)
    {
        throw std::out_of_range("diameter, diameter cannot be smaller then 1");
    }
    else if(nrWheels < 2) 
    {
        throw std::out_of_range("nrWheels, Truck needs to have atleast two wheels!");
    }

    for (int i = 0; i < nrWheels; i++)
    {
        wheels.push_back(new Wheel(diameter, material));
    }
    
    this->model = model;
}

Car::Car(const Car& other)
{
    TRACE("car copy constructor called!");

    model = other.model;
    licencePlate = other.licencePlate;
    for (Wheel* wheel : other.wheels)
    {
        wheels.push_back(new Wheel(*wheel));
    }
}

Car::~Car()
{
    TRACE("car destructor called!");
    for (Wheel* wheel : wheels)
    {
        delete wheel;
    }
}

 Car& Car::operator=(const Car& other) 
 {
    //eerst kijken of niet per ongeluk twee dezelfde car's aan elkaar gelijk worden gemaakt
    if (&other != this)
    {
        model = other.model;
        licencePlate = other.licencePlate;
        for (Wheel* wheel : wheels)
        {
            delete wheel;
        }
        wheels.clear();
        for (Wheel* wheel : other.wheels)
        {
            wheels.push_back(new Wheel(*wheel));
        }
    }
    return *this;
 }

std::string Car::GetModel() const 
{
    return model;
}

void Car::SetLicencePlate(const std::string& licence)
{
    licencePlate = licence;
}

std::string Car::GetLicencePlate() const
{
    return licencePlate;
}

int Car::GetNrWheels() const
{
    return wheels.size();
}

const Wheel* Car::GetWheel(int index) const
{
    if (index >= 0 && index < (int)wheels.size())
    {
        return wheels[index];
    }
    else
    {
        throw std::out_of_range("Not an valid index");
    }
}

void Car::RemoveWheel(int index)
{
    if (index >= 0 && index < (int)wheels.size())
    {
        delete wheels[index];
        wheels.erase(wheels.begin() + index);
    }
    else
    {
        throw std::out_of_range("Not an valid index");
    }
}

void Car::AddWheel(int diameter, const std::string& material)
{
    wheels.push_back(new Wheel(diameter, material));
}