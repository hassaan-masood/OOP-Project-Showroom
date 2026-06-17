#include "Part.h"

Part::Part() {
    name_of_part = "NULL";
    cost_of_part = 0;
}

Part::Part(string _name, double _cost) {
    this->name_of_part = _name;
    this->cost_of_part = _cost;
}

string Part::get_Part () const {
    return this->name_of_part;
}

double Part::get_Cost () const {
    return this->cost_of_part;
}

void Part::change_Part_Name (string nw_name) {
    this->name_of_part = nw_name;
}

void Part::change_Cost (double nw_cost) {
    this->cost_of_part = nw_cost;
}