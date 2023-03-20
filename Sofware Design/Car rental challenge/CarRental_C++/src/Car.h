#include <stdexcept>
#include <string>
using namespace std;

#ifndef CAR_H
#define CAR_H

class Car
{
private:
    double pricePerKm;

    string manufacturer;
    string model;
    int buildYear;
    string licencePlate;
    int kilometers;
    bool isAvailable;

public:

    Car(string manufacturer, string model, int buildYear, string licencePlate, double pricePerKm);
    virtual ~Car() {}

    bool Rent();
    double Return(int kilometers);
    virtual void Clean() = 0;

    virtual bool NeedsCleaning() const = 0;

    string GetManufacturer() const;
    string GetModel() const;
    int GetBuildYear() const;
    string GetLicencePlate() const;
    int GetKilometers() const;
    bool GetIsAvailable() const;

    string ToString() const;
};


#endif