#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
using namespace std;

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

    Vehicle ();

    Vehicle (string _model_id,string _brand,string _model_name,int _year
    ,string _color,double _price,int _quantity,bool _is_electric,
    int _engine_cc, int _mileage_kmph,string _fuel_type);

    Vehicle (const Vehicle& other);

    // Destructor

    virtual ~Vehicle();

    // Getters.

    string get_model_id () const;

    string get_brand () const;

    string get_model_name () const;

    int get_year () const;

    string get_color () const;

    double get_price () const;

    int get_quantity () const;

    bool get_is_electric () const;

    int get_engine_cc () const;

    int get_mileage () const;

    string get_fuel_type () const;

    void setPrice(double newPrice);

    void setQuantity(int q);

    void reduceQuantity(int amount);

    void increaseQuantity(int amount);

    virtual void displayInfo() const = 0;

    virtual string getType () = 0;
};

#endif