#include "Scooty.h"

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

Scooty :: Scooty () : Vehicle () {
    this->storageCapacityLiters = 0.0;
}

Scooty :: Scooty(string modelID, string brand, string modelName, int year, string color, double price, int quantity,
bool isElectric, int engineCC, double mileageKmpl, string fuelType, double _ssl) : Vehicle 
(modelID, brand, modelName, year, color, price, quantity, isElectric, engineCC, mileageKmpl, fuelType) {
    this->storageCapacityLiters = _ssl;
}

Scooty :: Scooty(const Scooty& other) : Vehicle(other) {
   this->storageCapacityLiters = other.storageCapacityLiters;
}

void Scooty :: displayInfo() const {
    cout << "〣-------------------------------------------------------------"
    "-----------------------------------------------------------------------〣\n";
    cout << "〣                 Model_ID:" << this->model_id << " ║ Brand:" << this->brand << " ║ Model_Name:" << this->model_name 
    << " ║ Year:" << this->year << " ║ Colour:" << this->color << " ║ Price:" << this->price << endl << "〣              Qunatity:"
    << this->quantity << " ║ Brand:" << this->brand << " ║ Electric:" << this->is_electic  << " ║ Engine_CC:" << this->engine_cc
    << " ║ Mileage:" << this->mileage_kmph << " ║ Fuel_Type:" << this->fuel_type << " ║ Storage_Capacity:" << this->storageCapacityLiters; 
    cout << "\n〣-------------------------------------------------------------"
    "-----------------------------------------------------------------------〣\n";
}

string Scooty :: getType() {
    return "Scooty";
}

double Scooty :: get_storageCapacityLiters() const {
    return this->storageCapacityLiters;
}

Scooty* Scooty :: getScootyModel(const string &__modelID) {
    string temp_model_id, temp_brand, temp_model_name, 
    temp_color, temp_fuel_type, temp;

    int temp_year, temp_quantity, temp_engine_cc,
    temp_mileage_kmph;

    double temp_price, temp_scl;
    bool temp_is_electic;

    int num = number_of_models ("scooties.txt");

    ifstream read ("scooties.txt");
    if (!read) {
        throw "\"scooties.txt\" File Can't be Opened.\n";
    } getline (read,temp,'\n');

    for (int i = 0; i < num; i++) {
        getline (read,temp_model_id,'|');
        getline (read,temp_brand,'|');
        getline (read,temp_model_name,'|');
        read >> temp_year;
        read.ignore();
        getline (read,temp_color,'|');
        read >> temp_price;
        read.ignore();
        read >> temp_quantity;
        read.ignore();
        read >> temp_is_electic;
        read.ignore();
        read >> temp_engine_cc;
        read.ignore();
        read >> temp_mileage_kmph;
        read.ignore();
        getline (read,temp_fuel_type,'|');
        read >> temp_scl;
        read.ignore();

        if (temp_model_id == __modelID) {
            Scooty* temp_scooty  = new Scooty (temp_model_id, temp_brand, temp_model_name, temp_year, temp_color,
            temp_price, temp_quantity, temp_is_electic, temp_engine_cc, temp_mileage_kmph,
            temp_fuel_type, temp_scl);

            read.close();
            return temp_scooty;
        }
    }

    read.close();
    throw "Scooty Not Found.\n";
    return nullptr;
}