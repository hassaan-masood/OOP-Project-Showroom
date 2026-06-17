#include "Inventory.h"

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

void Inventory::loadAll() {
    string temp_model_id, temp_brand, temp_model_name, 
    temp_color, temp_fuel_type, temp_transmission, temp, temp_bikeType;

    int temp_year, temp_quantity, temp_engine_cc,
    temp_mileage_kmph, temp_seatingCapacity, temp_numDoors;

    double temp_price, temp_scl;
    bool temp_is_electic;

    int num = number_of_models ("cars.txt");

    ifstream read ("cars.txt");
    if (!read) {
        throw "\"cars.txt\" File Can't be Opened.\n";
    } getline (read,temp,'\n');

    this->intro_lines.push_back (temp);

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

        Car nw_car (temp_model_id, temp_brand, temp_model_name, temp_year, temp_color,
        temp_price, temp_quantity, temp_is_electic, temp_engine_cc, temp_mileage_kmph,
        temp_fuel_type, temp_seatingCapacity, temp_transmission, temp_numDoors); 

        this->cars.push_back (nw_car);

    } read.close();

    num = number_of_models ("bikes.txt");

    read.open("bikes.txt");
    if (!read) {
        throw "\"bikes.txt\" File Can't be Opened.\n";
    } getline (read,temp,'\n');

    this->intro_lines.push_back (temp);

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
        getline (read,temp_bikeType,'\n');

        Bike nw_bike (temp_model_id, temp_brand, temp_model_name, temp_year, temp_color,
        temp_price, temp_quantity, temp_is_electic, temp_engine_cc, temp_mileage_kmph,
        temp_fuel_type, temp_bikeType); 

        this->bikes.push_back (nw_bike);

    } read.close();

    num = number_of_models ("scooties.txt");

    read.open ("scooties.txt");
    if (!read) {
        throw "\"scooties.txt\" File Can't be Opened.\n";
    } getline (read,temp,'\n');

    this->intro_lines.push_back (temp);

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

        Scooty nw_scooty (temp_model_id, temp_brand, temp_model_name, temp_year, temp_color,
        temp_price, temp_quantity, temp_is_electic, temp_engine_cc, temp_mileage_kmph,
        temp_fuel_type, temp_scl); 

        this->scooties.push_back (nw_scooty);

    } read.close();

}

void Inventory::Changeable_disp (string type) {
    if (type == "NULL") {
        cout << "                             〣---------------------------〣 CARS AVAILABLE 〣---------------------------〣\n\n";
        for (int i = 0; i < this->cars.size(); i++) {
            this->cars[i].displayInfo();
            cout << endl;
        }
        cout << "                             〣---------------------------〣 BIKES AVAILABLE 〣---------------------------〣\n\n";
        for (int i = 0; i < this->bikes.size(); i++) {
            this->bikes[i].displayInfo();
            cout << endl;
        }
        cout << "                             〣---------------------------〣 SCOOTIES AVAILABLE 〣---------------------------〣\n\n";
        for (int i = 0; i < this->scooties.size(); i++) {
            this->scooties[i].displayInfo();
            cout << endl;
        }
    } else if (type == "Car") {
        cout << "                              〣---------------------------〣 CARS AVAILABLE 〣---------------------------〣\n\n";
        for (int i = 0; i < this->cars.size(); i++) {
            this->cars[i].displayInfo();
            cout << endl;
        }
    } else if (type == "Bike") {
        cout << "                              〣---------------------------〣 BIKES AVAILABLE 〣---------------------------〣\n\n";
        for (int i = 0; i < this->bikes.size(); i++) {
            this->bikes[i].displayInfo();
            cout << endl;
        }
    } else if (type == "Scooty") {
        cout << "                              〣---------------------------〣 SCOOTIES AVAILABLE 〣---------------------------〣\n\n";
        for (int i = 0; i < this->scooties.size(); i++) {
            this->scooties[i].displayInfo();
            cout << endl;
        }
    } else {
        throw "Wrong Type Entered.\n";
    }
}

