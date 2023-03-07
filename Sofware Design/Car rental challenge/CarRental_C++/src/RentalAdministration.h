#include "Car.h"
#include "vector"

#ifndef RENTALADMINISTRATION_H
#define RENTALADMINISTRATION_H

class RentalAdministration
{
private:
    vector<Car> Cars;

    Car FindCar(string licencePlate);
    Car FindCarWithException(string licencePlate);
    
public:
    RentalAdministration();
    bool Add(Car car);
    bool RentCar(string lisencePlate);
    double ReturnCar(string liscenePlate, int kilometers);
    void CleanCar(string licencePlate);

};


#endif

