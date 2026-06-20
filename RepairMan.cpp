#include "RepairMan.h"

RepairMan :: RepairMan () : Employee () {
    this->specialization = "NULL";
}

RepairMan :: RepairMan(string _id, string _name, string _email, string _phone,
string _username, string _password, string _cnic,
string _hireDate, double _Salary, string  _specialization) : Employee 
(_id,_name,_email,_phone,_username,_password,_cnic,_hireDate,_Salary) {
    this->specialization = _specialization;
} 

RepairMan :: RepairMan (const RepairMan &other) : Employee (other) {
    this->specialization = other.specialization;
}

void RepairMan :: showMenu() {
    cout << "                            |〣--------------------------|〣 RepairMan Service Menu 〣|--------------------------〣|\n\n";
    cout << "1. View Pending Jobs.\n";
    cout << "2. Accept Pending Jobs.\n";
    cout << "3. Mark Job Status.\n";
    cout << "4. View My Profile.\n";
    cout << "5. Exit.\n";
}

string RepairMan :: getRole() const {
    return "RepairMan";
}

string RepairMan :: getSpecialization() const {
    return this->specialization;
}

string RepairMan :: getPasscode () const {
    return this->password;
}

void RepairMan :: disp_Info () const {
    cout << "                            |〣--------------------------|〣 Repairman Service Menu 〣|--------------------------〣|\n\n";
    cout << "〣------------------------------------------------------------------"
    "-----------------------------------------------------------------------〣\n";
    cout << "〣           RepairMan_ID: " << this->id << " ║ Name: " << this->name << " ║ CNIC: " << this->cnic
    << " ║ Salary: " << this->salary << " ║ Phone: " << this->phone;
    cout << "\n〣------------------------------------------------------------------"
    "-----------------------------------------------------------------------〣\n";
}

void RepairMan :: viewPendingJobs(RepairManager &rm) const {
    vector<RepairRecord> hist = rm.getPendingJobs();
    if (hist.size() == 0) {
        cout << "No Pending Jobs Right Now.\n";
    } else {
        for (int i = 0 ; i < hist.size(); i++) {
            hist[i].Disp_RepairRecord();
        }
    }
}

void RepairMan :: acceptJob(RepairManager &rm, const string &repairID) {
    rm.assignRepairMan(repairID, this->id);
    rm.updateStatus(repairID, "In Progress");
    rm.saveAll();
}

void RepairMan :: markJobCompleted(RepairManager &rm, const string &repairID) {
    rm.updateStatus(repairID, "Completed");
    rm.saveAll();
}
    
RepairMan* RepairMan :: get_RepairmanById (string __id) {
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