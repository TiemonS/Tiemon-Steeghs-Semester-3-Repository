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

    bool needsCleaning;

public:

    Car(string manufacturer, string model, int buildYear, string licencePlate, double pricePerKm);
    
    bool Rent();
    double Return(int kilometers);
    virtual void Clean();

    virtual bool NeedsCleaning();

    string GetManufacturer() const;
    string GetModel() const;
    int GetBuildYear() const;
    string GetLicencePlate() const;
    int GetKilometers() const;
    bool GetIsAvailable() const;

    bool GetNeedsCleaning() const;

    string ToString() const;
};


#endif