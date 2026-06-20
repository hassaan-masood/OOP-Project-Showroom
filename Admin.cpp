#include "Admin.h"
#include "Inventory.h"
#include "SalesManager.h"
#include "RepairManager.h"
#include "Employee.h"
#include "Car.h"
#include "Bike.h"
#include "Scooty.h"
#include "Salesman.h"
#include "RepairMan.h"
#include "Customer.h"
#include "RepairRecord.h"

Admin :: Admin () {
    this->id = "NULL";
    this->name = "NULL";
    this->username = "NULL";
    this->password = "NULL";
}

Admin :: Admin(string _id, string _name, string _username, string _password) {
    this->id = _id;
    this->name = _name;
    this->username = _username;
    this->password = _password;
}

void Admin :: showMenu() {
    cout << "                            |〣--------------------------|〣 Admin Control Panel 〣|--------------------------〣|\n\n";
    cout << "1.  Add New Car Model.\n";
    cout << "2.  Add New Bike Model.\n";
    cout << "3.  Add New Scooty Model.\n";
    cout << "4.  Remove Vehicle Model.\n";
    cout << "5.  Update Vehicle Stock.\n";
    cout << "6.  View All Vehicles.\n";
    cout << "7.  Add New Customer.\n";
    cout << "8.  Remove Customer.\n";
    cout << "9.  Add New Salesman.\n";
    cout << "10. Remove Salesman.\n";
    cout << "11. Add New RepairMan.\n";
    cout << "12. Remove RepairMan.\n";
    cout << "13. Update Employee Salary.\n";
    cout << "14. View All Customers.\n";
    cout << "15. View All Salesmen.\n";
    cout << "16. View All RepairMen.\n";
    cout << "17. View Pending Repairs.\n";
    cout << "18. Assign RepairMan to Job.\n";
    cout << "19. View Sales Report.\n";
    cout << "20. Exit.\n";
}

string Admin :: getRole() const {
    return "Admin";
}  

string Admin :: getUsername() const { 
    return this->username; 
}

bool Admin :: checkPassword(const string &inp) const {
    return this->password == inp; 
}

void Admin :: addCar(Inventory &inv) {
    string temp_model_id, temp_brand, temp_model_name, temp_color, temp_fuel_type, temp_transmission;
    int temp_year, temp_quantity, temp_engine_cc, temp_mileage_kmph, temp_seatingCapacity, temp_numDoors;
    double temp_price;
    bool temp_is_electic;

    cout << "Enter Car's ID: ";
    cin >> temp_model_id;
    cout << "Enter Car's Brand: ";
    cin >> temp_brand;
    cout << "Enter Car's Name: ";
    cin.ignore();
    getline(cin, temp_model_name);
    cout << "Enter Car's Release Year: ";
    cin >> temp_year;
    cout << "Enter Car's Color: ";
    cin.ignore();
    getline(cin, temp_color);
    cout << "Enter Car's Price: ";
    cin >> temp_price;
    cout << "Enter Car's Quantity: ";
    cin >> temp_quantity;
    cout << "Is Car Electric? (0 or 1): ";\
    cin >> temp_is_electic;
    cout << "Enter Car's Engine CC: ";
    cin >> temp_engine_cc;
    cout << "Enter Car's Mileage (KMPH): ";
    cin >> temp_mileage_kmph;
    cout << "Enter Car's Fuel Type: ";
    cin >> temp_fuel_type;
    cout << "Enter Car's Seating Capacity: ";
    cin >> temp_seatingCapacity;
    cout << "Enter Car's Transmisstion (Automatic/Manual): ";
    cin >> temp_transmission;
    cout << "Enter Car's Number of Doors: ";
    cin >> temp_numDoors;

    Car nw_Car (temp_model_id, temp_brand, temp_model_name, temp_year, temp_color, temp_price,
    temp_quantity, temp_is_electic, temp_engine_cc, temp_mileage_kmph, temp_fuel_type, temp_seatingCapacity,
    temp_transmission, temp_numDoors);

    inv.addCar(nw_Car);
    inv.saveAll();
}   

