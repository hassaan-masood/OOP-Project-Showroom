#ifndef SCOOTY_H
#define SCOOTY_H

#include "Vehicle.h"
#include <iostream>
#include <fstream>
using namespace std;

class Scooty : public Vehicle {
    double storageCapacityLiters;
 
public:
    Scooty ();

    Scooty(string modelID, string brand, string modelName, int year, string color, double price, int quantity,
    bool isElectric, int engineCC, double mileageKmpl, string fuelType, double _ssl);

    Scooty(const Scooty& other);
 
    void displayInfo() const override;

    string getType() override;
 
    double get_storageCapacityLiters() const;

    static Scooty* getScootyModel(const string &__modelID);
};

#endif