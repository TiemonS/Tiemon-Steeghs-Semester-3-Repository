//============================================================================
// Name        : menu.cpp
// Author      : Freddy Hurkmans
// Version     :
// Copyright   : copyright Freddy Hurkmans
// Description : Assignment 1 example
//============================================================================

#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

#include "RentalAdministration.h"
#include "Limousine.h"
#include "Sedan.h"

void addTestDataToAdministration(RentalAdministration* administration)
{
    string licencePlates[] = {"SD-001", "SD-002", "SD-003", "SD-004", "LM-001", "LM-002"};

    for (int i = 0; i < 4; i++)
    {
        Car* sedan = new Sedan("BMW", "535d", 2012 + i, licencePlates[i], false);
        administration->Add(sedan);
    }
    for (int i = 4; i < 6; i++)
    {
        Car* limousine = new Limousine("Rolls Roys", "Phantom Extended Wheelbase", 2015, licencePlates[i], true);
        administration->Add(limousine);
    }
}

static const Car* GetCurrentCar(const RentalAdministration* administration, size_t carNumber)
{
    vector<Car*> CarVector = administration->GetCars();

    if (carNumber > CarVector.size())
    {
        cout << "\nError: car with car number " << carNumber << " does not exist!\n";
        return NULL;
    }

    for (size_t i = 0; i < CarVector.size(); i++)
    {
       if (carNumber == i)
       {
        return CarVector[i];
       }
       
    }
    
    return NULL; // return the correct car here
}

static void PrintCars(const RentalAdministration* administration)
{
    // pseudo code (i.e.: not real C++ code):
    // int i = 1;
    // foreach (car in administration->GetCars())
    // {
    //     cout << "\n" << i << ": " << car.ToString;
    //     i++;
    // }
    vector<Car*> CarVector = administration->GetCars();

    for (size_t i = 0; i < CarVector.size(); i++)
    {
        cout << "\n" << i << ": " << CarVector[i]->ToString();
    }
    
}

static size_t selectCar(const RentalAdministration* administration)
{
    size_t carNumber = 0;
    cout << "These cars are currently listed:";
    PrintCars(administration);
    cout << "\nPlease enter a car number: ";
    std::cin >> carNumber;
    std::cin.ignore();
    return carNumber;
}

static void rentCar(RentalAdministration* administration, size_t carNumber)
{
    const Car* curr = GetCurrentCar(administration, carNumber);
    
    if (administration->RentCar(curr->GetLicencePlate()))
    {
        cout << "Car " << curr->ToString() << " rented";
    }
    else
    {
        cout << "ERROR: car " << curr->ToString() << " is not available!";
    }
    
    // rent this car, functionality comparable to btnRent_Click

    
}

static void returnCar(RentalAdministration* administration, size_t carNumber)
{
    const Car* curr = GetCurrentCar(administration, carNumber);

    try
    {
        double costs = administration->ReturnCar(curr->GetLicencePlate(), curr->GetKilometers());
        cout << "Car " << curr->ToString() << " returned, price: " << costs;

    }
    catch(invalid_argument& exception)
    {
        cout << "ERROR: car " << curr->ToString() << " - " << exception.what();
    }
    
    
    // return this car, do handle exceptions
}

static void printIfCarNeedsCleaning(const RentalAdministration* administration, size_t carNumber)
{
    const Car* curr = GetCurrentCar(administration, carNumber);
    if (curr->NeedsCleaning())
    {
        cout << curr->GetLicencePlate() << "Car needs cleaning!";
    }
    else
    {
        cout << curr->GetLicencePlate() << "Car does not need cleaning!";
    }

    // print if this car needs cleaning
}

static void cleanCar(RentalAdministration* administration, size_t carNumber)
{
    const Car* curr = GetCurrentCar(administration, carNumber);
    administration->CleanCar(curr->GetLicencePlate());
    // clean this car, see: btnClean_Click
}

static void freeAdministration(RentalAdministration* administration)
{
    vector<Car*> CarVector = administration->GetCars();
    for (size_t i = 0; i < CarVector.size(); i++)
    {
        Car* car = CarVector[i];
        delete car;
    }
}


static void showMenu( void )
{
    cout << ("\n\nCar Rental menu\n");
    cout << ("===============\n");
    cout << ("(1) Show all cars\n");
    cout << ("(2) Select car\n");
    cout << ("(3) Rent selected car\n");
    cout << ("(4) Return selected car\n");
    cout << ("(5) Check if car needs cleaning\n");
    cout << ("(6) Clean car\n");
    cout << ("-----------------------\n");
    cout << ("(9) QUIT\n\n");
    cout << ("Choice : ");
}

int main( void )
{
    bool quit = false;
    size_t carNumber = 0;

    RentalAdministration administration;
    addTestDataToAdministration(&administration);

    while (!quit)
    {
        char choice = '\0';
        showMenu();
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case '1' :
            PrintCars(&administration);
            break;
        case '2' :
            carNumber = selectCar(&administration);
            break;
        case '3' :
            rentCar(&administration, carNumber);
            break;
        case '4' :
            returnCar(&administration, carNumber);
            break;
        case '5' :
            printIfCarNeedsCleaning(&administration, carNumber);
            break;
        case '6' :
            cleanCar(&administration, carNumber);
            break;

        case '9' :
            freeAdministration(&administration);
            quit = true;
            break;
        default:
            cout << "\n\nI did not understand your choice (" << choice << ")" << endl;
            break;
        }
    }

    return 0;
}