void Inventory::saveAll() const {
    if (this->cars.size() == 0 && this->bikes.size () == 0 && this->scooties.size() == 0) {
        throw "Can't Save Anything.\n";
        return;
    }

    ofstream write ("cars.txt");
    if (!write) {
        throw "\"cars.txt\" File Can't be Opened.\n";
    }
    write << this->intro_lines[0] << endl;
    for (int i = 0; i < this->cars.size(); i++) {
        write << this->cars[i].get_model_id() << "|" << this->cars[i].get_brand() << "|" << this->cars[i].get_model_name() 
        << "|" << this->cars[i].get_year() << "|" << this->cars[i].get_color() << "|" << this->cars[i].get_price() 
        << "|" << this->cars[i].get_quantity() << "|" << this->cars[i].get_is_electric() << "|" << this->cars[i].get_engine_cc()
        << "|" << this->cars[i].get_mileage() << "|" << this->cars[i].get_fuel_type() << "|" << this->cars[i].getSeatingCapacity()
        << "|" << this->cars[i].getTransmission() << "|" << this->cars[i].getNumDoors() << endl;
    } write.close();

    write.open ("bikes.txt");
    if (!write) {
        throw "\"bikes.txt\" File Can't be Opened.\n";
    }
    write << this->intro_lines[1] << endl;
    for (int i = 0; i < this->bikes.size(); i++) {
        write << this->bikes[i].get_model_id() << "|" << this->bikes[i].get_brand() << "|" << this->bikes[i].get_model_name() 
        << "|" << this->bikes[i].get_year() << "|" << this->bikes[i].get_color() << "|" << this->bikes[i].get_price() 
        << "|" << this->bikes[i].get_quantity() << "|" << this->bikes[i].get_is_electric() << "|" << this->bikes[i].get_engine_cc()
        << "|" << this->bikes[i].get_mileage() << "|" << this->bikes[i].get_fuel_type() << "|" << this->bikes[i].get_bikeType() << endl;

        if (i < this->bikes.size() - 1) {
            cout << endl;
        }

    } write.close();

    write.open ("scooties.txt");
    if (!write) {
        throw "\"scooties.txt\" File Can't be Opened.\n";
    }
    write << this->intro_lines[2] << endl;
    for (int i = 0; i < this->scooties.size(); i++) {
        write << this->scooties[i].get_model_id() << "|" << this->scooties[i].get_brand() << "|" << this->scooties[i].get_model_name() 
        << "|" << this->scooties[i].get_year() << "|" << this->scooties[i].get_color() << "|" << this->scooties[i].get_price() 
        << "|" << this->scooties[i].get_quantity() << "|" << this->scooties[i].get_is_electric() << "|" << this->scooties[i].get_engine_cc()
        << "|" << this->scooties[i].get_mileage() << "|" << this->scooties[i].get_fuel_type() << "|" << this->scooties[i].get_storageCapacityLiters() << endl;

        if (i < this->scooties.size() - 1) {
            cout << endl;
        }

    } write.close();
}

vector<Vehicle*> Inventory::getAllVehicles() {
    vector <Vehicle*> all_models;

    if (this->cars.size() == 0 && this->bikes.size () == 0 && this->scooties.size() == 0) {
        throw "No Vehicles Loaded.\n";
        return all_models;    
    }

    for (int i = 0; i < this->cars.size(); i++) {
        all_models.push_back (&this->cars[i]);
    } for (int i = 0; i < this->bikes.size(); i++) {
        all_models.push_back (&this->bikes[i]);
    } for (int i = 0; i < this->scooties.size(); i++) {
        all_models.push_back (&this->scooties[i]);
    } 

    return all_models;
}

Vehicle* Inventory::findByModelID(const string &modelID) {
    if (this->cars.size() == 0 && this->bikes.size () == 0 && this->scooties.size() == 0) {
        throw "No Vehicles Loaded.\n";
        return nullptr;
    }

    for (int i = 0; i < this->cars.size(); i++) {
        if (this->cars[i].get_model_id() == modelID) {
            return &(this->cars[i]);
        }
    } for (int i = 0; i < this->bikes.size(); i++) {
        if (this->bikes[i].get_model_id() == modelID) {
            return &(this->bikes[i]);
        }
    } for (int i = 0; i < this->scooties.size(); i++) {
        if (this->scooties[i].get_model_id() == modelID) {
            return &(this->scooties[i]);
        }
    } 

    throw "Model Not Found.\n";
    return nullptr;
}

void Inventory::addCar(const Car &c) {
    this->cars.push_back (c);
}

void Inventory::addBike(const Bike &b) {
    this->bikes.push_back(b);
}

void Inventory::addScooty(const Scooty &s) {
    this->scooties.push_back (s);
}

bool Inventory::removeModel(const string &modelID) {
    for (int i = 0; i < cars.size(); i++) {
        if (cars[i].get_model_id() == modelID) {
            cars.erase(cars.begin() + i);        // I google Searched That How to remove Target from Vector.
            return true;
        }
    }
    for (int i = 0; i < bikes.size(); i++) {
        if (bikes[i].get_model_id() == modelID) {
            bikes.erase(bikes.begin() + i);  
            return true;
        }
    }
    for (int i = 0; i < scooties.size(); i++) {
        if (scooties[i].get_model_id() == modelID) {
            scooties.erase(scooties.begin() + i);
            return true;
        }
    }

    return false;
}

void Inventory::updateQuantity(const string &modelID, int newQuantity) {
    if (newQuantity < 0) {
        throw "Can't Update to Negative Quantity.\n";
    }
    Vehicle* ptr = findByModelID (modelID);
    ptr->setQuantity(newQuantity);
}

vector<Car*> Inventory::getCars() {
    vector <Car*> ptr;
    for (int i = 0; i < this->cars.size(); i++) {
        ptr.push_back (&(this->cars[i]));
    }
    return ptr;
}

vector<Bike*> Inventory::getBikes() {
    vector <Bike*> ptr;
    for (int i = 0; i < this->bikes.size(); i++) {
        ptr.push_back (&(this->bikes[i]));
    }
    return ptr;
}

vector<Scooty*> Inventory::getScooties() {
    vector <Scooty*> ptr;
    for (int i = 0; i < this->scooties.size(); i++) {
        ptr.push_back (&(this->scooties[i]));
    }
    return ptr;
}