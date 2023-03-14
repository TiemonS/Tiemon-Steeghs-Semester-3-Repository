#include "RentalAdministration.h"

#include <stdexcept>
using namespace std;

RentalAdministration::RentalAdministration() 
{
}

bool RentalAdministration::Add(Car *car) 
{
    if (car == nullptr)
    {
        throw invalid_argument("Car cannot be NULL!");
    }

    if (FindCar(car->GetLicencePlate()) != NULL)
    {
        return false;
    }
    Cars.push_back(car);
    return true;
}

bool RentalAdministration::RentCar(string licencePlate)
{
    if (licencePlate.empty())
    {
        throw invalid_argument("LicencePlate cannot be NULL!");
    }

    Car *car = FindCarWithException(licencePlate);
    return car->Rent();
}

double RentalAdministration::ReturnCar(string licencePlate, int kilometers)
{
    if(licencePlate.empty())
    {
        throw invalid_argument("LicencePlate cannot be NULL!");
    }

    Car *car = FindCarWithException(licencePlate);
    if (car->GetIsAvailable())
    {
        throw invalid_argument("car is not rented" + licencePlate);
    }

    return car->Return(kilometers);
}

void RentalAdministration::CleanCar(string licencePlate)
{
    if (licencePlate.empty() == true)
    {
        throw invalid_argument("LicencePlate cannot be NULL!");
    }

    Car *car = FindCarWithException(licencePlate);
    car->Clean();
}

Car* RentalAdministration::FindCar(string licencePlate)
{
    for (long unsigned int i = 0; i < Cars.size(); i++) {
        if (Cars[i]->GetLicencePlate() == licencePlate)
        {
            return Cars[i];
        }
    }
    return NULL;
}

Car* RentalAdministration::FindCarWithException(string licencePlate)
{
    Car *car = FindCar(licencePlate);
    if (car == nullptr)
    {
        throw new invalid_argument(licencePlate + "no car with this licenceplate in administration");
    }
    return car;
}

const vector<Car*> RentalAdministration::GetCars() const
{
    return Cars;
}