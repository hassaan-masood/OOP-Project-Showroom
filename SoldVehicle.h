#ifndef SOLDVEHICLE_H
#define SOLDVEHICLE_H

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

int number_of_models (string file_name) {
    int num_of_lines = 0;
    string temp;

    ifstream fin (file_name);
    if (!fin) {
        throw "Can't Open this File.\n";
    }

    while (!fin.eof()) {
        getline (fin, temp,'\n');
        num_of_lines++;
    }
    fin.close();

    num_of_lines -= 2;
    return num_of_lines;
    
}

class SoldVehicle {
    string plateNumber;
    string modelID;
    string vehicleType;
    string brand;
    string modelName;
    string customerID;
    string salesmanID;
    string saleDate;
    double salePrice;
 
public:

    SoldVehicle ();

    SoldVehicle(string _plateNumber, string _modelID, string _vehicleType,
    string _brand, string _modelName, string _customerID,
    string _salesmanID, string _saleDate, double _salePrice);
 
    static SoldVehicle* getSoldVehicle(const string &__numPlate);

    void dispSoldVec ();
    
    string getPlateNumber() const;

    string getModelID() const;

    string getVehicleType() const;

    string getBrand() const;

    string getModelName() const;

    string getCustomerID() const;

    string getSalesmanID() const;

    string getSaleDate() const;

    double getSalePrice() const;
};

#endif