#ifndef PART_H
#define PART_H

#include <string>

using namespace std;

class Part {
    string name_of_part;
    double cost_of_part;

public:
    Part();

    Part(string _name, double _cost);

    string get_Part () const;

    double get_Cost () const;

    void change_Part_Name (string nw_name);

    void change_Cost (double nw_cost);
};

#endif