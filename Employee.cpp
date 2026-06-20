#include "Employee.h"

Employee :: Employee () : Person () {
    this->cnic = "NULL";
    this->hireDate = "NULL";
    this->salary = 0;
}

Employee :: Employee(string _id, string _name, string _email, string _phone,
string _username, string _password, string _cnic,
string _hireDate, double _salary) : Person (_id,_name,_email,_phone,_username,_password) {
    this->cnic = _cnic;
    this->hireDate = _hireDate;
    this->salary = _salary;
}

Employee :: Employee (const Employee &other) : Person (other) {
    this->cnic = other.cnic;
    this->hireDate = other.hireDate;
    this->salary = other.salary;
}

string Employee :: getCNIC() const {
    return this->cnic;
}

string Employee :: getHireDate() const {
    return this->hireDate;
}

double Employee :: getSalary() const {
    return this->salary;
}

void Employee :: setSalary(double nw_sal) {
    if (nw_sal > 0) {
        this->salary = nw_sal;
    } else {
        throw "Can't Update Salary.\n";
    }
}