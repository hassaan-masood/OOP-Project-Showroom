#include "SaleRecord.h"

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

SaleRecord::SaleRecord () {
    this->saleID = "NULL";
    this->modelID = "NULL";
    this->vehicleType = "NULL";
    this->customerID = "NULL";
    this->salesmanID = "NULL";
    this->saleDate = "NULL";
    this->salePrice = 0;
}

SaleRecord::SaleRecord(string _saleID, string _modelID, string _vehicleType,
string _customerID, string _salesmanID, string _saleDate, double _salePrice) {
    this->saleID = _saleID;
    this->modelID = _modelID;
    this->vehicleType = _vehicleType;
    this->customerID = _customerID;
    this->salesmanID = _salesmanID;
    this->saleDate = _saleDate;
    this->salePrice = _salePrice;
}

SaleRecord* SaleRecord::getSaleRecord(const string &__saleID) {
    string temp_saleID, temp_modelID, temp_vehicleType, temp_customerID, temp_salesmanID, temp_saleDate;
    double temp_salePrice;
    string temp;

    int num = number_of_models ("sales.txt");
    ifstream read ("sales.txt");
    if (!read) {
        throw "\"cars.txt\" File Can't be Opened.\n";
    } getline (read,temp, '\n');
    
    for (int i = 0; i < num; i++) {
        getline (read,temp_saleID, '|');
        getline (read,temp_modelID, '|');
        getline (read,temp_vehicleType, '|');
        getline (read,temp_customerID, '|');
        getline (read,temp_salesmanID, '|');
        getline (read,temp_saleDate, '|');
        read >> temp_salePrice;
        read.ignore();

        if (temp_saleID == __saleID) {
            SaleRecord * slr = new SaleRecord (temp_saleID, temp_modelID, temp_vehicleType,
            temp_customerID, temp_salesmanID, temp_saleDate, temp_salePrice);

            read.close();
            return slr;
        }
    }

    read.close();
    throw "No Such Sale Record Exist.\n";
    return nullptr;
}

void SaleRecord::dispRec () {
    cout << "〣---------------------------------------------------------------------"
    "-----------------------------------------------------------------------〣\n";
    cout << "〣 Sale_ID:" << this->saleID << " ║ Model_ID:" << this->modelID << " ║ Vehicle_Type:" << this->vehicleType 
    << " ║ Customer_ID:" << this->customerID << " ║ Salesman_ID:" << this->salesmanID << " ║ Sale_Date:" << this->saleDate << " ║ Sale_Price:"
    << this->salePrice; 
    cout << "\n〣---------------------------------------------------------------------"
    "-----------------------------------------------------------------------〣\n";
}

string SaleRecord::getSaleID() const {
    return this->saleID;
}

string SaleRecord::getModelID() const {
    return this->modelID;
}

string SaleRecord::getVehicleType() const {
    return this->vehicleType;
}

string SaleRecord::getCustomerID() const {
    return this->customerID;
}

string SaleRecord::getSalesmanID() const {
    return this->salesmanID;
}

string SaleRecord::getSaleDate() const {
    return this->saleDate;
}

double SaleRecord::getSalePrice() const {
    return this->salePrice;
}