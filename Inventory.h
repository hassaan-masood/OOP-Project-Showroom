#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "Vehicle.h"
#include "Car.h"
#include "Bike.h"
#include "Scooty.h"

using namespace std;

class Inventory {
private:
    vector<Car> cars;
    vector<Bike> bikes;
    vector<Scooty> scooties;
    vector <string> intro_lines;
 
public:
    void loadAll();

    void Changeable_disp (string type = "NULL");

    void saveAll() const;

    vector<Vehicle*> getAllVehicles();

    Vehicle* findByModelID(const string &modelID);
 
    void addCar(const Car &c);

    void addBike(const Bike &b);

    void addScooty(const Scooty &s);

    bool removeModel(const string &modelID);

    void updateQuantity(const string &modelID, int newQuantity);

    vector<Car*> getCars();

    vector<Bike*> getBikes();

    vector<Scooty*> getScooties();
};

#endif