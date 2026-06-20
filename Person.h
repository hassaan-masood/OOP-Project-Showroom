#ifndef PERSON_H
#define PERSON_H

#include <string>
using namespace std;

class Person {
protected:
    string id;
    string name;
    string email;
    string phone;
    string username;
    string password;

public:
    Person ();

    Person(string _id, string _name, string _email, string _phone,
    string _username, string _password);

    Person (const Person &other);
 
    virtual ~Person() {}

    string getID() const;

    string getName() const;

    string getEmail() const;

    string getPhone() const;

    string getUsername() const;
 
    bool checkPassword(const string &inp_pass) const;
 
    virtual void showMenu() = 0;
    virtual string getRole() const = 0;
    virtual void disp_Info () const = 0;
};

#endif