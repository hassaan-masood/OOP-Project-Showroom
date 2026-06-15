#ifndef SALERECORD_H
#define SALERECORD_H

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class SaleRecord {
    string saleID;
    string modelID;
    string vehicleType;
    string customerID;
    string salesmanID;
    string saleDate;
    double salePrice;
 
public:
    SaleRecord ();

    SaleRecord(string _saleID, string _modelID, string _vehicleType,
    string _customerID, string _salesmanID, string _saleDate, double _salePrice);
 
    static SaleRecord* getSaleRecord(const string &__saleID);
    
    void dispRec ();
    
    string getSaleID() const;

    string getModelID() const;

    string getVehicleType() const;

    string getCustomerID() const;

    string getSalesmanID() const;

    string getSaleDate() const;
    double getSalePrice() const;
};

#endif