#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "Person.h"
#include <string>
using namespace std;

class Employee : public Person {
protected:
    string cnic;
    string hireDate;
    double salary;
 
public:
    Employee ();

    Employee(string _id, string _name, string _email, string _phone,
    string _username, string _password, string _cnic,
    string _hireDate, double _salary);

    Employee (const Employee &other);

    string getCNIC() const;

    string getHireDate() const;

    double getSalary() const;

    void setSalary(double nw_sal);    
};

#endif