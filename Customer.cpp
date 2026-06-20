#include "Customer.h"

Customer :: Customer () : Person () {
    this->cnic = "NULL";
    this->address = "NULL";
    this->registration_date = "NULL";
}

Customer :: Customer(string _id, string _name, string _email, string _phone,
string _username, string _password, string _cnic,
string _address, string _registration_date) : Person (_id,_name,_email,_phone,_username,_password) {
    this->cnic = _cnic;
    this->address = _address;
    this->registration_date = _registration_date;
} 

Customer :: Customer (const Customer &other) : Person (other) {
    this->cnic = other.cnic;
    this->address = other.address;
    this->registration_date = other.registration_date;
}

void Customer :: showMenu() {
    cout << "                            |〣--------------------------|〣 Customer Service Menu 〣|--------------------------〣|\n\n";
    cout << "1. Browse Stock Cars.\n";
    cout << "2. Browse Stock Bikes.\n";
    cout << "3. Browse Stock Scooties.\n";
    cout << "4. Browse All Stock Vehicles.\n";
    cout << "5. View My Purchase History.\n";
    cout << "6. View My Repair History.\n";
    cout << "7. Request Vehicle Repair.\n";
    cout << "8. Exit.\n";
}

string Customer :: getRole() const {
    return "Customer";
}

string Customer :: getPasscode () const {
    return this->password;
}

string Customer :: get_Date () const {
    return this->registration_date;
}

void Customer :: disp_Info () const {
    cout << "                            |〣--------------------------|〣 Customer Service Menu 〣|--------------------------〣|\n\n";
    cout << "〣------------------------------------------------------------------"
    "-----------------------------------------------------------------------〣\n";
    cout << "〣           Customer_ID: " << this->id << " ║ Name: " << this->name << " ║ CNIC: " << this->cnic
    << " ║ Registration_Date: " << this->registration_date << " ║ Phone: " << this->phone;
    cout << "\n〣------------------------------------------------------------------"
    "-----------------------------------------------------------------------〣\n";
}

string Customer :: getCNIC() const {
    return this->cnic;
}
string Customer :: getAddress() const {
    return this->address;
}

void Customer :: browse_cars(Inventory &inv) const {
    cout << "                            |〣--------------------------|〣 Customer Service Menu 〣|--------------------------〣|\n\n";
    
    inv.Changeable_disp("Car");
}

void Customer :: browse_bikes(Inventory &inv) const {
    cout << "                            |〣--------------------------|〣 Customer Service Menu 〣|--------------------------〣|\n\n";
    
    inv.Changeable_disp("Bike");
}

void Customer :: browse_scooties(Inventory &inv) const {
    cout << "                            |〣--------------------------|〣 Customer Service Menu 〣|--------------------------〣|\n\n";
    
    inv.Changeable_disp("Scooty");
}

void Customer :: browse_vehicles(Inventory &inv) const {
    cout << "                            |〣--------------------------|〣 Customer Service Menu 〣|--------------------------〣|\n\n";
    
    inv.Changeable_disp();
}

void Customer :: viewMyPurchases(SalesManager &sm) const {
    cout << "                            |〣--------------------------|〣 Customer Service Menu 〣|--------------------------〣|\n\n";
 
    vector <SoldVehicle> vect = sm.get_Purchases_By_Customer (this->id);
    if (vect.size() == 0) {
        cout << "No Purchases Yet.\n";
    } else {
        for (int i = 0; i < vect.size(); i++) {
            vect[i].dispSoldVec();
        }
    }
}

void Customer :: viewMyRepairHistory(RepairManager &rm) const {
    cout << "                            |〣--------------------------|〣 Customer Service Menu 〣|--------------------------〣|\n\n";
 
    vector <RepairRecord> vect = rm.getHistoryByCustomer (this->id);
    if (vect.size() == 0) {
        cout << "No Purchases Yet.\n";
    } else {
        for (int i = 0; i < vect.size(); i++) {
            vect[i].Disp_RepairRecord();
        }
    }
}

void Customer :: requestVehicleRepair(RepairManager &rm, const string &plateNumber,
const string &vehicleType, const string &partChanged) {

    time_t now = time(nullptr);
    tm* local_time = localtime(&now);
    stringstream ss;
    ss << put_time(local_time, "%d-%m-%Y");
    string date_str = ss.str();

    rm.requestRepair(this->id, plateNumber, vehicleType, partChanged,date_str);
}

Customer* Customer :: getCustomer_ById (string __id) {
    string temp_id, temp_name, temp_email, temp_phone, temp_username, temp_password;
    string temp_cnic, temp_address, temp_registration_date;
    string temp;

    int num = number_of_models ("customer.txt");

    ifstream read ("customer.txt");
    if (!read) {
        throw "\"customer.txt\" File Can't be Opened.\n";
    } getline (read, temp, '\n');

    for (int i = 0; i < num; i++) {
        getline (read, temp_id, '|');
        getline (read, temp_name, '|');
        getline (read, temp_email, '|');
        getline (read, temp_phone, '|');
        getline (read, temp_cnic, '|');
        getline (read, temp_address, '|');
        getline (read, temp_registration_date, '|');
        getline (read, temp_username, '|');
        getline (read, temp_password, '\n');

        if (temp_id == __id) {
            Customer* to_send = new Customer (temp_id, temp_name, temp_email, temp_phone, temp_username,
            temp_password, temp_cnic, temp_address, temp_registration_date);

            return to_send;
        }
    }

    throw "No Customer of Such ID.\n";
    return nullptr;
}