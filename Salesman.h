#ifndef SALESMAN_H
#define SALESMAN_H

#include "Employee.h"
#include "Inventory.h"
#include "Customer.h"
#include "SalesManager.h"
#include "Vehicle.h"
#include "SoldVehicle.h"
#include "SaleRecord.h"
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

class Salesman : public Employee {
    int sales_ct;
 
public:
    Salesman ();

    Salesman(string _id, string _name, string _email, string _phone,
    string _username, string _password, string _cnic,
    string _hireDate, double _Salary, int _ct);

    Salesman (const Salesman &other);
 
    void showMenu() override;

    string getRole() const override;

    void disp_Info () const override;
 
    int getSalesCount() const;

    string getPasscode () const;

    void incrementSalesCount();
 
    void sellVehicle(Inventory &inv, Customer &cust, SalesManager &sm,
    const string &modelID);

    void viewMyProfile() const;

    void viewMySalesHistory(SalesManager &sm) const;
    
    static Salesman* getSalesman_ById (string __id);
};

#endif