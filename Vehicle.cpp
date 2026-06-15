#include "Vehicle.h"

// Constructors

Vehicle :: Vehicle () {
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

Vehicle :: Vehicle (string _model_id,string _brand,string _model_name,int _year
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

Vehicle :: Vehicle (const Vehicle& other) {
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

Vehicle :: ~Vehicle() {}

// Getters.

string Vehicle :: get_model_id () const {
    return this->model_id;
}

string Vehicle :: get_brand () const {
    return this->brand;
}

string Vehicle :: get_model_name () const {
    return this->model_name;
}

int Vehicle :: get_year () const {
    return this->year;
}

string Vehicle :: get_color () const {
    return this->color;
}

double Vehicle :: get_price () const {
    return this->price;
}

int Vehicle :: get_quantity () const {
    return this->quantity;
}

bool Vehicle :: get_is_electric () const {
    return this->is_electic;
}

int Vehicle :: get_engine_cc () const {
    return this->engine_cc;
}

int Vehicle :: get_mileage () const {
    return this->mileage_kmph;
}

string Vehicle :: get_fuel_type () const {
    return this->fuel_type;
}

void Vehicle :: setPrice(double newPrice) {
    this->price = newPrice;
}

void Vehicle :: setQuantity(int q) {
    this->quantity = q;
}

void Vehicle :: reduceQuantity(int amount) {
    if (this->quantity-amount < 0) {
        throw "Not Enough Stock.\n";
    } else {
        this->quantity -= amount;
    }
}

void Vehicle :: increaseQuantity(int amount) {
    this->quantity += amount;
}