#ifndef REPAIRMAN_H
#define REPAIRMAN_H

#include "Employee.h"
#include "RepairManager.h"
#include "RepairRecord.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

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

class RepairMan : public Employee {
    string specialization;

public:
    RepairMan ();

    RepairMan(string _id, string _name, string _email, string _phone,
    string _username, string _password, string _cnic,
    string _hireDate, double _Salary, string  _specialization);

    RepairMan (const RepairMan &other);

    void showMenu() override;
    
    string getRole() const override;
 
    string getSpecialization() const;

    string getPasscode () const;

    void disp_Info () const override;
 
    void viewPendingJobs(RepairManager &rm) const;

    void acceptJob(RepairManager &rm, const string &repairID);

    void markJobCompleted(RepairManager &rm, const string &repairID);
        
    static RepairMan* get_RepairmanById (string __id);
};

#endif