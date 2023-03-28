#include "Wheel.h"
#include <string>
#include <iostream>
#include <stdexcept>

Wheel::Wheel(int diameter, const std::string& material) 
{
    if (diameter < 1)
    {
        throw std::out_of_range("diameter, diameter needs to be atleast 1!");
    }
    
    this->diameter = diameter;
    this->material = material;
}

std::string Wheel::GetMaterial() const 
{
    return material;
}

void Wheel::SetMaterial(const std::string& material) 
{
    this->material == material;
}

int Wheel::GetDiameter() const 
{
    return diameter;
}
