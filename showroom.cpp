#include <iostream>
#include <unistd.h>
#include <iomanip>
#include <fstream>
#include <vector>

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

class Vehicle {
protected:
    string model_id;
    string brand;
    string model_name;
    int year;
    string color;
    double price;
    int quantity;
    bool is_electic;
    int engine_cc;
    int mileage_kmph;
    string fuel_type;

public:
    // Constructors

    Vehicle () {
        model_id = "NULL";
        brand = "NULL";
        model_name = "NULL";
        year = 2000;
        color = "NULL";
        price = 0;
        quantity = 0;
        is_electic = 0;
        engine_cc = 0;
        mileage_kmph = 0;
        fuel_type = "NULL";
    }

    Vehicle (string _model_id,string _brand,string _model_name,int _year
    ,string _color,double _price,int _quantity,bool _is_electric,
    int _engine_cc, int _mileage_kmph,string _fuel_type) {

        this->model_id = _model_id;
        this->brand = _brand;
        this->model_name = _model_name;
        this->year = _year;
        this->color = _color;
        this->price = _price;
        this->quantity = _quantity;
        this->is_electic = _is_electric;
        this->engine_cc = _engine_cc;
        this->mileage_kmph = _mileage_kmph;
        this->fuel_type = _fuel_type;
    }

    Vehicle (const Vehicle& other) {
        this->model_id = other.model_id;
        this->brand = other.brand;
        this->model_name = other.model_name;
        this->year = other.year;
        this->color = other.color;
        this->price = other.price;
        this->quantity = other.quantity;
        this->is_electic = other.is_electic;
        this->engine_cc = other.engine_cc;
        this->mileage_kmph = other.mileage_kmph;
        this->fuel_type = other.fuel_type;
    }

    // Destructor

    virtual ~Vehicle() {}

    // Getters.

    string get_model_id () const {
        return this->model_id;
    }

    string get_brand () const {
        return this->brand;
    }

    string get_model_name () const {
        return this->model_name;
    }

    int get_year () const {
        return this->year;
    }

    string get_color () const {
        return this->color;
    }

    double get_price () const {
        return this->price;
    }

    int get_quantity () const {
        return this->quantity;
    }

    bool get_is_electric () const {
        return this->is_electic;
    }

    int get_engine_cc () const {
        return this->engine_cc;
    }

    int get_mileage () const {
        return this->mileage_kmph;
    } 

    string get_fuel_type () const {
        return this->fuel_type;
    }

    void setPrice(double newPrice) {
        this->price = newPrice;
    }

    void setQuantity(int q) {
        this->quantity = q;
    }

    void reduceQuantity(int amount) {
        if (this->quantity-amount < 0) {
            throw "Not Enough Stock.\n";
        } else {
            this->quantity -= amount;
        }
    }
    void increaseQuantity(int amount) {
        this->quantity += amount;
    }

    virtual void displayInfo() const = 0;

    virtual string getType () = 0;
};

class Car : public Vehicle {
    int seatingCapacity;
    string transmission;
    int numDoors;
 
public:
    Car () : Vehicle () {
        this->seatingCapacity = 0;
        this->transmission = "NONE";
        this->numDoors = 0;
    }

    Car(string modelID, string brand, string modelName, int year, string color, double price, int quantity,
    bool isElectric, int engineCC, double mileageKmpl, string fuelType, int _seatingCapacity, string _transmission,
    int _numDoors) : Vehicle (modelID, brand, modelName, year, color, price, quantity, isElectric, engineCC, mileageKmpl, fuelType) {
        this->seatingCapacity = _seatingCapacity;
        this->transmission = _transmission;
        this->numDoors = _numDoors;
    }

    Car (const Car& other) : Vehicle(other) {
        this->seatingCapacity = other.seatingCapacity;
        this->transmission = other.transmission;
        this->numDoors = other.numDoors;
    }
 
    void displayInfo() const override {
        cout << "〣------------------------------------------------------------------"
        "-----------------------------------------------------------------------〣\n";
        cout << "〣            Model_ID:" << this->model_id << " ║ Brand:" << this->brand << " ║ Model_Name:" << this->model_name 
        << " ║ Year:" << this->year << " ║ Colour:" << this->color << " ║ Price:" << this->price << " ║ Qunatity:"
        << this->quantity << endl << "〣 Brand:" << this->brand << " ║ Electric:" << this->is_electic  << " ║ Engine_CC:" << this->engine_cc
        << " ║ Mileage:" << this->mileage_kmph << " ║ Fuel_Type:" << this->fuel_type << " ║ Seating Cap:" << this->seatingCapacity << " ║ Transmisstion: " << this->transmission << " ║ Number of Doors:" << this->numDoors; 
        cout << "\n〣------------------------------------------------------------------"
        "-----------------------------------------------------------------------〣\n";
    }

