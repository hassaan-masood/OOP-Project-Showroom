#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "Person.h"
#include "Inventory.h"
#include "SalesManager.h"
#include "RepairManager.h"
#include "SoldVehicle.h"
#include "RepairManager.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>

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
    if (num_of_lines < 0) {
        return 0;
    } 

    return num_of_lines;
}


class Customer : public Person {
    string cnic;
    string address;
    string registration_date;

public:
    Customer ();

    Customer(string _id, string _name, string _email, string _phone,
    string _username, string _password, string _cnic,
    string _address, string _registration_date);

    Customer (const Customer &other);

    void showMenu() override;

    string getRole() const override;

    string getPasscode () const;

    string get_Date () const;

    void disp_Info () const override;

    string getCNIC() const;
    string getAddress() const;
 
    void browse_cars(Inventory &inv) const;

    void browse_bikes(Inventory &inv) const;

    void browse_scooties(Inventory &inv) const;

    void browse_vehicles(Inventory &inv) const;

    void viewMyPurchases(SalesManager &sm) const;

    void viewMyRepairHistory(RepairManager &rm) const;

    void requestVehicleRepair(RepairManager &rm, const string &plateNumber,
    const string &vehicleType, const string &partChanged);
 
    static Customer* getCustomer_ById (string __id);
};

#endif