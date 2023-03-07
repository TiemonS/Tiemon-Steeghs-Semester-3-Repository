#include <stdexcept>
using namespace std;

#include "Car.h"

Car::Car(string manufacturer, string model, 
    int buildYear, string licencePlate, double pricePerKm) 
{
    if ((manufacturer.empty() == true) || (model.empty() == true) || (licencePlate.empty() == true))
    {
        throw invalid_argument("One or more arguments are not valid!");
    }
    if (buildYear < 1890)
    {
        throw out_of_range("buildYear, Cars cannot be built before 1890!");
    }
    if (pricePerKm < 0)
    {
        throw out_of_range("pricePerKm, Cannot be smaller than 0");
    }

    this->manufacturer = manufacturer;
    this->model = model;
    this->buildYear = buildYear;
    this->licencePlate = licencePlate;
    this->kilometers = 0;
    this->isAvailable = true;
    this->pricePerKm = pricePerKm;
}

bool Car::Rent()
{
    if (!isAvailable)
    {
        return false;
    }

    isAvailable = false;
    return true;
}

double Car::Return(int kilometers)
{
    if (isAvailable)
    {
        throw invalid_argument("car was not rented");
    }
    if (kilometers < kilometers)
    {
        throw invalid_argument("car is returned with less kilometers than it had" + kilometers);
    }

    double cost = pricePerKm * (kilometers - this->kilometers);
    this->kilometers = kilometers;
    isAvailable = true;
    return cost;
}

string Car::GetManufacturer() 
{
    return manufacturer;
}
string Car::GetModel() 
{
    return model;
}
int Car::GetBuildYear() 
{
    return buildYear;
}
string Car::GetLicencePlate() 
{
    return licencePlate;
}
int Car::GetKilometers() 
{
    return kilometers;
}
bool Car::GetIsAvailable() 
{
    return isAvailable;
}
bool Car::GetNeedsCleaning() 
{
    return NeedsCleaning;
}

string Car::ToString()
{
    return manufacturer +
            " - " + model +
            ", " + licencePlate;
}