void Admin :: addBike(Inventory &inv) {
    string temp_model_id, temp_brand, temp_model_name, temp_color, temp_fuel_type, temp_bikeType;
    int temp_year, temp_quantity, temp_engine_cc, temp_mileage_kmph;
    double temp_price;
    bool temp_is_electic;

    cout << "Enter Bike's ID: ";
    cin >> temp_model_id;
    cout << "Enter Bike's Brand: ";
    cin >> temp_brand;
    cout << "Enter Bike's Name: ";
    cin.ignore();
    getline(cin, temp_model_name);
    cout << "Enter Bike's Release Year: ";
    cin >> temp_year;
    cout << "Enter Bike's Color: ";
    cin.ignore();
    getline(cin, temp_color);
    cout << "Enter Bike's Price: ";
    cin >> temp_price;
    cout << "Enter Bike's Quantity: ";
    cin >> temp_quantity;
    cout << "Is Bike Electric? (0 or 1): ";\
    cin >> temp_is_electic;
    cout << "Enter Bike's Engine CC: ";
    cin >> temp_engine_cc;
    cout << "Enter Bike's Mileage (KMPH): ";
    cin >> temp_mileage_kmph;
    cout << "Enter Bike's Fuel Type: ";
    cin >> temp_fuel_type;
    cout << "Enter Bike's Type (Standard/Sports): ";
    cin >> temp_bikeType;

    Bike nw_bike (temp_model_id, temp_brand, temp_model_name, temp_year, temp_color, temp_price,
    temp_quantity, temp_is_electic, temp_engine_cc, temp_mileage_kmph, temp_fuel_type, temp_bikeType);

    inv.addBike(nw_bike);
    inv.saveAll();
} 

void Admin :: addScooty(Inventory &inv) {
    string temp_model_id, temp_brand, temp_model_name, temp_color, temp_fuel_type;
    int temp_year, temp_quantity, temp_engine_cc, temp_mileage_kmph;
    double temp_price, temp_stCap;
    bool temp_is_electic;

    cout << "Enter Scooty's ID: ";
    cin >> temp_model_id;
    cout << "Enter Scooty's Brand: ";
    cin >> temp_brand;
    cout << "Enter Scooty's Name: ";
    cin.ignore();
    getline(cin, temp_model_name);
    cout << "Enter Scooty's Release Year: ";
    cin >> temp_year;
    cout << "Enter Scooty's Color: ";
    cin.ignore();
    getline(cin, temp_color);
    cout << "Enter Scooty's Price: ";
    cin >> temp_price;
    cout << "Enter Scooty's Quantity: ";
    cin >> temp_quantity;
    cout << "Is Scooty Electric? (0 or 1): ";\
    cin >> temp_is_electic;
    cout << "Enter Scooty's Engine CC: ";
    cin >> temp_engine_cc;
    cout << "Enter Scooty's Mileage (KMPH): ";
    cin >> temp_mileage_kmph;
    cout << "Enter Scooty's Fuel Type: ";
    cin >> temp_fuel_type;
    cout << "Enter Scooty's Storage Capacity (Litres): ";
    cin >> temp_stCap;

    Scooty nw_scooty (temp_model_id, temp_brand, temp_model_name, temp_year, temp_color, temp_price,
    temp_quantity, temp_is_electic, temp_engine_cc, temp_mileage_kmph, temp_fuel_type, temp_stCap);

    inv.addScooty(nw_scooty);
    inv.saveAll();
} 

void Admin :: removeVehicleModel(Inventory &inv, const string &modelID) {
    inv.removeModel (modelID);
    inv.saveAll();
}

void Admin :: updateStock(Inventory &inv, const string &modelID, int newQuantity) {
    inv.updateQuantity(modelID, newQuantity);
}
 
void Admin :: addSalesman(vector<Salesman> &salesmen) {
    string id, name, email, phone, username, password, cnic;
    double salary;

    time_t now = time(nullptr);
    tm* local_time = localtime(&now);
    stringstream ss;
    ss << put_time(local_time, "%d-%m-%Y");
    string date_str = ss.str();

    cout << "Enter ID: ";
    cin >> id;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Email: ";
    cin >> email;
    cout << "Enter Phone: ";
    cin >> phone;
    cout << "Enter CNIC: ";
    cin >> cnic;
    cout << "Enter Salary: ";
    cin >> salary;
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    Salesman nw_sm (id, name, email, phone, username, password, cnic, date_str, salary, 0);
    salesmen.push_back(nw_sm);
}

void Admin :: removeSalesman(vector<Salesman> &salesmen, const string &salesmanID) {
    for (int i = 0; i < salesmen.size(); i++) {
        if (salesmen[i].getID() == salesmanID) {
            salesmen.erase(salesmen.begin() + i);
            return;
        }
    }

    throw "Salesman Not Found.\n";
    return;
}

void Admin :: addRepairMan(vector<RepairMan> &repairmen) {
    string id, name, email, phone, username, password, cnic, special;
    double salary;

    time_t now = time(nullptr);
    tm* local_time = localtime(&now);
    stringstream ss;
    ss << put_time(local_time, "%d-%m-%Y");
    string date_str = ss.str();

    cout << "Enter ID: ";
    cin >> id;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Email: ";
    cin >> email;
    cout << "Enter Phone: ";
    cin >> phone;
    cout << "Enter CNIC: ";
    cin >> cnic;
    cout << "Enter Salary: ";
    cin >> salary;
    cout << "Enter Specialization: ";
    cin >> special;
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    RepairMan nw_rm (id, name, email, phone, username, password, cnic, date_str, salary, special);
    repairmen.push_back(nw_rm);
}

