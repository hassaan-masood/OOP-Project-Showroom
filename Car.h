#ifndef CAR_H
#define CAR_H

#include "Vehicle.h"
#include <iostream>
#include <fstream>
using namespace std;

class Car : public Vehicle {
    int seatingCapacity;
    string transmission;
    int numDoors;
 
public:
    Car ();

    Car(string modelID, string brand, string modelName, int year, string color, double price, int quantity,
    bool isElectric, int engineCC, double mileageKmpl, string fuelType, int _seatingCapacity, string _transmission,
    int _numDoors);

    Car (const Car& other);
 
    void displayInfo() const override;

    string getType() override;
 
    int getSeatingCapacity() const;

    string getTransmission() const;

    int getNumDoors() const;
 
    static Car* getCarModel(const string &__modelID);
};

#endif