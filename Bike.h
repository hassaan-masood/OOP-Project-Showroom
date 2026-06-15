#ifndef BIKE_H
#define BIKE_H

#include "Vehicle.h"
#include <iostream>
#include <fstream>
using namespace std;

class Bike : public Vehicle {
    string bikeType; 
 
public:
    Bike ();

    Bike(string modelID, string brand, string modelName, int year, string color, double price, int quantity,
    bool isElectric, int engineCC, double mileageKmpl, string fuelType, string _bike_type);

    Bike(const Bike& other);
 
    void displayInfo() const override;

    string getType() override;
 
    string get_bikeType() const;

    static Bike* getBikeModel(const string &__modelID);
};

#endif