void Admin :: removeRepairMan(vector<RepairMan> &repairmen, const string &repairManID) {
    for (int i = 0; i < repairmen.size(); i++) {
        if (repairmen[i].getID() == repairManID) {
            repairmen.erase(repairmen.begin() + i);
            return;
        }
    }

    throw "Repairman Not Found.\n";
    return;
}

void Admin :: paySalary(Employee &emp, double amount) {
    return;
}
 
void Admin :: addCustomer(vector<Customer> &customers) {
    string id, name, email, phone, username, password, cnic, address;

    time_t now = time(nullptr);
    tm* local_time = localtime(&now);
    stringstream ss;
    ss << put_time(local_time, "%d-%m-%Y");
    string date_str = ss.str();

    cout << "Enter Customer ID: ";
    cin >> id;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Email: ";
    cin >> email;
    cout << "Enter Phone: ";
    cin >> phone;
    cout << "Enter CNIC: ";
    cin >> cnic;
    cout << "Enter Address: ";
    cin.ignore();
    getline(cin, address);
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    Customer nw_cust (id, name, email, phone, username, password, cnic, address, date_str);
    customers.push_back(nw_cust);
}

void Admin :: removeCustomer(vector<Customer> &customers, const string &customerID) {
    for (int i = 0; i < customers.size(); i++) {
        if (customers[i].getID() == customerID) {
            customers.erase(customers.begin() + i);
            return;
        }
    }

    throw "Customer Not Found.\n";
}
 
void Admin :: viewSalesReport(SalesManager &sm) const {
    cout << "                            |〣--------------------------|〣 Sales Report 〣|--------------------------〣|\n\n";
    cout << "〣------------------------------------------------------------------"
    "-----------------------------〣\n";
    cout << "〣      Total Sales: " << sm.get_Total_Sales_Count() << " ║ Total Revenue: " << sm.get_Total_Revenue();
    cout << "\n〣------------------------------------------------------------------"
    "-----------------------------〣\n";
}

void Admin :: viewAllVehicles(Inventory &inv) const {
    cout << "                            |〣--------------------------|〣 All Vehicles 〣|--------------------------〣|\n\n";
    inv.Changeable_disp();
}

void Admin :: viewAllSalesmen(const vector<Salesman> &salesmen) const {
    cout << "                            |〣--------------------------|〣 All Salesmen 〣|--------------------------〣|\n\n";
    if (salesmen.size() == 0) {
        cout << "No Salesmen On Record.\n";
        return;
    }
    for (int i = 0; i < salesmen.size(); i++) {
        salesmen[i].disp_Info();
    }
}

void Admin :: viewAllRepairmen(const vector<RepairMan> &repairmen) const {
    cout << "                            |〣--------------------------|〣 All Repairmen 〣|--------------------------〣|\n\n";
    if (repairmen.size() == 0) {
        cout << "No Repairmen On Record.\n";
        return;
    }
    for (int i = 0; i < repairmen.size(); i++) {
        repairmen[i].disp_Info();
    }
}

void Admin :: viewAllCustomers(const vector<Customer> &customers) const {
    cout << "                            |〣--------------------------|〣 All Customers 〣|--------------------------〣|\n\n";
    if (customers.size() == 0) {
        cout << "No Customers On Record.\n";
        return;
    }
    for (int i = 0; i < customers.size(); i++) {
        customers[i].disp_Info();
    }
}

void Admin :: viewPendingRepairs(RepairManager &rm) const {
    cout << "                            |〣--------------------------|〣 Pending Repairs 〣|--------------------------〣|\n\n";
    vector<RepairRecord> rem = rm.getPendingJobs();
    if (rem.size() == 0) {
        cout << "No Pending Repairs Right Now.\n";
        return;
    }
    for (int i = 0; i < rem.size(); i++) {
        rem[i].Disp_RepairRecord();
    }
}
 
Admin* Admin :: getAdmin () {
    string temp, id, name, username, passcode_;

    ifstream read ("admin.txt");
    if (!read) {
        throw "\"repairman.txt\" File Can't be Opened.\n";
    } getline (read, temp, '\n');

    getline (read,id,'|');
    getline (read,name,'|');
    getline (read,username,'|');
    getline (read,passcode_,'\n');

    read.close();

    Admin* deliver = new Admin (id,name,username,passcode_);
    return deliver;
}