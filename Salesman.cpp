#include "Salesman.h"

Salesman :: Salesman () : Employee () {
    this->sales_ct = 0;
}

Salesman :: Salesman(string _id, string _name, string _email, string _phone,
string _username, string _password, string _cnic,
string _hireDate, double _Salary, int _ct) : Employee 
(_id,_name,_email,_phone,_username,_password,_cnic,_hireDate,_Salary) {
    this->sales_ct = _ct;
} 

Salesman :: Salesman (const Salesman &other) : Employee (other) {
    this->sales_ct = other.sales_ct;
}

void Salesman :: showMenu() {
    cout << "                            |〣--------------------------|〣 Salesman Service Menu 〣|--------------------------〣|\n\n";
    cout << "1. Sale Any Vehicle.\n";
    cout << "2. View My Profile.\n";
    cout << "3. View My Sales History.\n";
    cout << "4. Exit.\n";
}

string Salesman :: getRole() const {
    return "Salesman";
} 

void Salesman :: disp_Info () const {
    cout << "                            |〣--------------------------|〣 Salesman Service Menu 〣|--------------------------〣|\n\n";
    cout << "〣------------------------------------------------------------------"
    "-----------------------------------------------------------------------〣\n";
    cout << "〣           Salesman_ID: " << this->id << " ║ Name: " << this->name << " ║ CNIC: " << this->cnic
    << " ║ Salary: " << this->salary << " ║ Phone: " << this->phone;
    cout << "\n〣------------------------------------------------------------------"
    "-----------------------------------------------------------------------〣\n";
}

int Salesman :: getSalesCount() const {
    return this->sales_ct;
}

string Salesman :: getPasscode () const {
    return this->password;
}

void Salesman :: incrementSalesCount() {
    this->sales_ct++;
}

void Salesman :: sellVehicle(Inventory &inv, Customer &cust, SalesManager &sm,
const string &modelID) {
    Vehicle* ptr = inv.findByModelID(modelID);
    int qunt = ptr->get_quantity();
    qunt --;
    inv.updateQuantity(modelID, qunt);
    string number_plt = sm.generate_PlateNumber();

    time_t now = time(nullptr);
    tm* local_time = localtime(&now);
    stringstream ss;
    ss << put_time(local_time, "%d-%m-%Y");
    string date_str = ss.str();

    int num = number_of_models ("sales.txt");
    num += 1;
    string s1 = to_string(num);
    string slr_id = "SL00" + s1;

    SoldVehicle veh (number_plt, modelID, ptr->getType(), ptr->get_brand(), ptr->get_model_name(), cust.getID(), this->id, date_str, ptr->get_price());
    SaleRecord rec (slr_id, modelID, ptr->getType(), cust.getID(), this->id, date_str, ptr->get_price());

    sm.record_Sale(rec,veh);
    sm.saveAll();
    inv.saveAll();
}

void Salesman :: viewMyProfile() const {
    cout << "                            |〣--------------------------|〣 Salesman Service Menu 〣|--------------------------〣|\n\n";
    cout << "〣------------------------------------------------------------------"
    "-----------------------------------------------------------------------〣\n";
    cout << "〣           Salesman_ID: " << this->id << " ║ Name: " << this->name << " ║ CNIC: " << this->cnic
    << " ║ Salary: " << this->salary << " ║ Phone: " << this->phone;
    cout << "\n〣------------------------------------------------------------------"
    "-----------------------------------------------------------------------〣\n";   
}

void Salesman :: viewMySalesHistory(SalesManager &sm) const {
    vector <SaleRecord> slr = sm.get_Sales_By_Salesman(this->id);
    if (slr.size() == 0) {
        cout << "You Have No Sales Yet.\n";
    } else {
        for (int i = 0; i < slr.size(); i++) {
            slr[i].dispRec();
        }
    }
}

Salesman* Salesman :: getSalesman_ById (string __id) {
    string temp_id, temp_name, temp_email, temp_phone, temp_username, temp_password;
    string temp_cnic, temp_hireDate;
    int temp_sale_ctt;
    double temp_salary;
    string temp;

    int num = number_of_models ("salesman.txt");

    ifstream read ("salesman.txt");
    if (!read) {
        throw "\"salesman.txt\" File Can't be Opened.\n";
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
        read >> temp_sale_ctt;
        read.ignore();
        getline (read, temp_username, '|');
        getline (read, temp_password, '\n');

        if (temp_id == __id) {
            Salesman* to_send = new Salesman (temp_id, temp_name, temp_email, temp_phone, temp_username,
            temp_password, temp_cnic, temp_hireDate, temp_salary, temp_sale_ctt);

            return to_send;
        }
    }

    throw "No Salesman of Such ID.\n";
    return nullptr;
}