    string getType() override {
        return "Car";
    }
 
    int getSeatingCapacity() const {
        return this->seatingCapacity;
    }

    string getTransmission() const {
        return this->transmission;
    }

    int getNumDoors() const {
        return this->numDoors;
    }
 
    static Car* getCarModel(const string &__modelID) {
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
};

class Bike : public Vehicle {
    string bikeType; 
 
public:
    Bike () : Vehicle () {
        this->bikeType = "NONE";
    }

    Bike(string modelID, string brand, string modelName, int year, string color, double price, int quantity,
    bool isElectric, int engineCC, double mileageKmpl, string fuelType, string _bike_type) : Vehicle 
    (modelID, brand, modelName, year, color, price, quantity, isElectric, engineCC, mileageKmpl, fuelType) {
        this->bikeType = _bike_type;
    }

    Bike(const Bike& other) : Vehicle(other) {
       this->bikeType = other.bikeType;
    }
 
    void displayInfo() const override {
        cout << "〣-------------------------------------------------------------"
        "-----------------------------------------------------------------------〣\n";
        cout << "〣                 Model_ID:" << this->model_id << " ║ Brand:" << this->brand << " ║ Model_Name:" << this->model_name 
        << " ║ Year:" << this->year << " ║ Colour:" << this->color << " ║ Price:" << this->price << endl << "〣             Qunatity:"
        << this->quantity << " ║ Brand:" << this->brand << " ║ Electric:" << this->is_electic  << " ║ Engine_CC:" << this->engine_cc
        << " ║ Mileage:" << this->mileage_kmph << " ║ Fuel_Type:" << this->fuel_type << " ║ Bike_Type:" << this->bikeType; 
        cout << "\n〣-------------------------------------------------------------"
        "-----------------------------------------------------------------------〣\n";
    }

    string getType() override {
        return "Bike";
    }
 
    string get_bikeType() const {
        return this->bikeType;
    }

    static Bike* getBikeModel(const string &__modelID) {
        string temp_model_id, temp_brand, temp_model_name, 
        temp_color, temp_fuel_type, temp_bikeType, temp;

        int temp_year, temp_quantity, temp_engine_cc,
        temp_mileage_kmph;

        double temp_price;
        bool temp_is_electic;

        int num = number_of_models ("bikes.txt");

        ifstream read ("bikes.txt");
        if (!read) {
            throw "\"bikes.txt\" File Can't be Opened.\n";
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
            getline (read,temp_bikeType,'\n');

            if (temp_model_id == __modelID) {
                Bike* temp_bike  = new Bike (temp_model_id, temp_brand, temp_model_name, temp_year, temp_color,
                temp_price, temp_quantity, temp_is_electic, temp_engine_cc, temp_mileage_kmph,
                temp_fuel_type, temp_bikeType);

                read.close();
                return temp_bike;
            }
        }

        read.close();
        throw "Bike Not Found.\n";
        return nullptr;
    }
};

class Scooty : public Vehicle {
    double storageCapacityLiters;
 
public:
    Scooty () : Vehicle () {
        this->storageCapacityLiters = 0.0;
    }

    Scooty(string modelID, string brand, string modelName, int year, string color, double price, int quantity,
    bool isElectric, int engineCC, double mileageKmpl, string fuelType, double _ssl) : Vehicle 
    (modelID, brand, modelName, year, color, price, quantity, isElectric, engineCC, mileageKmpl, fuelType) {
        this->storageCapacityLiters = _ssl;
    }

    Scooty(const Scooty& other) : Vehicle(other) {
       this->storageCapacityLiters = other.storageCapacityLiters;
    }
 
    void displayInfo() const override {
        cout << "〣-------------------------------------------------------------"
        "-----------------------------------------------------------------------〣\n";
        cout << "〣                 Model_ID:" << this->model_id << " ║ Brand:" << this->brand << " ║ Model_Name:" << this->model_name 
        << " ║ Year:" << this->year << " ║ Colour:" << this->color << " ║ Price:" << this->price << endl << "〣              Qunatity:"
        << this->quantity << " ║ Brand:" << this->brand << " ║ Electric:" << this->is_electic  << " ║ Engine_CC:" << this->engine_cc
        << " ║ Mileage:" << this->mileage_kmph << " ║ Fuel_Type:" << this->fuel_type << " ║ Storage_Capacity:" << this->storageCapacityLiters; 
        cout << "\n〣-------------------------------------------------------------"
        "-----------------------------------------------------------------------〣\n";
    }

