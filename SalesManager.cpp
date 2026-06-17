#include "SalesManager.h"

void SalesManager::loadAll() {
    string temp_saleID, temp_modelID, temp_vehicleType, temp_customerID, temp_salesmanID, temp_saleDate;
    string temp_plateNumber, temp_brand, temp_modelName;
    double temp_salePrice;
    string temp;

    int num = number_of_models ("sales.txt");
    ifstream read ("sales.txt");
    if (!read) {
        throw "\"sales.txt\" File Can't be Opened.\n";
    } getline (read,temp, '\n');

    this->intro_lines.push_back (temp);

    for (int i = 0; i < num; i++) {
        getline (read,temp_saleID, '|');
        getline (read,temp_modelID, '|');
        getline (read,temp_vehicleType, '|');
        getline (read,temp_customerID, '|');
        getline (read,temp_salesmanID, '|');
        getline (read,temp_saleDate, '|');
        read >> temp_salePrice;
        read.ignore();

        SaleRecord nw_Rec (temp_saleID, temp_modelID, temp_vehicleType,
        temp_customerID, temp_salesmanID, temp_saleDate, temp_salePrice);

        sales.push_back(nw_Rec);
    } read.close();

    num = number_of_models ("sold_Vehicles.txt");
    read.open ("sold_Vehicles.txt");

    if (!read) {
        throw "\"sold_Vehicles.txt\" File Can't be Opened.\n";
    } getline (read,temp, '\n');

    this->intro_lines.push_back (temp);

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

        SoldVehicle nw_veh (temp_plateNumber, temp_modelID, temp_vehicleType,
        temp_brand , temp_modelName,temp_customerID, temp_salesmanID, temp_saleDate, temp_salePrice);

        soldVehicles.push_back(nw_veh);
    }
}

void SalesManager::saveAll() const {
    ofstream write ("sales.txt");
    if (!write) {
        throw "\"sales.txt\" File Can't be Opened.\n";
    }

    write << this->intro_lines[0] << endl;
    for (int i = 0; i < this->sales.size(); i++) {
        write << this->sales[i].getSaleID() << "|" << this->sales[i].getModelID() << "|" 
        << this->sales[i].getVehicleType() << "|" << this->sales[i].getCustomerID() << "|" 
        << this->sales[i].getSalesmanID() << "|" << this->sales[i].getSaleDate() << "|"
        << this->sales[i].getSalePrice() << endl;
    } write.close();

    write.open ("sold_Vehicles.txt");
    if (!write) {
        throw "\"sold_Vehicles.txt\" File Can't be Opened.\n";
    }

    write << this->intro_lines[1] << endl;
    for (int i = 0; i < this->soldVehicles.size(); i++) {
        write << this->soldVehicles[i].getPlateNumber() << "|" << this->soldVehicles[i].getModelID() << "|" 
        << this->soldVehicles[i].getVehicleType() << "|" << this->soldVehicles[i].getBrand() << "|"  <<
        this->soldVehicles[i].getModelName() << "|" << this->soldVehicles[i].getCustomerID() << "|"  <<
        this->soldVehicles[i].getSalesmanID() << "|" << this->soldVehicles[i].getSaleDate() << "|" <<
        this->soldVehicles[i].getSalePrice() << endl;
    } write.close();
}

string SalesManager::generate_PlateNumber() const {
    string plate = "";

    for(int i = 0; i < 3; i++) {
        plate += 'A' + (rand() % 26); 
    } plate += "-";

    for(int i = 0; i < 4; i++) {
        plate += '0' + (rand() % 10); 
    }

    return plate;
}

void SalesManager::record_Sale(const SaleRecord &sr, const SoldVehicle &sv) {
    SaleRecord temp_saleRecord = sr;
    SoldVehicle temp_SoldVehicle = sv;

    this->sales.push_back (temp_saleRecord);
    this->soldVehicles.push_back (temp_SoldVehicle);
}

vector<SoldVehicle> SalesManager::get_Purchases_By_Customer(const string &customerID) const {
    vector <SoldVehicle> to_deliver;
    for (int i = 0; i < this->soldVehicles.size(); i++) {
        if (this->soldVehicles[i].getCustomerID() == customerID) {
            to_deliver.push_back (this->soldVehicles[i]);
        }
    }

    return to_deliver;
}

vector<SaleRecord> SalesManager::get_Sales_By_Salesman(const string &salesmanID) const {
    vector <SaleRecord> to_deliver;
    for (int i = 0; i < this->sales.size(); i++) {
        if (this->sales[i].getSalesmanID() == salesmanID) {
            to_deliver.push_back (this->sales[i]);
        }
    }

    return to_deliver;
}

int SalesManager::get_Total_Sales_Count() const {
    return this->sales.size();
}

double SalesManager::get_Total_Revenue() const { 
    double tempo = 0;
    for (int i = 0; i < this->sales.size(); i++) {
        tempo += this->sales[i].getSalePrice();
    }
    return tempo;
}