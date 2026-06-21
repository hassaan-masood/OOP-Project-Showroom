#include <iostream>
#include <unistd.h>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "raylib.h"
#include <string>
#include <vector>
#include <functional>

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

const int SW = 3000, SH = 1800;
const Color BG      = { 15,  15,  20, 255 };
const Color PANEL   = { 28,  28,  38, 255 };
const Color ACCENT  = { 82, 130, 255, 255 };
const Color ACCENT2 = {255, 100,  80, 255 };
const Color TXT     = {220, 220, 230, 255 };
const Color TXTSUB  = {140, 140, 160, 255 };
const Color BTN     = { 45,  45,  62, 255 };
const Color BTNHOV  = { 60,  60,  85, 255 };
const Color BTNACT  = { 82, 130, 255, 255 };
const Color FIELD   = { 35,  35,  50, 255 };
const Color FIELDFOC= { 45,  45,  68, 255 };
const Color ERR     = {220,  60,  60, 255 };
const Color SUCCESS = { 60, 200, 120, 255 };

Font gFont;

struct InputBox {
    Rectangle rect;
    std::string label;
    std::string value;
    bool password = false;
    bool focused  = false;
    int  maxLen   = 64;

    void draw() const {
        Color bg = focused ? FIELDFOC : FIELD;
        DrawRectangleRec(rect, bg);
        DrawRectangleLinesEx(rect, focused ? 2.0f : 1.0f,
                             focused ? ACCENT : TXTSUB);
        DrawTextEx(gFont, label.c_str(),
        {rect.x, rect.y - 32}, 26, 1, TXTSUB);

        std::string disp = password
            ? std::string(value.size(), '*') : value;

        DrawTextEx(gFont, disp.c_str(),
        {rect.x + 12, rect.y + 14}, 30, 1, TXT);

        if (focused && (int)(GetTime() * 2) % 2 == 0) {
            float cx = rect.x + 12 +
            MeasureTextEx(gFont, disp.c_str(), 30, 1).x;
            DrawLineEx({cx, rect.y + 8}, {cx, rect.y + rect.height - 8},
            2, ACCENT);
        }
    }

    bool handleKey() {
        if (!focused) return false;
        int ch = GetCharPressed();
        bool changed = false;
        while (ch > 0) {
            if (ch >= 32 && (int)value.size() < maxLen) {
                value += (char)ch;
                changed = true;
            }
            ch = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !value.empty()) {
            value.pop_back();
            changed = true;
        }
        return changed;
    }

    bool hitTest(Vector2 mp) const {
        return CheckCollisionPointRec(mp, rect);
    }
};

struct Button {
    Rectangle rect;
    std::string label;
    Color col     = BTN;
    Color colHov  = BTNHOV;
    Color colAct  = BTNACT;
    Color textCol = TXT;
    float fontSize = 36;

    bool draw() const {
        Vector2 mp = GetMousePosition();
        bool hov  = CheckCollisionPointRec(mp, rect);
        bool down = hov && IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        bool clk  = hov && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

        Color bg = down ? colAct : (hov ? colHov : col);
        DrawRectangleRec(rect, bg);
        DrawRectangleLinesEx(rect, hov ? 2.0f : 1.0f,
        hov ? ACCENT : TXTSUB);

        Vector2 ts = MeasureTextEx(gFont, label.c_str(), fontSize, 1);
        DrawTextEx(gFont, label.c_str(),
            { rect.x + (rect.width  - ts.x) / 2,
            rect.y + (rect.height - ts.y) / 2 },
            fontSize, 1, textCol);
        return clk;
    }
};

static string gNotifMsg;
static double      gNotifTime = -99;
static bool        gNotifErr  = false;

void showNotif(const std::string& msg, bool isErr = false) {
    gNotifMsg  = msg;
    gNotifTime = GetTime();
    gNotifErr  = isErr;
}

void drawNotif() {
    if (GetTime() - gNotifTime > 2.5) return;
    Color c = gNotifErr ? ERR : SUCCESS;
    DrawRectangle(0, SH - 64, SW, 64, { c.r, c.g, c.b, 200 });
    Vector2 ts = MeasureTextEx(gFont, gNotifMsg.c_str(), 24, 1);
    DrawTextEx(gFont, gNotifMsg.c_str(),
    { (SW - ts.x) / 2, SH - 46 }, 24, 1, WHITE);
}

void drawInfoList(const std::vector<std::string>& lines,
    Rectangle area, int& scrollY) {
    BeginScissorMode((int)area.x,(int)area.y,
    (int)area.width,(int)area.height);
    float y = area.y - scrollY;
    for (auto& l : lines) {
        DrawTextEx(gFont, l.c_str(), {area.x + 8, y}, 26, 1, TXT);
        y += 36;
    }
    EndScissorMode();
    float wh = GetMouseWheelMove();
    if (wh != 0 && CheckCollisionPointRec(GetMousePosition(), area))
    scrollY = std::max(0, scrollY - (int)(wh * 30));
}

