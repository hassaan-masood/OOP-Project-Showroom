#ifndef SALESMANAGER_H
#define SALESMANAGER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "SaleRecord.h"
#include "SoldVehicle.h"

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

class SalesManager {
    vector<SaleRecord> sales;
    vector<SoldVehicle> soldVehicles;
    vector <string> intro_lines;
 
public:
    void loadAll();

    void saveAll() const;
 
    string generate_PlateNumber() const;
 
    void record_Sale(const SaleRecord &sr, const SoldVehicle &sv);
 
    vector<SoldVehicle> get_Purchases_By_Customer(const string &customerID) const;

    vector<SaleRecord> get_Sales_By_Salesman(const string &salesmanID) const;
 
    int get_Total_Sales_Count() const;

    double get_Total_Revenue() const;
};

#endif