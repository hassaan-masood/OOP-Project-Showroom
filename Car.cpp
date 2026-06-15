#include "Car.h"

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

Car :: Car () : Vehicle () {
    this->seatingCapacity = 0;
    this->transmission = "NONE";
    this->numDoors = 0;
}

Car :: Car(string modelID, string brand, string modelName, int year, string color, double price, int quantity,
bool isElectric, int engineCC, double mileageKmpl, string fuelType, int _seatingCapacity, string _transmission,
int _numDoors) : Vehicle (modelID, brand, modelName, year, color, price, quantity, isElectric, engineCC, mileageKmpl, fuelType) {
    this->seatingCapacity = _seatingCapacity;
    this->transmission = _transmission;
    this->numDoors = _numDoors;
}

Car :: Car (const Car& other) : Vehicle(other) {
    this->seatingCapacity = other.seatingCapacity;
    this->transmission = other.transmission;
    this->numDoors = other.numDoors;
}

void Car :: displayInfo() const {
    cout << "〣------------------------------------------------------------------"
    "-----------------------------------------------------------------------〣\n";
    cout << "〣            Model_ID:" << this->model_id << " ║ Brand:" << this->brand << " ║ Model_Name:" << this->model_name 
    << " ║ Year:" << this->year << " ║ Colour:" << this->color << " ║ Price:" << this->price << " ║ Qunatity:"
    << this->quantity << endl << "〣 Brand:" << this->brand << " ║ Electric:" << this->is_electic  << " ║ Engine_CC:" << this->engine_cc
    << " ║ Mileage:" << this->mileage_kmph << " ║ Fuel_Type:" << this->fuel_type << " ║ Seating Cap:" << this->seatingCapacity << " ║ Transmisstion: " << this->transmission << " ║ Number of Doors:" << this->numDoors; 
    cout << "\n〣------------------------------------------------------------------"
    "-----------------------------------------------------------------------〣\n";
}

string Car :: getType() {
    return "Car";
}

int Car :: getSeatingCapacity() const {
    return this->seatingCapacity;
}

string Car :: getTransmission() const {
    return this->transmission;
}

int Car :: getNumDoors() const {
    return this->numDoors;
}

Car* Car :: getCarModel(const string &__modelID) {
    string temp_model_id, temp_brand, temp_model_name, 
    temp_color, temp_fuel_type, temp_transmission, temp;

    int temp_year, temp_quantity, temp_engine_cc,
    temp_mileage_kmph, temp_seatingCapacity, temp_numDoors;

    double temp_price;
    bool temp_is_electic;

    int num = number_of_models ("cars.txt");

    ifstream read ("cars.txt");
    if (!read) {
        throw "\"cars.txt\" File Can't be Opened.\n";
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
        read >> temp_seatingCapacity;
        read.ignore();
        getline (read,temp_transmission,'|'); 
        read >> temp_numDoors;
        read.ignore();

        if (temp_model_id == __modelID) {
            Car* temp_car  = new Car (temp_model_id, temp_brand, temp_model_name, temp_year, temp_color,
            temp_price, temp_quantity, temp_is_electic, temp_engine_cc, temp_mileage_kmph,
            temp_fuel_type, temp_seatingCapacity, temp_transmission, temp_numDoors);

            read.close();
            return temp_car;
        }
    }

    read.close();
    throw "Car Not Found.\n";
    return nullptr;
}