bool drawFormModal(const std::string& title,
    std::vector<InputBox>& fields,
    const std::string& confirmLabel = "Confirm") {
    DrawRectangle(0, 0, SW, SH, {0,0,0,160});

    float mw = 780, mh = 140.0f + fields.size() * 100.0f + 80.0f;
    float mx = (SW - mw) / 2, my = (SH - mh) / 2;
    DrawRectangle((int)mx,(int)my,(int)mw,(int)mh, PANEL);
    DrawRectangleLinesEx({mx,my,mw,mh}, 2, ACCENT);

    DrawTextEx(gFont, title.c_str(), {mx+20, my+18}, 30, 1, ACCENT);

    for (int i = 0; i < (int)fields.size(); i++) {
        fields[i].rect = { mx+20, my + 80.0f + i*100.0f, mw-40, 64 };
    }

    Vector2 mp = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (auto& f : fields)
            f.focused = f.hitTest(mp);
    }
    for (auto& f : fields) { f.draw(); f.handleKey(); }

    Button bConfirm{{ mx+20,      my+mh-76, 280, 62 }, confirmLabel,
                    BTN, BTNHOV, {60,180,100,255}};
    Button bCancel {{ mx+mw-300,  my+mh-76, 280, 62 }, "Cancel",
    BTN, BTNHOV, ACCENT2};

    if (bConfirm.draw()) return true;
    if (bCancel.draw())  return false;  
    return false;
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
            if (this->scooties[i].get_model_id() == modelID) {
                return &(this->scooties[i]);
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
            if (scooties[i].get_model_id() == modelID) {
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
            throw "\"sales.txt\" File Can't be Opened.\n";
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

class SoldVehicle {
    string plateNumber;
    string modelID;
    string vehicleType;
    string brand;
    string modelName;
    string customerID;
    string salesmanID;
    string saleDate;
    double salePrice;
 
public:

    SoldVehicle () {
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

    SoldVehicle(string _plateNumber, string _modelID, string _vehicleType,
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
 
    static SoldVehicle* getSoldVehicle(const string &__numPlate) {
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

    void dispSoldVec () {
        cout << "〣---------------------------------------------------------------------"
        "-----------------------------------------------------------------------〣\n";
        cout << "〣                   Plate_Number:" << this->plateNumber << " ║ Model_ID:" << this->modelID <<
        " ║ Vehicle_Type:" << this->vehicleType <<  " ║ Brand:" << this->brand << " ║ Model_Name:" << this->modelName 
        << endl << "〣                          Customer_ID:" << this->customerID << " ║ Salesman_ID:" <<
        this->salesmanID << " ║ Sale_Date:" << this->saleDate << " ║ Sale_Price:" << this->salePrice; 
        cout << "\n〣---------------------------------------------------------------------"
        "-----------------------------------------------------------------------〣\n";
    }
    
    string getPlateNumber() const {
        return this->plateNumber;
    }

    string getModelID() const {
        return this->modelID;
    }

    string getVehicleType() const {
        return this->vehicleType;
    }

    string getBrand() const {
        return this->brand;
    }

    string getModelName() const {
        return this->modelName;
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

class SalesManager {
    vector<SaleRecord> sales;
    vector<SoldVehicle> soldVehicles;
    vector <string> intro_lines;
 
public:
    void loadAll() {
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

    void saveAll() const {
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
 
    string generate_PlateNumber() const {
        string plate = "";

        for(int i = 0; i < 3; i++) {
            plate += 'A' + (rand() % 26); // Taking mod with 26 and adding in A 
        } plate += "-";

        for(int i = 0; i < 4; i++) {
            plate += '0' + (rand() % 10); // 0 + mod with 10 to generate random number
        }

        return plate;
    }
 
    void record_Sale(const SaleRecord &sr, const SoldVehicle &sv) {
        SaleRecord temp_saleRecord = sr;
        SoldVehicle temp_SoldVehicle = sv;

        this->sales.push_back (temp_saleRecord);
        this->soldVehicles.push_back (temp_SoldVehicle);
    }
 
    vector<SoldVehicle> get_Purchases_By_Customer(const string &customerID) const {
        vector <SoldVehicle> to_deliver;
        for (int i = 0; i < this->soldVehicles.size(); i++) {
            if (this->soldVehicles[i].getCustomerID() == customerID) {
                to_deliver.push_back (this->soldVehicles[i]);
            }
        }

        return to_deliver;
    }

    vector<SaleRecord> get_Sales_By_Salesman(const string &salesmanID) const {
        vector <SaleRecord> to_deliver;
        for (int i = 0; i < this->sales.size(); i++) {
            if (this->sales[i].getSalesmanID() == salesmanID) {
                to_deliver.push_back (this->sales[i]);
            }
        }

        return to_deliver;
    }
 
    int get_Total_Sales_Count() const {
        return this->sales.size();
    }

    double get_Total_Revenue() const { // To find total of all sales
        double tempo = 0;
        for (int i = 0; i < this->sales.size(); i++) {
            tempo += this->sales[i].getSalePrice();
        }
        return tempo;
    }
};

class RepairRecord {
    string repair_Id;
    string Customer_Id;
    string Plate_Number;
    string vehicle_type;
    string Part_Changed;
    double cost___chunna; // LMAFO
    string RepairManID;
    string status;
    string requestDate;
 
public:
    RepairRecord() {
        this->repair_Id = "NULL";
        this->Customer_Id = "NULL";
        this->Plate_Number = "NULL";
        this->vehicle_type = "NULL";
        this->Part_Changed = "NULL";
        this->cost___chunna = 0;
        this->RepairManID = "NULL";
        this->status = "NULL";
        this->requestDate = "NULL";
    }

    RepairRecord(string _repair_Id, string _Customer_Id, string _Plate_Number,
    string _vehicle_type, string _Part_Changed, double _cost___chunna,
    string _repairManID, string _status, string _requestDate) {
        this->repair_Id = _repair_Id;
        this->Customer_Id = _Customer_Id;
        this->Plate_Number = _Plate_Number;
        this->vehicle_type = _vehicle_type;
        this->Part_Changed = _Part_Changed;
        this->cost___chunna = _cost___chunna;
        this->RepairManID = _repairManID;
        this->status = _status;
        this->requestDate = _requestDate;
    }

    RepairRecord (const RepairRecord& other) {
        this->repair_Id = other.repair_Id;
        this->Customer_Id = other.Customer_Id;
        this->Plate_Number = other.Plate_Number;
        this->vehicle_type = other.vehicle_type;
        this->Part_Changed = other.Part_Changed;
        this->cost___chunna = other.cost___chunna;
        this->RepairManID = other.RepairManID;
        this->status = other.status;
        this->requestDate = other.requestDate;
    }

    static RepairRecord* getRepairRecord(const string &__repID) {
        string temp_repair_Id, temp_Customer_Id, temp_Plate_Number, temp_vehicle_type;
        string temp_Part_Changed, temp_RepairManID, temp_status, temp_requestDate;
        double temp_cost___chunna;
        string temp;
    
        int num = number_of_models ("Repair_history.txt");
        ifstream read ("Repair_history.txt");
        if (!read) {
            throw "\"Repair_history.txt\" File Can't be Opened.\n";
        } getline (read,temp, '\n');
        
        for (int i = 0; i < num; i++) {
            getline (read,temp_repair_Id, '|');
            getline (read,temp_Customer_Id, '|');
            getline (read,temp_Plate_Number, '|');
            getline (read,temp_vehicle_type, '|');
            getline (read,temp_Part_Changed, '|');
            read >> temp_cost___chunna;
            read.ignore();
            getline (read,temp_RepairManID, '|');
            getline (read,temp_status, '|');
            getline (read,temp_requestDate, '|');

            if (temp_repair_Id == __repID) {
                RepairRecord * rep_rec = new RepairRecord (temp_repair_Id, temp_Customer_Id, temp_Plate_Number,
                temp_vehicle_type, temp_Part_Changed, temp_cost___chunna, temp_RepairManID, temp_status, temp_requestDate);

                read.close();
                return rep_rec;
            }
        }

        read.close();
        throw "No Such Repair Record Exist.\n";
        return nullptr;
    }

    string get_RepairID() const {
        return this->repair_Id;
    }

    string get_CustomerID() const {
        return this->Customer_Id;
    }

    string get_PlateNumber() const {
        return this->Plate_Number;
    }

    string get_VehicleType() const {
        return this->vehicle_type;
    }

    string get_PartChanged() const {
        return this->Part_Changed;
    }

    double get_Cost() const {
        return this->cost___chunna;
    }

    string get_RepairManID() const {
        return this->RepairManID;
    }

    string get_Status() const {
        return this->status;
    }

    string get_RequestDate() const {
        return this->requestDate;
    }
 
    void set_Status(const string &nw_sts) {
        this->status = nw_sts;
    }

    void set_RepairManID(const string &id) {
        this->RepairManID = id;
    }

    void Disp_RepairRecord () {
        cout << "〣---------------------------------------------------------------------"
        "-----------------------------------------------------------------------〣\n";
        cout << "〣               Repair_ID:" << this->repair_Id << " ║ Customer_ID:" << this->Customer_Id <<
        " ║ Plate_Number:" << this->Plate_Number  << " ║ Vehicle_Type:" << this->vehicle_type <<
        " ║ Part:" << this->Part_Changed << endl << "〣                              Cost:" <<
        this->cost___chunna << " ║ Repair_ManID:" << this->RepairManID << " ║ Status:" << this->status <<
        " ║ Date:" << this->requestDate; 
        cout << "〣---------------------------------------------------------------------"
        "-----------------------------------------------------------------------〣\n";
    }


};

class Part {
    string name_of_part;
    double cost_of_part;

public:
    Part() {
        name_of_part = "NULL";
        cost_of_part = 0;
    }

    Part(string _name, double _cost) {
        this->name_of_part = _name;
        this->cost_of_part = _cost;
    }

    string get_Part () const {
        return this->name_of_part;
    }

    double get_Cost () const {
        return this->cost_of_part;
    }

    void change_Part_Name (string nw_name) {
        this->name_of_part = nw_name;
    }

    void change_Cost (double nw_cost) {
        this->cost_of_part = nw_cost;
    }
};

class RepairManager {
    vector<RepairRecord> repairs;
    vector<Part> partPriceList;
    vector <string> intro_lines;

public:
    void loadAll() {
        string temp_repair_Id, temp_Customer_Id, temp_Plate_Number, temp_vehicle_type, temp_Part_Changed;
        string temp_RepairManID, temp_status, temp_requestDate, temp_name_of_part;
        double temp_cost___chunna, temp_cost_of_part;
        string temp;

        int num = number_of_models("Repair_history.txt");
        ifstream read ("Repair_history.txt");
        if (!read) {
            throw "\"Repair_history.txt\" File Can't be Opened.\n";
        }   getline (read, temp, '\n');

        this->intro_lines.push_back (temp);

        for (int i = 0; i < num; i++) {
            getline (read, temp_repair_Id, '|');
            getline (read, temp_Customer_Id, '|');
            getline (read, temp_Plate_Number, '|');
            getline (read, temp_vehicle_type, '|');
            getline (read, temp_Part_Changed, '|');
            read >> temp_cost___chunna;
            read.ignore();
            getline (read, temp_RepairManID, '|');
            getline (read, temp_status, '|');
            getline (read, temp_requestDate, '\n');

            RepairRecord temp_rec (temp_repair_Id, temp_Customer_Id, temp_Plate_Number, temp_vehicle_type,
            temp_Part_Changed, temp_cost___chunna, temp_RepairManID, temp_status, temp_requestDate);

            this->repairs.push_back (temp_rec);
        } read.close();

        num = number_of_models("Price_list_of_Parts.txt");
        read.open ("Price_list_of_Parts.txt");
        if (!read) {
            throw "\"Price_list_of_Parts.txt\" File Can't be Opened.\n";
        }   getline (read, temp, '\n');

        this->intro_lines.push_back (temp);

        for (int i = 0; i < num; i++) {
            getline (read, temp_name_of_part, '|');
            read >> temp_cost_of_part;
            read.ignore();

            Part nw_part (temp_name_of_part, temp_cost_of_part);

            this->partPriceList.push_back (nw_part);
        } read.close();

    }

    void saveAll() const {
        ofstream write ("Repair_history.txt");
        if (!write) {
            throw "\"Repair_history.txt\" File Can't be Opened.\n";
        }

        write << this->intro_lines[0] << endl;
        for (int i = 0; i < this->repairs.size(); i++) {
            write << this->repairs[i].get_RepairID() << "|" << this->repairs[i].get_CustomerID() << "|" 
            << this->repairs[i].get_PlateNumber() << "|" << this->repairs[i].get_VehicleType() << "|"
            << this->repairs[i].get_PartChanged() << "|" << this->repairs[i].get_Cost() << "|" 
            << this->repairs[i].get_RepairManID() << "|" << this->repairs[i].get_Status() << "|"
            << this->repairs[i].get_RequestDate() << endl;
        } write.close();

        write.open ("Price_list_of_Parts.txt");
        if (!write) {
            throw "\"Price_list_of_Parts.txt\" File Can't be Opened.\n";
        }

        write << this->intro_lines[1] << endl;
        for (int i = 0; i < this->partPriceList.size(); i++) {
            write << this->partPriceList[i].get_Part() << "|" << this->partPriceList[i].get_Cost() << endl;
        } write.close();
    }

    double getPartCost(const string &part_name) const {
        for (int i = 0; i < this->partPriceList.size(); i++) {
            if (this->partPriceList[i].get_Part() == part_name) {
                return this->partPriceList[i].get_Cost();
            }
        }

        throw "No Such Part Exists.\n";
        return 0;
    }

    string requestRepair(const string &customerID, const string &plateNumber,
    const string &vehicleType, const string &partChanged,const string &datee) {
        double temp_cost = getPartCost (partChanged);
        string nw_id, prev_id;
        if (this->repairs.size() == 0) {
            nw_id = "RP001";
        } else {
            prev_id = this->repairs[this->repairs.size() - 1].get_RepairID();
            char ch = prev_id.back() + 1;
            nw_id = "RP00" + string (1,ch);
        }

        RepairRecord nw_rec (nw_id, customerID, plateNumber, vehicleType, partChanged, temp_cost, "NULL", "Pending", datee);
        this->repairs.push_back(nw_rec);
        return nw_id;
    }

    vector<RepairRecord> getHistoryByCustomer(const string &customer_ID) const {
        vector <RepairRecord> hist_to_deliver;
        for (int i = 0; i < this->repairs.size(); i++) {
            if (this->repairs[i].get_CustomerID() == customer_ID) {
                hist_to_deliver.push_back (this->repairs[i]);
            }
        }

        return hist_to_deliver;
    }

    vector<RepairRecord> getJobsByRepairMan(const string &Repair_ManID) const {
        vector <RepairRecord> hist_to_deliver;
        for (int i = 0; i < this->repairs.size(); i++) {
            if (this->repairs[i].get_RepairManID() == Repair_ManID) {
                hist_to_deliver.push_back (this->repairs[i]);
            }
        }

        return hist_to_deliver;
    }

    vector<RepairRecord> getPendingJobs() const {
        vector <RepairRecord> temp_pending_stuff;
        for (int i = 0; i < this->repairs.size(); i++) {
            if (this->repairs[i].get_Status() == "Pending") {
                temp_pending_stuff.push_back (this->repairs[i]);
            }
        }

        return temp_pending_stuff;
    }

    bool assignRepairMan(const string &__repair_ID, const string &repairManID) {
        for (int i = 0; i < this->repairs.size(); i++) {
            if (this->repairs[i].get_RepairID() == __repair_ID) {
                if (this->repairs[i].get_Status() == "Completed") {
                    throw "This Repair is Already Done.\n";
                    return 0;
                } else {
                    this->repairs[i].set_RepairManID (repairManID);
                    return 1;
                }
            }
        }

        throw "No Such Repair ID.\n";
        return 0;
    }

    bool updateStatus(const string &repair_iD, const string &newSts) {
        for (int i = 0; i < this->repairs.size(); i++) {
            if (this->repairs[i].get_RepairID() == repair_iD) {
                this->repairs[i].set_Status (newSts);
                return 1;
            }
        }

        throw "Can't Update Status.\n";
        return 0;
    }
};

class Person {
protected:
    string id;
    string name;
    string email;
    string phone;
    string username;
    string password;

public:
    Person () {
        this->id = "NULL";
        this->name = "NULL";
        this->email = "NULL";
        this->phone = "NULL";
        this->username = "NULL";
        this->password = "NULL";
    }

    Person(string _id, string _name, string _email, string _phone,
    string _username, string _password) {
        this->id = _id;
        this->name = _name;
        this->email = _email;
        this->phone = _phone;
        this->username = _username;
        this->password = _password;
    }

    Person (const Person &other) {
        this->id = other.id;
        this->name = other.name;
        this->email = other.email;
        this->phone = other.phone;
        this->username = other.username;
        this->password = other.password;
    }
 
    virtual ~Person() {}

    string getID() const {
        return this->id;
    }

    string getName() const {
        return this->name;
    }

    string getEmail() const {
        return this->email;
    }

    string getPhone() const {
        return this->phone;
    }

    string getUsername() const {
        return this->username;
    }
 
    bool checkPassword(const string &inp_pass) const {
        if (this->password == inp_pass) {
            return true;
        } return false;
    }
 
    virtual void showMenu() = 0;
    virtual string getRole() const = 0;
    virtual void disp_Info () const = 0;
};

class Employee : public Person {
protected:
    string cnic;
    string hireDate;
    double salary;
 
public:
    Employee () : Person () {
        this->cnic = "NULL";
        this->hireDate = "NULL";
        this->salary = 0;
    }

    Employee(string _id, string _name, string _email, string _phone,
    string _username, string _password, string _cnic,
    string _hireDate, double _salary) : Person (_id,_name,_email,_phone,_username,_password) {
        this->cnic = _cnic;
        this->hireDate = _hireDate;
        this->salary = _salary;
    }

    Employee (const Employee &other) : Person (other) {
        this->cnic = other.cnic;
        this->hireDate = other.hireDate;
        this->salary = other.salary;
    }

    string getCNIC() const {
        return this->cnic;
    }

    string getHireDate() const {
        return this->hireDate;
    }

    double getSalary() const {
        return this->salary;
    }

    void setSalary(double nw_sal) {
        if (nw_sal > 0) {
            this->salary = nw_sal;
        } else {
            throw "Can't Update Salary.\n";
        }
    }    
};

class Customer : public Person {
    string cnic;
    string address;
    string registration_date;

public:
    Customer () : Person () {
        this->cnic = "NULL";
        this->address = "NULL";
        this->registration_date = "NULL";
    }

    Customer(string _id, string _name, string _email, string _phone,
    string _username, string _password, string _cnic,
    string _address, string _registration_date) : Person (_id,_name,_email,_phone,_username,_password) {
        this->cnic = _cnic;
        this->address = _address;
        this->registration_date = _registration_date;
    } 

    Customer (const Customer &other) : Person (other) {
        this->cnic = other.cnic;
        this->address = other.address;
        this->registration_date = other.registration_date;
    }

    void showMenu() override {
        cout << "                            |〣--------------------------|〣 Customer Service Menu 〣|--------------------------〣|\n\n";
        cout << "1. Browse Stock Cars.\n";
        cout << "2. Browse Stock Bikes.\n";
        cout << "3. Browse Stock Scooties.\n";
        cout << "4. Browse All Stock Vehicles.\n";
        cout << "5. View My Purchase History.\n";
        cout << "6. View My Repair History.\n";
        cout << "7. Request Vehicle Repair.\n";
        cout << "8. Exit.\n";
    }

    string getRole() const override {
        return "Customer";
    }

    string getPasscode () const {
        return this->password;
    }

    string get_Date () const {
        return this->registration_date;
    }

    void disp_Info () const override {
        cout << "                            |〣--------------------------|〣 Customer Service Menu 〣|--------------------------〣|\n\n";
        cout << "〣------------------------------------------------------------------"
        "-----------------------------------------------------------------------〣\n";
        cout << "〣           Customer_ID: " << this->id << " ║ Name: " << this->name << " ║ CNIC: " << this->cnic
        << " ║ Registration_Date: " << this->registration_date << " ║ Phone: " << this->phone;
        cout << "\n〣------------------------------------------------------------------"
        "-----------------------------------------------------------------------〣\n";
    }

    string getCNIC() const {
        return this->cnic;
    }
    string getAddress() const {
        return this->address;
    }
 
    void browse_cars(Inventory &inv) const {
        cout << "                            |〣--------------------------|〣 Customer Service Menu 〣|--------------------------〣|\n\n";
        
        inv.Changeable_disp("Car");
    }

    void browse_bikes(Inventory &inv) const {
        cout << "                            |〣--------------------------|〣 Customer Service Menu 〣|--------------------------〣|\n\n";
        
        inv.Changeable_disp("Bike");
    }

    void browse_scooties(Inventory &inv) const {
        cout << "                            |〣--------------------------|〣 Customer Service Menu 〣|--------------------------〣|\n\n";
        
        inv.Changeable_disp("Scooty");
    }

    void browse_vehicles(Inventory &inv) const {
        cout << "                            |〣--------------------------|〣 Customer Service Menu 〣|--------------------------〣|\n\n";
        
        inv.Changeable_disp();
    }

    void viewMyPurchases(SalesManager &sm) const {
        cout << "                            |〣--------------------------|〣 Customer Service Menu 〣|--------------------------〣|\n\n";
     
        vector <SoldVehicle> vect = sm.get_Purchases_By_Customer (this->id);
        if (vect.size() == 0) {
            cout << "No Purchases Yet.\n";
        } else {
            for (int i = 0; i < vect.size(); i++) {
                vect[i].dispSoldVec();
            }
        }
    }

    void viewMyRepairHistory(RepairManager &rm) const {
        cout << "                            |〣--------------------------|〣 Customer Service Menu 〣|--------------------------〣|\n\n";
     
        vector <RepairRecord> vect = rm.getHistoryByCustomer (this->id);
        if (vect.size() == 0) {
            cout << "No Purchases Yet.\n";
        } else {
            for (int i = 0; i < vect.size(); i++) {
                vect[i].Disp_RepairRecord();
            }
        }
    }

    void requestVehicleRepair(RepairManager &rm, const string &plateNumber,
    const string &vehicleType, const string &partChanged) {

        // This Code Block in taken From Online Resourses To Get Current Time.
        time_t now = time(nullptr);
        tm* local_time = localtime(&now);
        stringstream ss;
        ss << put_time(local_time, "%d-%m-%Y");
        string date_str = ss.str();

        rm.requestRepair(this->id, plateNumber, vehicleType, partChanged,date_str);
    }
 
    static Customer* getCustomer_ById (string __id) {
        string temp_id, temp_name, temp_email, temp_phone, temp_username, temp_password;
        string temp_cnic, temp_address, temp_registration_date;
        string temp;

        int num = number_of_models ("customer.txt");

        ifstream read ("customer.txt");
        if (!read) {
            throw "\"customer.txt\" File Can't be Opened.\n";
        } getline (read, temp, '\n');

        for (int i = 0; i < num; i++) {
            getline (read, temp_id, '|');
            getline (read, temp_name, '|');
            getline (read, temp_email, '|');
            getline (read, temp_phone, '|');
            getline (read, temp_cnic, '|');
            getline (read, temp_address, '|');
            getline (read, temp_registration_date, '|');
            getline (read, temp_username, '|');
            getline (read, temp_password, '\n');

            if (temp_id == __id) {
                Customer* to_send = new Customer (temp_id, temp_name, temp_email, temp_phone, temp_username,
                temp_password, temp_cnic, temp_address, temp_registration_date);
    
                return to_send;
            }
        }

        throw "No Customer of Such ID.\n";
        return nullptr;
    } 
};

class Salesman : public Employee {
    int sales_ct;
 
public:
    Salesman () : Employee () {
        this->sales_ct = 0;
    }

    Salesman(string _id, string _name, string _email, string _phone,
    string _username, string _password, string _cnic,
    string _hireDate, double _Salary, int _ct) : Employee 
    (_id,_name,_email,_phone,_username,_password,_cnic,_hireDate,_Salary) {
        this->sales_ct = _ct;
    } 

    Salesman (const Salesman &other) : Employee (other) {
        this->sales_ct = other.sales_ct;
    }
 
    void showMenu() override {
        cout << "                            |〣--------------------------|〣 Salesman Service Menu 〣|--------------------------〣|\n\n";
        cout << "1. Sale Any Vehicle.\n";
        cout << "2. View My Profile.\n";
        cout << "3. View My Sales History.\n";
        cout << "4. Exit.\n";
    }

    string getRole() const override {
        return "Salesman";
    } 

    void disp_Info () const override {
        cout << "                            |〣--------------------------|〣 Salesman Service Menu 〣|--------------------------〣|\n\n";
        cout << "〣------------------------------------------------------------------"
        "-----------------------------------------------------------------------〣\n";
        cout << "〣           Salesman_ID: " << this->id << " ║ Name: " << this->name << " ║ CNIC: " << this->cnic
        << " ║ Salary: " << this->salary << " ║ Phone: " << this->phone;
        cout << "\n〣------------------------------------------------------------------"
        "-----------------------------------------------------------------------〣\n";
    }
 
    int getSalesCount() const {
        return this->sales_ct;
    }

    string getPasscode () const {
        return this->password;
    }

    void incrementSalesCount() {
        this->sales_ct++;
    }
 
    void sellVehicle(Inventory &inv, Customer &cust, SalesManager &sm,
    const string &modelID) {
        Vehicle* ptr = inv.findByModelID(modelID);
        int qunt = ptr->get_quantity();
        qunt --;
        inv.updateQuantity(modelID, qunt);
        string number_plt = sm.generate_PlateNumber();

        // This Code Block in taken From Online Resourses To Get Current Time.
        time_t now = time(nullptr);
        tm* local_time = localtime(&now);
        stringstream ss;
        ss << put_time(local_time, "%d-%m-%Y");
        string date_str = ss.str();

        int num = number_of_models ("sales.txt");
        num += 1;
        string s1 = to_string(num);
        string slr_id = "SL00" + s1;

        SoldVehicle veh (number_plt, modelID, ptr->getType(), ptr->get_brand(), ptr->get_model_name(), cust.getID(), this->id, date_str, ptr->get_price());
        SaleRecord rec (slr_id, modelID, ptr->getType(), cust.getID(), this->id, date_str, ptr->get_price());

        sm.record_Sale(rec,veh);
        sm.saveAll();
        inv.saveAll();
    }

    void viewMyProfile() const {
        cout << "                            |〣--------------------------|〣 Salesman Service Menu 〣|--------------------------〣|\n\n";
        cout << "〣------------------------------------------------------------------"
        "-----------------------------------------------------------------------〣\n";
        cout << "〣           Salesman_ID: " << this->id << " ║ Name: " << this->name << " ║ CNIC: " << this->cnic
        << " ║ Salary: " << this->salary << " ║ Phone: " << this->phone;
        cout << "\n〣------------------------------------------------------------------"
        "-----------------------------------------------------------------------〣\n";   
    }

    void viewMySalesHistory(SalesManager &sm) const {
        vector <SaleRecord> slr = sm.get_Sales_By_Salesman(this->id);
        if (slr.size() == 0) {
            cout << "You Have No Sales Yet.\n";
        } else {
            for (int i = 0; i < slr.size(); i++) {
                slr[i].dispRec();
            }
        }
    }
    
    static Salesman* getSalesman_ById (string __id) {
        string temp_id, temp_name, temp_email, temp_phone, temp_username, temp_password;
        string temp_cnic, temp_hireDate;
        int temp_sale_ctt;
        double temp_salary;
        string temp;

        int num = number_of_models ("salesman.txt");

        ifstream read ("salesman.txt");
        if (!read) {
            throw "\"salesman.txt\" File Can't be Opened.\n";
        } getline (read, temp, '\n');

        for (int i = 0; i < num; i++) {
            getline (read, temp_id, '|');
            getline (read, temp_name, '|');
            getline (read, temp_email, '|');
            getline (read, temp_phone, '|');
            getline (read, temp_cnic, '|');
            getline (read, temp_hireDate, '|');
            read >> temp_salary;
            read.ignore();
            read >> temp_sale_ctt;
            read.ignore();
            getline (read, temp_username, '|');
            getline (read, temp_password, '\n');

            if (temp_id == __id) {
                Salesman* to_send = new Salesman (temp_id, temp_name, temp_email, temp_phone, temp_username,
                temp_password, temp_cnic, temp_hireDate, temp_salary, temp_sale_ctt);
    
                return to_send;
            }
        }

        throw "No Salesman of Such ID.\n";
        return nullptr;
    }
};

class RepairMan : public Employee {
    string specialization;

public:
    RepairMan () : Employee () {
        this->specialization = "NULL";
    }

    RepairMan(string _id, string _name, string _email, string _phone,
    string _username, string _password, string _cnic,
    string _hireDate, double _Salary, string  _specialization) : Employee 
    (_id,_name,_email,_phone,_username,_password,_cnic,_hireDate,_Salary) {
        this->specialization = _specialization;
    } 

    RepairMan (const RepairMan &other) : Employee (other) {
        this->specialization = other.specialization;
    }

    void showMenu() override {
        cout << "                            |〣--------------------------|〣 RepairMan Service Menu 〣|--------------------------〣|\n\n";
        cout << "1. View Pending Jobs.\n";
        cout << "2. Accept Pending Jobs.\n";
        cout << "3. Mark Job Status.\n";
        cout << "4. View My Profile.\n";
        cout << "5. Exit.\n";
    }
    
    string getRole() const override {
        return "RepairMan";
    }
 
    string getSpecialization() const {
        return this->specialization;
    }

    string getPasscode () const {
        return this->password;
    }

    void disp_Info () const override {
        cout << "                            |〣--------------------------|〣 Repairman Service Menu 〣|--------------------------〣|\n\n";
        cout << "〣------------------------------------------------------------------"
        "-----------------------------------------------------------------------〣\n";
        cout << "〣           RepairMan_ID: " << this->id << " ║ Name: " << this->name << " ║ CNIC: " << this->cnic
        << " ║ Salary: " << this->salary << " ║ Phone: " << this->phone;
        cout << "\n〣------------------------------------------------------------------"
        "-----------------------------------------------------------------------〣\n";
    }
 
    void viewPendingJobs(RepairManager &rm) const {
        vector<RepairRecord> hist = rm.getPendingJobs();
        if (hist.size() == 0) {
            cout << "No Pending Jobs Right Now.\n";
        } else {
            for (int i = 0 ; i < hist.size(); i++) {
                hist[i].Disp_RepairRecord();
            }
        }
    }

    void acceptJob(RepairManager &rm, const string &repairID) {
        rm.assignRepairMan(repairID, this->id);
        rm.updateStatus(repairID, "In Progress");
        rm.saveAll();
    }

    void markJobCompleted(RepairManager &rm, const string &repairID) {
        rm.updateStatus(repairID, "Completed");
        rm.saveAll();
    }
        
    static RepairMan* get_RepairmanById (string __id) {
        string temp_id, temp_name, temp_email, temp_phone, temp_username, temp_password;
        string temp_cnic, temp_hireDate, temp_specialization;
        double temp_salary;
        string temp;

        int num = number_of_models ("repairman.txt");

        ifstream read ("repairman.txt");
        if (!read) {
            throw "\"repairman.txt\" File Can't be Opened.\n";
        } getline (read, temp, '\n');

        for (int i = 0; i < num; i++) {
            getline (read, temp_id, '|');
            getline (read, temp_name, '|');
            getline (read, temp_email, '|');
            getline (read, temp_phone, '|');
            getline (read, temp_cnic, '|');
            getline (read, temp_hireDate, '|');
            read >> temp_salary;
            read.ignore();
            getline (read, temp_specialization, '|');
            getline (read, temp_username, '|');
            getline (read, temp_password, '\n');

            if (temp_id == __id) {
                RepairMan* to_send = new RepairMan (temp_id, temp_name, temp_email, temp_phone, temp_username,
                temp_password, temp_cnic, temp_hireDate, temp_salary, temp_specialization);
    
                return to_send;
            }
        }

        throw "No RepairMan of Such ID.\n";
        return nullptr;
    }
    
};

class Admin {
    string id;
    string name;
    string username;
    string password;

public:
    Admin () {
        this->id = "NULL";
        this->name = "NULL";
        this->username = "NULL";
        this->password = "NULL";
    }

    Admin(string _id, string _name, string _username, string _password) {
        this->id = _id;
        this->name = _name;
        this->username = _username;
        this->password = _password;
    }
 
    void showMenu() {
        cout << "                            |〣--------------------------|〣 Admin Control Panel 〣|--------------------------〣|\n\n";
        cout << "1.  Add New Car Model.\n";
        cout << "2.  Add New Bike Model.\n";
        cout << "3.  Add New Scooty Model.\n";
        cout << "4.  Remove Vehicle Model.\n";
        cout << "5.  Update Vehicle Stock.\n";
        cout << "6.  View All Vehicles.\n";
        cout << "7.  Add New Customer.\n";
        cout << "8.  Remove Customer.\n";
        cout << "9.  Add New Salesman.\n";
        cout << "10. Remove Salesman.\n";
        cout << "11. Add New RepairMan.\n";
        cout << "12. Remove RepairMan.\n";
        cout << "13. Update Employee Salary.\n";
        cout << "14. View All Customers.\n";
        cout << "15. View All Salesmen.\n";
        cout << "16. View All RepairMen.\n";
        cout << "17. View Pending Repairs.\n";
        cout << "18. Assign RepairMan to Job.\n";
        cout << "19. View Sales Report.\n";
        cout << "20. Exit.\n";
    }

    string getRole() const {
        return "Admin";
    }  

    string getUsername() const { 
        return this->username; 
    }

    bool checkPassword(const string &inp) const {
        return this->password == inp; 
    }

    void addCar(Inventory &inv) {
        string temp_model_id, temp_brand, temp_model_name, temp_color, temp_fuel_type, temp_transmission;
        int temp_year, temp_quantity, temp_engine_cc, temp_mileage_kmph, temp_seatingCapacity, temp_numDoors;
        double temp_price;
        bool temp_is_electic;

        cout << "Enter Car's ID: ";
        cin >> temp_model_id;
        cout << "Enter Car's Brand: ";
        cin >> temp_brand;
        cout << "Enter Car's Name: ";
        cin.ignore();
        getline(cin, temp_model_name);
        cout << "Enter Car's Release Year: ";
        cin >> temp_year;
        cout << "Enter Car's Color: ";
        cin.ignore();
        getline(cin, temp_color);
        cout << "Enter Car's Price: ";
        cin >> temp_price;
        cout << "Enter Car's Quantity: ";
        cin >> temp_quantity;
        cout << "Is Car Electric? (0 or 1): ";\
        cin >> temp_is_electic;
        cout << "Enter Car's Engine CC: ";
        cin >> temp_engine_cc;
        cout << "Enter Car's Mileage (KMPH): ";
        cin >> temp_mileage_kmph;
        cout << "Enter Car's Fuel Type: ";
        cin >> temp_fuel_type;
        cout << "Enter Car's Seating Capacity: ";
        cin >> temp_seatingCapacity;
        cout << "Enter Car's Transmisstion (Automatic/Manual): ";
        cin >> temp_transmission;
        cout << "Enter Car's Number of Doors: ";
        cin >> temp_numDoors;

        Car nw_Car (temp_model_id, temp_brand, temp_model_name, temp_year, temp_color, temp_price,
        temp_quantity, temp_is_electic, temp_engine_cc, temp_mileage_kmph, temp_fuel_type, temp_seatingCapacity,
        temp_transmission, temp_numDoors);

        inv.addCar(nw_Car);
        inv.saveAll();
    }   

    void addBike(Inventory &inv) {
        string temp_model_id, temp_brand, temp_model_name, temp_color, temp_fuel_type, temp_bikeType;
        int temp_year, temp_quantity, temp_engine_cc, temp_mileage_kmph;
        double temp_price;
        bool temp_is_electic;

        cout << "Enter Bike's ID: ";
        cin >> temp_model_id;
        cout << "Enter Bike's Brand: ";
        cin >> temp_brand;
        cout << "Enter Bike's Name: ";
        cin.ignore();
        getline(cin, temp_model_name);
        cout << "Enter Bike's Release Year: ";
        cin >> temp_year;
        cout << "Enter Bike's Color: ";
        cin.ignore();
        getline(cin, temp_color);
        cout << "Enter Bike's Price: ";
        cin >> temp_price;
        cout << "Enter Bike's Quantity: ";
        cin >> temp_quantity;
        cout << "Is Bike Electric? (0 or 1): ";\
        cin >> temp_is_electic;
        cout << "Enter Bike's Engine CC: ";
        cin >> temp_engine_cc;
        cout << "Enter Bike's Mileage (KMPH): ";
        cin >> temp_mileage_kmph;
        cout << "Enter Bike's Fuel Type: ";
        cin >> temp_fuel_type;
        cout << "Enter Bike's Type (Standard/Sports): ";
        cin >> temp_bikeType;

        Bike nw_bike (temp_model_id, temp_brand, temp_model_name, temp_year, temp_color, temp_price,
        temp_quantity, temp_is_electic, temp_engine_cc, temp_mileage_kmph, temp_fuel_type, temp_bikeType);

        inv.addBike(nw_bike);
        inv.saveAll();
    } 

    void addScooty(Inventory &inv) {
        string temp_model_id, temp_brand, temp_model_name, temp_color, temp_fuel_type;
        int temp_year, temp_quantity, temp_engine_cc, temp_mileage_kmph;
        double temp_price, temp_stCap;
        bool temp_is_electic;

        cout << "Enter Scooty's ID: ";
        cin >> temp_model_id;
        cout << "Enter Scooty's Brand: ";
        cin >> temp_brand;
        cout << "Enter Scooty's Name: ";
        cin.ignore();
        getline(cin, temp_model_name);
        cout << "Enter Scooty's Release Year: ";
        cin >> temp_year;
        cout << "Enter Scooty's Color: ";
        cin.ignore();
        getline(cin, temp_color);
        cout << "Enter Scooty's Price: ";
        cin >> temp_price;
        cout << "Enter Scooty's Quantity: ";
        cin >> temp_quantity;
        cout << "Is Scooty Electric? (0 or 1): ";\
        cin >> temp_is_electic;
        cout << "Enter Scooty's Engine CC: ";
        cin >> temp_engine_cc;
        cout << "Enter Scooty's Mileage (KMPH): ";
        cin >> temp_mileage_kmph;
        cout << "Enter Scooty's Fuel Type: ";
        cin >> temp_fuel_type;
        cout << "Enter Scooty's Storage Capacity (Litres): ";
        cin >> temp_stCap;

        Scooty nw_scooty (temp_model_id, temp_brand, temp_model_name, temp_year, temp_color, temp_price,
        temp_quantity, temp_is_electic, temp_engine_cc, temp_mileage_kmph, temp_fuel_type, temp_stCap);

        inv.addScooty(nw_scooty);
        inv.saveAll();
    } 

    void removeVehicleModel(Inventory &inv, const string &modelID) {
        inv.removeModel (modelID);
        inv.saveAll();
    }

    void updateStock(Inventory &inv, const string &modelID, int newQuantity) {
        inv.updateQuantity(modelID, newQuantity);
    }
 
    void addSalesman(vector<Salesman> &salesmen) {
        string id, name, email, phone, username, password, cnic;
        double salary;

        time_t now = time(nullptr);
        tm* local_time = localtime(&now);
        stringstream ss;
        ss << put_time(local_time, "%d-%m-%Y");
        string date_str = ss.str();

        cout << "Enter ID: ";
        cin >> id;
        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Email: ";
        cin >> email;
        cout << "Enter Phone: ";
        cin >> phone;
        cout << "Enter CNIC: ";
        cin >> cnic;
        cout << "Enter Salary: ";
        cin >> salary;
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;

        Salesman nw_sm (id, name, email, phone, username, password, cnic, date_str, salary, 0);
        salesmen.push_back(nw_sm);
    }
    

    void removeSalesman(vector<Salesman> &salesmen, const string &salesmanID) {
        for (int i = 0; i < salesmen.size(); i++) {
            if (salesmen[i].getID() == salesmanID) {
                salesmen.erase(salesmen.begin() + i);  //Learned this methord After a Google search.
                return;
            }
        }

        throw "Salesman Not Found.\n";
        return;
    }

    void addRepairMan(vector<RepairMan> &repairmen) {
        string id, name, email, phone, username, password, cnic, special;
        double salary;

        time_t now = time(nullptr);
        tm* local_time = localtime(&now);
        stringstream ss;
        ss << put_time(local_time, "%d-%m-%Y");
        string date_str = ss.str();

        cout << "Enter ID: ";
        cin >> id;
        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Email: ";
        cin >> email;
        cout << "Enter Phone: ";
        cin >> phone;
        cout << "Enter CNIC: ";
        cin >> cnic;
        cout << "Enter Salary: ";
        cin >> salary;
        cout << "Enter Specialization: ";
        cin >> special;
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;

        RepairMan nw_rm (id, name, email, phone, username, password, cnic, date_str, salary, special);
        repairmen.push_back(nw_rm);
    }
    void removeRepairMan(vector<RepairMan> &repairmen, const string &repairManID) {
        for (int i = 0; i < repairmen.size(); i++) {
            if (repairmen[i].getID() == repairManID) {
                repairmen.erase(repairmen.begin() + i);
                return;
            }
        }

        throw "Repairman Not Found.\n";
        return;
    }

    void paySalary(Employee &emp, double amount) {
        return;
    }
 
    void addCustomer(vector<Customer> &customers) {
        string id, name, email, phone, username, password, cnic, address;

        time_t now = time(nullptr);
        tm* local_time = localtime(&now);
        stringstream ss;
        ss << put_time(local_time, "%d-%m-%Y");
        string date_str = ss.str();

        cout << "Enter Customer ID: ";
        cin >> id;
        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Email: ";
        cin >> email;
        cout << "Enter Phone: ";
        cin >> phone;
        cout << "Enter CNIC: ";
        cin >> cnic;
        cout << "Enter Address: ";
        cin.ignore();
        getline(cin, address);
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;

        Customer nw_cust (id, name, email, phone, username, password, cnic, address, date_str);
        customers.push_back(nw_cust);
    }

    void removeCustomer(vector<Customer> &customers, const string &customerID) {
        for (int i = 0; i < customers.size(); i++) {
            if (customers[i].getID() == customerID) {
                customers.erase(customers.begin() + i);
                return;
            }
        }

        throw "Customer Not Found.\n";
    }
 
    void viewSalesReport(SalesManager &sm) const {
        cout << "                            |〣--------------------------|〣 Sales Report 〣|--------------------------〣|\n\n";
        cout << "〣------------------------------------------------------------------"
        "-----------------------------〣\n";
        cout << "〣      Total Sales: " << sm.get_Total_Sales_Count() << " ║ Total Revenue: " << sm.get_Total_Revenue();
        cout << "\n〣------------------------------------------------------------------"
        "-----------------------------〣\n";
    }

    void viewAllVehicles(Inventory &inv) const {
        cout << "                            |〣--------------------------|〣 All Vehicles 〣|--------------------------〣|\n\n";
        inv.Changeable_disp();
    }

    void viewAllSalesmen(const vector<Salesman> &salesmen) const {
        cout << "                            |〣--------------------------|〣 All Salesmen 〣|--------------------------〣|\n\n";
        if (salesmen.size() == 0) {
            cout << "No Salesmen On Record.\n";
            return;
        }
        for (int i = 0; i < salesmen.size(); i++) {
            salesmen[i].disp_Info();
        }
    }

    void viewAllRepairmen(const vector<RepairMan> &repairmen) const {
        cout << "                            |〣--------------------------|〣 All Repairmen 〣|--------------------------〣|\n\n";
        if (repairmen.size() == 0) {
            cout << "No Repairmen On Record.\n";
            return;
        }
        for (int i = 0; i < repairmen.size(); i++) {
            repairmen[i].disp_Info();
        }
    }

    void viewAllCustomers(const vector<Customer> &customers) const {
        cout << "                            |〣--------------------------|〣 All Customers 〣|--------------------------〣|\n\n";
        if (customers.size() == 0) {
            cout << "No Customers On Record.\n";
            return;
        }
        for (int i = 0; i < customers.size(); i++) {
            customers[i].disp_Info();
        }
    }

    void viewPendingRepairs(RepairManager &rm) const {
        cout << "                            |〣--------------------------|〣 Pending Repairs 〣|--------------------------〣|\n\n";
        vector<RepairRecord> rem = rm.getPendingJobs();
        if (rem.size() == 0) {
            cout << "No Pending Repairs Right Now.\n";
            return;
        }
        for (int i = 0; i < rem.size(); i++) {
            rem[i].Disp_RepairRecord();
        }
    }
 
    static Admin* getAdmin () {
        string temp, id, name, username, passcode_;

        ifstream read ("admin.txt");
        if (!read) {
            throw "\"repairman.txt\" File Can't be Opened.\n";
        } getline (read, temp, '\n');

        getline (read,id,'|');
        getline (read,name,'|');
        getline (read,username,'|');
        getline (read,passcode_,'\n');

        read.close();

        Admin* deliver = new Admin (id,name,username,passcode_);
        return deliver;
    }
};

class ShowroomSystem {
    Inventory inventory;
    SalesManager salesManager;
    RepairManager repairManager;
 
    vector<Admin> admins;
    vector<Salesman> salesmen;
    vector<Customer> customers;
    vector<RepairMan> repairmen;
    vector <string> introLines;
 
public:
    void loadEverything() {
        string temp_id, temp_name, temp_email, temp_phone, temp_username, temp_password;
        string temp_cnic, temp_hireDate, temp_address, temp_registration_date, temp_specialization;
        int temp_sales_ct;
        double temp_salary;
        string temp;

        this->inventory.loadAll();
        this->salesManager.loadAll();
        this->repairManager.loadAll();

        int num = number_of_models("admin.txt");

        ifstream read("admin.txt");
        if (!read) {
            throw "\"admin.txt\" File Can't be Opened.\n";
        } getline(read, temp, '\n');

        introLines.push_back (temp);

        for (int i = 0; i < num; i++) {
            string a_id, a_name, a_username, a_password;
            getline(read, a_id, '|');
            getline(read, a_name, '|');
            getline(read, a_username, '|');
            getline(read, a_password, '\n');

            Admin nw_admin(a_id, a_name, a_username, a_password);
            this->admins.push_back(nw_admin);
        } read.close();

        num = number_of_models("salesman.txt");

        read.open("salesman.txt");
        if (!read) {
            throw "\"salesman.txt\" File Can't be Opened.\n";
        } getline(read, temp, '\n');

        introLines.push_back (temp);

        for (int i = 0; i < num; i++) {
            getline(read, temp_id, '|');
            getline(read, temp_name, '|');
            getline(read, temp_email, '|');
            getline(read, temp_phone, '|');
            getline(read, temp_cnic, '|');
            getline(read, temp_hireDate, '|');
            read >> temp_salary;
            read.ignore();
            read >> temp_sales_ct;
            read.ignore();
            getline(read, temp_username, '|');
            getline(read, temp_password, '\n');

            Salesman nw_sm(temp_id, temp_name, temp_email, temp_phone, temp_username,
            temp_password, temp_cnic, temp_hireDate, temp_salary, temp_sales_ct);
            this->salesmen.push_back(nw_sm);
        } read.close();

        num = number_of_models("customer.txt");

        read.open("customer.txt");
        if (!read) {
            throw "\"customer.txt\" File Can't be Opened.\n";
        } getline(read, temp, '\n');

        introLines.push_back(temp);

        for (int i = 0; i < num; i++) {
            getline(read, temp_id, '|');
            getline(read, temp_name, '|');
            getline(read, temp_email, '|');
            getline(read, temp_phone, '|');
            getline(read, temp_cnic, '|');
            getline(read, temp_address, '|');
            getline(read, temp_registration_date, '|');
            getline(read, temp_username, '|');
            getline(read, temp_password, '\n');

            Customer nw_cust(temp_id, temp_name, temp_email, temp_phone, temp_username,
            temp_password, temp_cnic, temp_address, temp_registration_date);
            this->customers.push_back(nw_cust);
        } read.close();

        num = number_of_models("repairman.txt");

        read.open("repairman.txt");
        if (!read) {
            throw "\"repairman.txt\" File Can't be Opened.\n";
        } getline(read, temp, '\n');

        introLines.push_back(temp);

        for (int i = 0; i < num; i++) {
            getline(read, temp_id, '|');
            getline(read, temp_name, '|');
            getline(read, temp_email, '|');
            getline(read, temp_phone, '|');
            getline(read, temp_cnic, '|');
            getline(read, temp_hireDate, '|');
            read >> temp_salary;
            read.ignore();
            getline(read, temp_specialization, '|');
            getline(read, temp_username, '|');
            getline(read, temp_password, '\n');

            RepairMan nw_rm(temp_id, temp_name, temp_email, temp_phone, temp_username,
            temp_password, temp_cnic, temp_hireDate, temp_salary, temp_specialization);
            this->repairmen.push_back(nw_rm);
        
        } read.close();
    }

    void run() {
        this->loadEverything();

        enum Screen { MAIN, ADMIN_MENU, CUSTOMER_MENU,
                    SALESMAN_MENU, REPAIRMAN_MENU };
        Screen screen = MAIN;

        bool loginOpen = false;
        bool adminLogin = false;
        std::vector<InputBox> loginFields = {
            {{}, "Username", "", false},
            {{}, "Password", "", true }
        };

        Person*   loggedPerson = nullptr;
        Admin*    loggedAdmin  = nullptr;

        std::vector<std::string> infoLines;
        int infoScroll = 0;
        bool showingInfo = false;

        bool      subModalOpen = false;
        std::string subModalTitle;
        std::vector<InputBox> subFields;
        std::function<void()> subConfirmCb;   

        auto openSub = [&](const std::string& title,
            std::vector<std::pair<std::string,bool>> fieldDefs,
            std::function<void()> cb) {
            subModalTitle = title;
            subFields.clear();
            for (auto& d : fieldDefs)
                subFields.push_back({{}, d.first, "", d.second});
            subConfirmCb = cb;
            subModalOpen = true;
            showingInfo  = false;
        };

        auto captureInventory = [&](const std::string& type) {
            infoLines.clear();
            infoScroll = 0;
            auto addVeh = [&](Vehicle* v) {
                infoLines.push_back(
                    "ID:" + v->get_model_id() +
                    "  Brand:" + v->get_brand() +
                    "  Model:" + v->get_model_name() +
                    "  Year:"  + std::to_string(v->get_year()) +
                    "  Price:" + std::to_string((int)v->get_price()) +
                    "  Qty:"   + std::to_string(v->get_quantity()) +
                    "  Fuel:"  + v->get_fuel_type());
            };
            if (type == "Car" || type == "All")
                for (auto p : inventory.getCars())   addVeh(p);
            if (type == "Bike" || type == "All")
                for (auto p : inventory.getBikes())  addVeh(p);
            if (type == "Scooty" || type == "All")
                for (auto p : inventory.getScooties()) addVeh(p);
            showingInfo = true;
        };

        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(BG);

            if (screen == MAIN) {
                DrawTextEx(gFont, "Showroom Management System",
                {40, 50}, 52, 1, ACCENT);
                DrawTextEx(gFont, "Please log in to continue",
                {40, 125}, 30, 1, TXTSUB);

                Button bLogin    {{40, 240, 500, 90}, "Staff / Customer Login"};
                Button bAdmLogin {{40, 350, 500, 90}, "Admin Login"};
                Button bExit     {{40, 460, 500, 90}, "Exit", BTN, BTNHOV, ACCENT2};

                if (bLogin.draw())    { 
                    adminLogin=false; loginOpen=true;
                    loginFields[0].value=""; loginFields[1].value="";
                }
                if (bAdmLogin.draw()) { 
                    adminLogin=true;  loginOpen=true;
                    loginFields[0].value=""; loginFields[1].value=""; 
                }
                if (bExit.draw()) {
                    EndDrawing();
                    break; 
                }
                if (loginOpen) {
                    bool confirmed = drawFormModal(
                        adminLogin ? "Admin Login" : "Staff / Customer Login",
                        loginFields, "Login");

                    if (IsKeyPressed(KEY_ESCAPE)) {
                        loginOpen = false;
                    }
                    if (confirmed) {
                        loginOpen = false;
                        std::string uname = loginFields[0].value;
                        std::string pwd   = loginFields[1].value;

                        if (adminLogin) {
                            for (auto& a : admins) {
                                if (a.getUsername()==uname && a.checkPassword(pwd)) {
                                    loggedAdmin = &a;
                                    screen = ADMIN_MENU;
                                    break;
                                }
                            }
                            if (!loggedAdmin) showNotif("Invalid admin credentials.", true);
                        } else {
                            loggedPerson = authenticateLogin(uname, pwd);
                            if (!loggedPerson) {
                                showNotif("Invalid credentials.", true);
                            } else {
                                std::string role = loggedPerson->getRole();
                                if (role == "Customer")  screen = CUSTOMER_MENU;
                                if (role == "Salesman")  screen = SALESMAN_MENU;
                                if (role == "RepairMan") screen = REPAIRMAN_MENU;
                            }
                        }
                    }
                }
            }

            else if (screen == ADMIN_MENU) {
                DrawTextEx(gFont, "Admin Control Panel",
                        {40, 14}, 38, 1, ACCENT);
                Button bBack{{SW-240,12,220,66},"Logout",BTN,BTNHOV,ACCENT2};
                if (bBack.draw()) {
                    loggedAdmin=nullptr; screen=MAIN;
                    showingInfo=false; subModalOpen=false;
                }

                struct Item { std::string label; int id; };
                std::vector<Item> items = {
                    {"Add Car",1},{"Add Bike",2},{"Add Scooty",3},
                    {"Remove Vehicle",4},{"Update Stock",5},{"View All Vehicles",6},
                    {"Add Customer",7},{"Remove Customer",8},
                    {"Add Salesman",9},{"Remove Salesman",10},
                    {"Add RepairMan",11},{"Remove RepairMan",12},
                    {"Update Salary",13},
                    {"View Customers",14},{"View Salesmen",15},{"View RepairMen",16},
                    {"Pending Repairs",17},{"Assign RepairMan",18},
                    {"Sales Report",19}
                };
                int cols=3; float bw=360,bh=76,pad=16;
                float startX=40, startY=96;
                for (int i=0;i<(int)items.size();i++){
                    int c=i%cols, r=i/cols;
                    Button b{{startX+c*(bw+pad), startY+r*(bh+pad), bw, bh},
                            items[i].label};
                    if (b.draw() && !subModalOpen && !showingInfo) {
                        int id = items[i].id;
                        try {
                            if (id==1) openSub("Add Car",
                                {{"Model ID",false},{"Brand",false},{"Model Name",false},
                                {"Year",false},{"Color",false},{"Price",false},
                                {"Quantity",false},{"Electric (0/1)",false},
                                {"Engine CC",false},{"Mileage KMPH",false},
                                {"Fuel Type",false},{"Seating Capacity",false},
                                {"Transmission",false},{"Num Doors",false}},
                                [&](){
                                    auto& f=subFields;
                                    Car c(f[0].value,f[1].value,f[2].value,
                                        stoi(f[3].value),f[4].value,stod(f[5].value),
                                        stoi(f[6].value),(bool)stoi(f[7].value),
                                        stoi(f[8].value),stoi(f[9].value),f[10].value,
                                        stoi(f[11].value),f[12].value,stoi(f[13].value));
                                    inventory.addCar(c); inventory.saveAll();
                                    showNotif("Car added.");
                                });
                            else if (id==2) openSub("Add Bike",
                                {{"Model ID",false},{"Brand",false},{"Model Name",false},
                                {"Year",false},{"Color",false},{"Price",false},
                                {"Quantity",false},{"Electric (0/1)",false},
                                {"Engine CC",false},{"Mileage KMPH",false},
                                {"Fuel Type",false},{"Bike Type",false}},
                                [&](){
                                    auto& f=subFields;
                                    Bike b(f[0].value,f[1].value,f[2].value,
                                        stoi(f[3].value),f[4].value,stod(f[5].value),
                                        stoi(f[6].value),(bool)stoi(f[7].value),
                                        stoi(f[8].value),stoi(f[9].value),
                                        f[10].value,f[11].value);
                                    inventory.addBike(b); inventory.saveAll();
                                    showNotif("Bike added.");
                                });
                            else if (id==3) openSub("Add Scooty",
                                {{"Model ID",false},{"Brand",false},{"Model Name",false},
                                {"Year",false},{"Color",false},{"Price",false},
                                {"Quantity",false},{"Electric (0/1)",false},
                                {"Engine CC",false},{"Mileage KMPH",false},
                                {"Fuel Type",false},{"Storage Capacity L",false}},
                                [&](){
                                    auto& f=subFields;
                                    Scooty s(f[0].value,f[1].value,f[2].value,
                                        stoi(f[3].value),f[4].value,stod(f[5].value),
                                        stoi(f[6].value),(bool)stoi(f[7].value),
                                        stoi(f[8].value),stoi(f[9].value),
                                        f[10].value,stod(f[11].value));
                                    inventory.addScooty(s); inventory.saveAll();
                                    showNotif("Scooty added.");
                                });
                            else if (id==4) openSub("Remove Vehicle",
                                {{"Model ID",false}},
                                [&](){ inventory.removeModel(subFields[0].value);
                                    inventory.saveAll(); showNotif("Removed."); });
                            else if (id==5) openSub("Update Stock",
                                {{"Model ID",false},{"New Quantity",false}},
                                [&](){ inventory.updateQuantity(subFields[0].value,
                                        stoi(subFields[1].value));
                                    inventory.saveAll(); showNotif("Stock updated."); });
                            else if (id==6) { captureInventory("All"); }
                            else if (id==7) openSub("Add Customer",
                                {{"Customer ID",false},{"Name",false},{"Email",false},
                                {"Phone",false},{"CNIC",false},{"Address",false},
                                {"Username",false},{"Password",true}},
                                [&](){
                                    auto& f=subFields;
                                    time_t now=time(nullptr); tm* lt=localtime(&now);
                                    std::stringstream ss; ss<<std::put_time(lt,"%d-%m-%Y");
                                    Customer c(f[0].value,f[1].value,f[2].value,
                                        f[3].value,f[6].value,f[7].value,
                                        f[4].value,f[5].value,ss.str());
                                    customers.push_back(c); saveEverything();
                                    showNotif("Customer added.");
                                });
                            else if (id==8) openSub("Remove Customer",
                                {{"Customer ID",false}},
                                [&](){ loggedAdmin->removeCustomer(customers,subFields[0].value);
                                    saveEverything(); showNotif("Customer removed."); });
                            else if (id==9) openSub("Add Salesman",
                                {{"ID",false},{"Name",false},{"Email",false},
                                {"Phone",false},{"CNIC",false},{"Salary",false},
                                {"Username",false},{"Password",true}},
                                [&](){
                                    auto& f=subFields;
                                    time_t now=time(nullptr); tm* lt=localtime(&now);
                                    std::stringstream ss; ss<<std::put_time(lt,"%d-%m-%Y");
                                    Salesman s(f[0].value,f[1].value,f[2].value,
                                        f[3].value,f[6].value,f[7].value,
                                        f[4].value,ss.str(),stod(f[5].value),0);
                                    salesmen.push_back(s); saveEverything();
                                    showNotif("Salesman added.");
                                });
                            else if (id==10) openSub("Remove Salesman",
                                {{"Salesman ID",false}},
                                [&](){ loggedAdmin->removeSalesman(salesmen,subFields[0].value);
                                    saveEverything(); showNotif("Salesman removed."); });
                            else if (id==11) openSub("Add RepairMan",
                                {{"ID",false},{"Name",false},{"Email",false},
                                {"Phone",false},{"CNIC",false},{"Salary",false},
                                {"Specialization",false},{"Username",false},{"Password",true}},
                                [&](){
                                    auto& f=subFields;
                                    time_t now=time(nullptr); tm* lt=localtime(&now);
                                    std::stringstream ss; ss<<std::put_time(lt,"%d-%m-%Y");
                                    RepairMan r(f[0].value,f[1].value,f[2].value,
                                        f[3].value,f[7].value,f[8].value,
                                        f[4].value,ss.str(),stod(f[5].value),f[6].value);
                                    repairmen.push_back(r); saveEverything();
                                    showNotif("RepairMan added.");
                                });
                            else if (id==12) openSub("Remove RepairMan",
                                {{"RepairMan ID",false}},
                                [&](){ loggedAdmin->removeRepairMan(repairmen,subFields[0].value);
                                    saveEverything(); showNotif("RepairMan removed."); });
                            else if (id==13) openSub("Update Salary",
                                {{"Employee Type (Salesman/RepairMan)",false},
                                {"Employee ID",false},{"New Salary",false}},
                                [&](){
                                    std::string type=subFields[0].value;
                                    std::string eid =subFields[1].value;
                                    double sal=stod(subFields[2].value);
                                    if(type=="Salesman"){
                                        for(auto& s:salesmen) if(s.getID()==eid){s.setSalary(sal);break;}
                                    } else {
                                        for(auto& r:repairmen) if(r.getID()==eid){r.setSalary(sal);break;}
                                    }
                                    saveEverything(); showNotif("Salary updated.");
                                });
                            else if (id==14){
                                infoLines.clear(); infoScroll=0;
                                for(auto& c:customers)
                                    infoLines.push_back(
                                        "ID:"+c.getID()+"  Name:"+c.getName()+
                                        "  Phone:"+c.getPhone()+"  CNIC:"+c.getCNIC());
                                showingInfo=true;
                            }
                            else if (id==15){
                                infoLines.clear(); infoScroll=0;
                                for(auto& s:salesmen)
                                    infoLines.push_back(
                                        "ID:"+s.getID()+"  Name:"+s.getName()+
                                        "  Salary:"+std::to_string((int)s.getSalary())+
                                        "  Sales:"+std::to_string(s.getSalesCount()));
                                showingInfo=true;
                            }
                            else if (id==16){
                                infoLines.clear(); infoScroll=0;
                                for(auto& r:repairmen)
                                    infoLines.push_back(
                                        "ID:"+r.getID()+"  Name:"+r.getName()+
                                        "  Spec:"+r.getSpecialization()+
                                        "  Salary:"+std::to_string((int)r.getSalary()));
                                showingInfo=true;
                            }
                            else if (id==17){
                                auto pend=repairManager.getPendingJobs();
                                infoLines.clear(); infoScroll=0;
                                for(auto& rr:pend)
                                    infoLines.push_back(
                                        "RepID:"+rr.get_RepairID()+
                                        "  Plate:"+rr.get_PlateNumber()+
                                        "  Part:"+rr.get_PartChanged()+
                                        "  Cost:"+std::to_string((int)rr.get_Cost()));
                                if(infoLines.empty()) infoLines.push_back("No pending repairs.");
                                showingInfo=true;
                            }
                            else if (id==18) openSub("Assign RepairMan",
                                {{"Repair ID",false},{"RepairMan ID",false}},
                                [&](){ repairManager.assignRepairMan(
                                        subFields[0].value,subFields[1].value);
                                    repairManager.saveAll();
                                    showNotif("RepairMan assigned."); });
                            else if (id==19){
                                infoLines.clear(); infoScroll=0;
                                infoLines.push_back(
                                    "Total Sales: "+std::to_string(salesManager.get_Total_Sales_Count()));
                                infoLines.push_back(
                                    "Total Revenue: "+std::to_string((long long)salesManager.get_Total_Revenue()));
                                showingInfo=true;
                            }
                        } catch(const char* e){ showNotif(e,true); }
                    }
                }

                if (showingInfo) {
                    DrawRectangle(0,0,SW,SH,{0,0,0,160});
                    Rectangle area={60,60,(float)SW-120,(float)SH-140};
                    DrawRectangleRec(area,PANEL);
                    DrawRectangleLinesEx(area,2,ACCENT);
                    drawInfoList(infoLines,{area.x+8,area.y+8,area.width-16,area.height-56},infoScroll);
                    Button bClose{{area.x+area.width-230,area.y+area.height-72,210,58},"Close"};
                    if(bClose.draw()||IsKeyPressed(KEY_ESCAPE)) showingInfo=false;
                }

                if (subModalOpen) {
                    bool confirmed = drawFormModal(subModalTitle, subFields);
                    if (confirmed) {
                        try { subConfirmCb(); } catch(const char* e){ showNotif(e,true); }
                        subModalOpen=false;
                    }
                    if (IsKeyPressed(KEY_ESCAPE)) subModalOpen=false;
                }
            }

            else if (screen == CUSTOMER_MENU) {
                Customer* cust = (Customer*)loggedPerson;
                DrawTextEx(gFont,
                    ("Welcome, " + cust->getName()).c_str(),
                    {40,14},36,1,ACCENT);
                Button bBack{{SW-240,12,220,66},"Logout",BTN,BTNHOV,ACCENT2};
                if(bBack.draw()){
                    loggedPerson=nullptr; screen=MAIN;
                    showingInfo=false; subModalOpen=false;
                }

                struct Item{ std::string label; int id; };
                std::vector<Item> items={
                    {"Browse Cars",1},{"Browse Bikes",2},{"Browse Scooties",3},
                    {"Browse All",4},{"My Purchases",5},
                    {"My Repair History",6},{"Request Repair",7}
                };
                float bw=380,bh=84,pad=16,sx=40,sy=104;
                int cols=3;
                for(int i=0;i<(int)items.size();i++){
                    int c=i%cols,r=i/cols;
                    Button b{{sx+c*(bw+pad),sy+r*(bh+pad),bw,bh},items[i].label};
                    if(b.draw()&&!subModalOpen&&!showingInfo){
                        int id=items[i].id;
                        try{
                            if(id>=1&&id<=4){
                                std::string t=(id==1?"Car":id==2?"Bike":id==3?"Scooty":"All");
                                captureInventory(t);
                            } else if(id==5){
                                auto purch=salesManager.get_Purchases_By_Customer(cust->getID());
                                infoLines.clear(); infoScroll=0;
                                for(auto& sv:purch)
                                    infoLines.push_back(
                                        "Plate:"+sv.getPlateNumber()+
                                        "  Model:"+sv.getModelID()+
                                        "  Brand:"+sv.getBrand()+
                                        "  Date:"+sv.getSaleDate()+
                                        "  Price:"+std::to_string((int)sv.getSalePrice()));
                                if(infoLines.empty()) infoLines.push_back("No purchases yet.");
                                showingInfo=true;
                            } else if(id==6){
                                auto hist=repairManager.getHistoryByCustomer(cust->getID());
                                infoLines.clear(); infoScroll=0;
                                for(auto& rr:hist)
                                    infoLines.push_back(
                                        "RepID:"+rr.get_RepairID()+
                                        "  Plate:"+rr.get_PlateNumber()+
                                        "  Part:"+rr.get_PartChanged()+
                                        "  Status:"+rr.get_Status());
                                if(infoLines.empty()) infoLines.push_back("No repair history.");
                                showingInfo=true;
                            } else if(id==7){
                                openSub("Request Vehicle Repair",
                                    {{"Plate Number",false},
                                    {"Vehicle Type (Car/Bike/Scooty)",false},
                                    {"Part to Repair",false}},
                                    [&,cust](){
                                        cust->requestVehicleRepair(repairManager,
                                            subFields[0].value,subFields[1].value,
                                            subFields[2].value);
                                        repairManager.saveAll();
                                        showNotif("Repair request submitted.");
                                    });
                            }
                        } catch(const char* e){ showNotif(e,true); }
                    }
                }

                if(showingInfo){
                    DrawRectangle(0,0,SW,SH,{0,0,0,160});
                    Rectangle area={60,60,(float)SW-120,(float)SH-140};
                    DrawRectangleRec(area,PANEL);
                    DrawRectangleLinesEx(area,2,ACCENT);
                    drawInfoList(infoLines,{area.x+8,area.y+8,area.width-16,area.height-56},infoScroll);
                    Button bClose{{area.x+area.width-230,area.y+area.height-72,210,58},"Close"};
                    if(bClose.draw()||IsKeyPressed(KEY_ESCAPE)) showingInfo=false;
                }
                if(subModalOpen){
                    bool confirmed=drawFormModal(subModalTitle,subFields);
                    if(confirmed){
                        try{ subConfirmCb(); } catch(const char* e){ showNotif(e,true); }
                        subModalOpen=false;
                    }
                    if(IsKeyPressed(KEY_ESCAPE)) subModalOpen=false;
                }
            }

            else if (screen == SALESMAN_MENU) {
                Salesman* sm = (Salesman*)loggedPerson;
                DrawTextEx(gFont,
                    ("Salesman: " + sm->getName()).c_str(),
                    {40,14},36,1,ACCENT);
                Button bBack{{SW-240,12,220,66},"Logout",BTN,BTNHOV,ACCENT2};
                if(bBack.draw()){
                    loggedPerson=nullptr; screen=MAIN;
                    showingInfo=false; subModalOpen=false;
                }

                struct Item{ std::string label; int id; };
                std::vector<Item> items={
                    {"Sell Vehicle",1},{"My Profile",2},{"My Sales History",3}
                };
                float bw=440,bh=88,pad=18,sx=40,sy=110;
                for(int i=0;i<(int)items.size();i++){
                    Button b{{sx,sy+i*(bh+pad),bw,bh},items[i].label};
                    if(b.draw()&&!subModalOpen&&!showingInfo){
                        int id=items[i].id;
                        try{
                            if(id==1){
                                openSub("Sell Vehicle",
                                    {{"Model ID",false},{"Customer ID",false}},
                                    [&,sm](){
                                        Customer* c=Customer::getCustomer_ById(subFields[1].value);
                                        sm->sellVehicle(inventory,*c,salesManager,subFields[0].value);
                                        sm->incrementSalesCount();
                                        saveEverything(); delete c;
                                        showNotif("Vehicle sold successfully.");
                                    });
                            } else if(id==2){
                                infoLines.clear(); infoScroll=0;
                                infoLines.push_back("ID:    "+sm->getID());
                                infoLines.push_back("Name:  "+sm->getName());
                                infoLines.push_back("Phone: "+sm->getPhone());
                                infoLines.push_back("CNIC:  "+sm->getCNIC());
                                infoLines.push_back("Hired: "+sm->getHireDate());
                                infoLines.push_back("Salary:"+std::to_string((int)sm->getSalary()));
                                infoLines.push_back("Sales: "+std::to_string(sm->getSalesCount()));
                                showingInfo=true;
                            } else if(id==3){
                                auto hist=salesManager.get_Sales_By_Salesman(sm->getID());
                                infoLines.clear(); infoScroll=0;
                                for(auto& sr:hist)
                                    infoLines.push_back(
                                        "SaleID:"+sr.getSaleID()+
                                        "  Model:"+sr.getModelID()+
                                        "  Date:"+sr.getSaleDate()+
                                        "  Price:"+std::to_string((int)sr.getSalePrice()));
                                if(infoLines.empty()) infoLines.push_back("No sales yet.");
                                showingInfo=true;
                            }
                        } catch(const char* e){ showNotif(e,true); }
                    }
                }

                if(showingInfo){
                    DrawRectangle(0,0,SW,SH,{0,0,0,160});
                    Rectangle area={60,60,(float)SW-120,(float)SH-140};
                    DrawRectangleRec(area,PANEL);
                    DrawRectangleLinesEx(area,2,ACCENT);
                    drawInfoList(infoLines,{area.x+8,area.y+8,area.width-16,area.height-56},infoScroll);
                    Button bClose{{area.x+area.width-230,area.y+area.height-72,210,58},"Close"};
                    if(bClose.draw()||IsKeyPressed(KEY_ESCAPE)) showingInfo=false;
                }
                if(subModalOpen){
                    bool confirmed=drawFormModal(subModalTitle,subFields);
                    if(confirmed){
                        try{ subConfirmCb(); } catch(const char* e){ showNotif(e,true); }
                        subModalOpen=false;
                    }
                    if(IsKeyPressed(KEY_ESCAPE)) subModalOpen=false;
                }
            }

            else if (screen == REPAIRMAN_MENU) {
                RepairMan* rm = (RepairMan*)loggedPerson;
                DrawTextEx(gFont,
                    ("RepairMan: " + rm->getName()).c_str(),
                    {40,14},36,1,ACCENT);
                Button bBack{{SW-240,12,220,66},"Logout",BTN,BTNHOV,ACCENT2};
                if(bBack.draw()){
                    loggedPerson=nullptr; screen=MAIN;
                    showingInfo=false; subModalOpen=false;
                }

                struct Item{ std::string label; int id; };
                std::vector<Item> items={
                    {"View Pending Jobs",1},{"Accept Job",2},
                    {"Mark Job Completed",3},{"My Profile",4}
                };
                float bw=440,bh=88,pad=18,sx=40,sy=110;
                for(int i=0;i<(int)items.size();i++){
                    Button b{{sx,sy+i*(bh+pad),bw,bh},items[i].label};
                    if(b.draw()&&!subModalOpen&&!showingInfo){
                        int id=items[i].id;
                        try{
                            if(id==1){
                                auto pend=repairManager.getPendingJobs();
                                infoLines.clear(); infoScroll=0;
                                for(auto& rr:pend)
                                    infoLines.push_back(
                                        "RepID:"+rr.get_RepairID()+
                                        "  Cust:"+rr.get_CustomerID()+
                                        "  Plate:"+rr.get_PlateNumber()+
                                        "  Part:"+rr.get_PartChanged()+
                                        "  Cost:"+std::to_string((int)rr.get_Cost()));
                                if(infoLines.empty()) infoLines.push_back("No pending jobs.");
                                showingInfo=true;
                            } else if(id==2){
                                openSub("Accept Job",
                                    {{"Repair ID",false}},
                                    [&,rm](){
                                        rm->acceptJob(repairManager,subFields[0].value);
                                        showNotif("Job accepted.");
                                    });
                            } else if(id==3){
                                openSub("Mark Job Completed",
                                    {{"Repair ID",false}},
                                    [&,rm](){
                                        rm->markJobCompleted(repairManager,subFields[0].value);
                                        showNotif("Job marked completed.");
                                    });
                            } else if(id==4){
                                infoLines.clear(); infoScroll=0;
                                infoLines.push_back("ID:    "+rm->getID());
                                infoLines.push_back("Name:  "+rm->getName());
                                infoLines.push_back("Phone: "+rm->getPhone());
                                infoLines.push_back("CNIC:  "+rm->getCNIC());
                                infoLines.push_back("Spec:  "+rm->getSpecialization());
                                infoLines.push_back("Salary:"+std::to_string((int)rm->getSalary()));
                                showingInfo=true;
                            }
                        } catch(const char* e){ showNotif(e,true); }
                    }
                }

                if(showingInfo){
                    DrawRectangle(0,0,SW,SH,{0,0,0,160});
                    Rectangle area={60,60,(float)SW-120,(float)SH-140};
                    DrawRectangleRec(area,PANEL);
                    DrawRectangleLinesEx(area,2,ACCENT);
                    drawInfoList(infoLines,{area.x+8,area.y+8,area.width-16,area.height-56},infoScroll);
                    Button bClose{{area.x+area.width-230,area.y+area.height-72,210,58},"Close"};
                    if(bClose.draw()||IsKeyPressed(KEY_ESCAPE)) showingInfo=false;
                }
                if(subModalOpen){
                    bool confirmed=drawFormModal(subModalTitle,subFields);
                    if(confirmed){
                        try{ subConfirmCb(); } catch(const char* e){ showNotif(e,true); }
                        subModalOpen=false;
                    }
                    if(IsKeyPressed(KEY_ESCAPE)) subModalOpen=false;
                }
            }

            drawNotif();
            EndDrawing();
        }
    }
 
    Person* authenticateLogin(const string &username, const string &password) {
        for (int i = 0; i < this->salesmen.size(); i++) {
            if (this->salesmen[i].getUsername() == username && this->salesmen[i].checkPassword(password)) {
                return &(this->salesmen[i]);
            }
        }
        for (int i = 0; i < this->customers.size(); i++) {
            if (this->customers[i].getUsername() == username && this->customers[i].checkPassword(password)) {
                return &(this->customers[i]);
            }
        }
        for (int i = 0; i < this->repairmen.size(); i++) {
            if (this->repairmen[i].getUsername() == username && this->repairmen[i].checkPassword(password)) {
                return &(this->repairmen[i]);
            }
        }

        return nullptr;
    }

    void saveEverything() const {
        ofstream write("salesman.txt");

        if (!write) {
            throw "\"salesman.txt\" File Can't be Opened.\n";
        } write << introLines[1] << endl;

        for (int i = 0; i < this->salesmen.size(); i++) {
            write << this->salesmen[i].getID() << "|" << this->salesmen[i].getName() << "|"
            << this->salesmen[i].getEmail() << "|" << this->salesmen[i].getPhone() << "|"
            << this->salesmen[i].getCNIC() << "|" << this->salesmen[i].getHireDate() << "|"
            << this->salesmen[i].getSalary() << "|" << this->salesmen[i].getSalesCount() << "|"
            << this->salesmen[i].getUsername() << "|" << this->salesmen[i].getPasscode() << endl;
        } write.close();

        write.open("customer.txt");

        if (!write) {
            throw "\"customer.txt\" File Can't be Opened.\n";
        } write << introLines[2] << endl;

        for (int i = 0; i < this->customers.size(); i++) {
            write << this->customers[i].getID() << "|" << this->customers[i].getName() << "|"
            << this->customers[i].getEmail() << "|" << this->customers[i].getPhone() << "|"
            << this->customers[i].getCNIC() << "|" << this->customers[i].getAddress() << "|"
            << this->customers[i].get_Date() << "|"
            << this->customers[i].getUsername() << "|" << this->customers[i].getPasscode() << endl;
        } write.close();

        write.open("repairman.txt");

        if (!write) {
            throw "\"repairman.txt\" File Can't be Opened.\n";
        } write << introLines[3] << endl;

        for (int i = 0; i < this->repairmen.size(); i++) {
            write << this->repairmen[i].getID() << "|" << this->repairmen[i].getName() << "|"
            << this->repairmen[i].getEmail() << "|" << this->repairmen[i].getPhone() << "|"
            << this->repairmen[i].getCNIC() << "|" << this->repairmen[i].getHireDate() << "|"
            << this->repairmen[i].getSalary() << "|" << this->repairmen[i].getSpecialization() << "|"
            << this->repairmen[i].getUsername() << "|" << this->repairmen[i].getPasscode() << endl;
        } write.close();   
    }
};

int main() {
    srand(time(0));
    InitWindow(SW, SH, "Showroom Management System");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    gFont = LoadFontEx("fonts/Roboto-Regular.ttf", 256, nullptr, 250);
    SetTextureFilter(gFont.texture, TEXTURE_FILTER_BILINEAR);
    if (gFont.texture.id == 0) gFont = GetFontDefault();

    ShowroomSystem sys;
    sys.run();         
    UnloadFont(gFont);
    CloseWindow();
    return 0;
}