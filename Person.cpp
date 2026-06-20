#include "Person.h"

Person :: Person () {
    this->id = "NULL";
    this->name = "NULL";
    this->email = "NULL";
    this->phone = "NULL";
    this->username = "NULL";
    this->password = "NULL";
}

Person :: Person(string _id, string _name, string _email, string _phone,
string _username, string _password) {
    this->id = _id;
    this->name = _name;
    this->email = _email;
    this->phone = _phone;
    this->username = _username;
    this->password = _password;
}

Person :: Person (const Person &other) {
    this->id = other.id;
    this->name = other.name;
    this->email = other.email;
    this->phone = other.phone;
    this->username = other.username;
    this->password = other.password;
}

string Person :: getID() const {
    return this->id;
}

string Person :: getName() const {
    return this->name;
}

string Person :: getEmail() const {
    return this->email;
}

string Person :: getPhone() const {
    return this->phone;
}

string Person :: getUsername() const {
    return this->username;
}

bool Person :: checkPassword(const string &inp_pass) const {
    if (this->password == inp_pass) {
        return true;
    } return false;
}