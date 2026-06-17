#include "SoldVehicle.h"

SoldVehicle::SoldVehicle () {
    this->plateNumber = "NULL";
    this->modelID = "NULL";
    this->vehicleType = "NULL";
    this->brand = "NULL";
    this->modelName = "NULL";
    this->customerID = "NULL";
    this->salesmanID = "NULL";
    this->saleDate = "NULL";
    this->salePrice = 0;
}

SoldVehicle::SoldVehicle(string _plateNumber, string _modelID, string _vehicleType,
string _brand, string _modelName, string _customerID,
string _salesmanID, string _saleDate, double _salePrice) {
    this->plateNumber = _plateNumber;
    this->modelID = _modelID;
    this->vehicleType = _vehicleType;
    this->brand = _brand;
    this->modelName = _modelName;
    this->customerID = _customerID;
    this->salesmanID = _salesmanID;
    this->saleDate = _saleDate;
    this->salePrice = _salePrice;
}

SoldVehicle* SoldVehicle::getSoldVehicle(const string &__numPlate) {
    string temp_plateNumber, temp_modelID, temp_vehicleType, temp_brand, temp_modelName, temp_customerID,
    temp_salesmanID, temp_saleDate;
    double temp_salePrice;
    string temp;

    int num = number_of_models ("sold_Vehicles.txt");
    ifstream read ("sold_Vehicles.txt");
    if (!read) {
        throw "\"sold_Vehicles.txt\" File Can't be Opened.\n";
    } getline (read,temp, '\n');
    
    for (int i = 0; i < num; i++) {
        getline (read,temp_plateNumber, '|');
        getline (read,temp_modelID, '|');
        getline (read,temp_vehicleType, '|');
        getline (read,temp_brand, '|');
        getline (read,temp_modelName, '|');
        getline (read,temp_customerID, '|');
        getline (read,temp_salesmanID, '|');
        getline (read,temp_saleDate, '|');
        read >> temp_salePrice;
        read.ignore();

        if (temp_plateNumber == __numPlate) {
            SoldVehicle * sld_veh = new SoldVehicle (temp_plateNumber, temp_modelID, temp_vehicleType,
            temp_brand ,temp_modelName ,temp_customerID, temp_salesmanID, temp_saleDate, temp_salePrice);

            read.close();
            return sld_veh;
        }
    }

    read.close();
    throw "No Such Sold Vehicle Exist.\n";
    return nullptr;
}

void SoldVehicle::dispSoldVec () {
    cout << "〣---------------------------------------------------------------------"
    "-----------------------------------------------------------------------〣\n";
    cout << "〣                   Plate_Number:" << this->plateNumber << " ║ Model_ID:" << this->modelID <<
    " ║ Vehicle_Type:" << this->vehicleType <<  " ║ Brand:" << this->brand << " ║ Model_Name:" << this->modelName 
    << endl << "〣                          Customer_ID:" << this->customerID << " ║ Salesman_ID:" <<
    this->salesmanID << " ║ Sale_Date:" << this->saleDate << " ║ Sale_Price:" << this->salePrice; 
    cout << "\n〣---------------------------------------------------------------------"
    "-----------------------------------------------------------------------〣\n";
}

string SoldVehicle::getPlateNumber() const {
    return this->plateNumber;
}

string SoldVehicle::getModelID() const {
    return this->modelID;
}

string SoldVehicle::getVehicleType() const {
    return this->vehicleType;
}

string SoldVehicle::getBrand() const {
    return this->brand;
}

string SoldVehicle::getModelName() const {
    return this->modelName;
}

string SoldVehicle::getCustomerID() const {
    return this->customerID;
}

string SoldVehicle::getSalesmanID() const {
    return this->salesmanID;
}

string SoldVehicle::getSaleDate() const {
    return this->saleDate;
}

double SoldVehicle::getSalePrice() const {
    return this->salePrice;
}