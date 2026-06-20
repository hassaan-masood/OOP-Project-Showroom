#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include "Inventory.h"
#include "SalesManager.h"
#include "RepairManager.h"
#include "Employee.h"
#include "Car.h"
#include "Bike.h"
#include "Scooty.h"
#include "Salesman.h"
#include "RepairMan.h"
#include "Customer.h"
#include "RepairRecord.h"

using namespace std;

class Admin {
    string id;
    string name;
    string username;
    string password;

public:
    Admin ();

    Admin(string _id, string _name, string _username, string _password);
 
    void showMenu();

    string getRole() const;

    string getUsername() const;

    bool checkPassword(const string &inp) const;

    void addCar(Inventory &inv);

    void addBike(Inventory &inv);

    void addScooty(Inventory &inv);

    void removeVehicleModel(Inventory &inv, const string &modelID);

    void updateStock(Inventory &inv, const string &modelID, int newQuantity);
 
    void addSalesman(vector<Salesman> &salesmen);

    void removeSalesman(vector<Salesman> &salesmen, const string &salesmanID);

    void addRepairMan(vector<RepairMan> &repairmen);

    void removeRepairMan(vector<RepairMan> &repairmen, const string &repairManID);

    void paySalary(Employee &emp, double amount);
 
    void addCustomer(vector<Customer> &customers);

    void removeCustomer(vector<Customer> &customers, const string &customerID);
 
    void viewSalesReport(SalesManager &sm) const;

    void viewAllVehicles(Inventory &inv) const;

    void viewAllSalesmen(const vector<Salesman> &salesmen) const;

    void viewAllRepairmen(const vector<RepairMan> &repairmen) const;

    void viewAllCustomers(const vector<Customer> &customers) const;

    void viewPendingRepairs(RepairManager &rm) const;
 
    static Admin* getAdmin ();
};

#endif