    string getType() override {
        return "Scooty";
    }
 
    double get_storageCapacityLiters() const {
        return this->storageCapacityLiters;
    }

    static Scooty* getScootyModel(const string &__modelID) {
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
};

class Inventory {
private:
    vector<Car> cars;
    vector<Bike> bikes;
    vector<Scooty> scooties;
    vector <string> intro_lines;
 
public:
    void loadAll() {
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

    void Changeable_disp (string type = "NULL") {
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

    void saveAll() const {
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
 

    vector<Vehicle*> getAllVehicles() {
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

    Vehicle* findByModelID(const string &modelID) {
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
            if (this->bikes[i].get_model_id() == modelID) {
                return &(this->bikes[i]);
            }
        } 

        throw "Model Not Found.\n";
        return nullptr;
    }
 
    void addCar(const Car &c) {
        this->cars.push_back (c);
    }

    void addBike(const Bike &b) {
        this->bikes.push_back(b);
    }

    void addScooty(const Scooty &s) {
        this->scooties.push_back (s);
    }

    bool removeModel(const string &modelID) {
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
            if (bikes[i].get_model_id() == modelID) {
                scooties.erase(scooties.begin() + i);
                return true;
            }
        }

        return false;
    }
    void updateQuantity(const string &modelID, int newQuantity) {
        if (newQuantity < 0) {
            throw "Can't Update to Negative Quantity.\n";
        }
        Vehicle* ptr = findByModelID (modelID);
        ptr->setQuantity(newQuantity);
    }

    vector<Car*> getCars() {
        vector <Car*> ptr;
        for (int i = 0; i < this->cars.size(); i++) {
            ptr.push_back (&(this->cars[i]));
        }
        return ptr;
    }

    vector<Bike*> getBikes() {
        vector <Bike*> ptr;
        for (int i = 0; i < this->bikes.size(); i++) {
            ptr.push_back (&(this->bikes[i]));
        }
        return ptr;
    }

    vector<Scooty*> getScooties() {
        vector <Scooty*> ptr;
        for (int i = 0; i < this->scooties.size(); i++) {
            ptr.push_back (&(this->scooties[i]));
        }
        return ptr;
    }
    
};

class SaleRecord {
    string saleID;
    string modelID;
    string vehicleType;
    string customerID;
    string salesmanID;
    string saleDate;
    double salePrice;
 
public:
    SaleRecord () {
        this->saleID = "NULL";
        this->modelID = "NULL";
        this->vehicleType = "NULL";
        this->customerID = "NULL";
        this->salesmanID = "NULL";
        this->saleDate = "NULL";
        this->salePrice = 0;
    }

    SaleRecord(string _saleID, string _modelID, string _vehicleType,
    string _customerID, string _salesmanID, string _saleDate, double _salePrice) {
        this->saleID = _saleID;
        this->modelID = _modelID;
        this->vehicleType = _vehicleType;
        this->customerID = _customerID;
        this->salesmanID = _salesmanID;
        this->saleDate = _saleDate;
        this->salePrice = _salePrice;
    }
 
    static SaleRecord* getSaleRecord(const string &__saleID) {
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
    
    void dispRec () {
        cout << "〣---------------------------------------------------------------------"
        "-----------------------------------------------------------------------〣\n";
        cout << "〣 Sale_ID:" << this->saleID << " ║ Model_ID:" << this->modelID << " ║ Vehicle_Type:" << this->vehicleType 
        << " ║ Customer_ID:" << this->customerID << " ║ Salesman_ID:" << this->salesmanID << " ║ Sale_Date:" << this->saleDate << " ║ Sale_Price:"
        << this->salePrice; 
        cout << "\n〣---------------------------------------------------------------------"
        "-----------------------------------------------------------------------〣\n";
    }
    
    string getSaleID() const {
        return this->saleID;
    }

    string getModelID() const {
        return this->modelID;
    }

    string getVehicleType() const {
        return this->vehicleType;
    }

    string getCustomerID() const {
        return this->customerID;
    }

    string getSalesmanID() const {
        return this->salesmanID;
    }

    string getSaleDate() const {
        return this->saleDate;
    }
    double getSalePrice() const {
        return this->salePrice;
    }
};

int main () { 
    
}   