#include "RepairManager.h"

void RepairManager::loadAll() {
    string temp_repair_Id, temp_Customer_Id, temp_Plate_Number, temp_vehicle_type, temp_Part_Changed;
    string temp_RepairManID, temp_status, temp_requestDate, temp_name_of_part;
    double temp_cost___chunna, temp_cost_of_part;
    string temp;

    int num = number_of_models("Repair_history.txt");
    ifstream read ("Repair_history.txt");
    if (!read) {
        throw "\"Repair_history.txt\" File Can't be Opened.\n";
    }   getline (read, temp, '\n');

    this->intro_lines.push_back (temp);

    for (int i = 0; i < num; i++) {
        getline (read, temp_repair_Id, '|');
        getline (read, temp_Customer_Id, '|');
        getline (read, temp_Plate_Number, '|');
        getline (read, temp_vehicle_type, '|');
        getline (read, temp_Part_Changed, '|');
        read >> temp_cost___chunna;
        read.ignore();
        getline (read, temp_RepairManID, '|');
        getline (read, temp_status, '|');
        getline (read, temp_requestDate, '\n');

        RepairRecord temp_rec (temp_repair_Id, temp_Customer_Id, temp_Plate_Number, temp_vehicle_type,
        temp_Part_Changed, temp_cost___chunna, temp_RepairManID, temp_status, temp_requestDate);

        this->repairs.push_back (temp_rec);
    } read.close();

    num = number_of_models("Price_list_of_Parts.txt");
    read.open ("Price_list_of_Parts.txt");
    if (!read) {
        throw "\"Price_list_of_Parts.txt\" File Can't be Opened.\n";
    }   getline (read, temp, '\n');

    this->intro_lines.push_back (temp);

    for (int i = 0; i < num; i++) {
        getline (read, temp_name_of_part, '|');
        read >> temp_cost_of_part;
        read.ignore();

        Part nw_part (temp_name_of_part, temp_cost_of_part);

        this->partPriceList.push_back (nw_part);
    } read.close();

}

void RepairManager::saveAll() const {
    ofstream write ("Repair_history.txt");
    if (!write) {
        throw "\"Repair_history.txt\" File Can't be Opened.\n";
    }

    write << this->intro_lines[0] << endl;
    for (int i = 0; i < this->repairs.size(); i++) {
        write << this->repairs[i].get_RepairID() << "|" << this->repairs[i].get_CustomerID() << "|" 
        << this->repairs[i].get_PlateNumber() << "|" << this->repairs[i].get_VehicleType() << "|"
        << this->repairs[i].get_PartChanged() << "|" << this->repairs[i].get_Cost() << "|" 
        << this->repairs[i].get_RepairManID() << "|" << this->repairs[i].get_Status() << "|"
        << this->repairs[i].get_RequestDate() << endl;
    } write.close();

    write.open ("Price_list_of_Parts.txt");
    if (!write) {
        throw "\"Price_list_of_Parts.txt\" File Can't be Opened.\n";
    }

    write << this->intro_lines[1] << endl;
    for (int i = 0; i < this->partPriceList.size(); i++) {
        write << this->partPriceList[i].get_Part() << "|" << this->partPriceList[i].get_Cost() << endl;
    } write.close();
}

double RepairManager::getPartCost(const string &part_name) const {
    for (int i = 0; i < this->partPriceList.size(); i++) {
        if (this->partPriceList[i].get_Part() == part_name) {
            return this->partPriceList[i].get_Cost();
        }
    }

    throw "No Such Part Exists.\n";
    return 0;
}

string RepairManager::requestRepair(const string &customerID, const string &plateNumber,
const string &vehicleType, const string &partChanged, string &datee) {
    double temp_cost = getPartCost (partChanged);
    string nw_id, prev_id;
    if (this->repairs.size() == 0) {
        nw_id = "RP001";
    } else {
        prev_id = this->repairs[this->repairs.size() - 1].get_RepairID();
        char ch = prev_id.back() + 1;
        nw_id = "RP00" + string (1,ch);
    }

    RepairRecord nw_rec (nw_id, customerID, plateNumber, vehicleType, partChanged, temp_cost, "NULL", "Pending", datee);
    this->repairs.push_back(nw_rec);
    return nw_id;
}

vector<RepairRecord> RepairManager::getHistoryByCustomer(const string &customer_ID) const {
    vector <RepairRecord> hist_to_deliver;
    for (int i = 0; i < this->repairs.size(); i++) {
        if (this->repairs[i].get_CustomerID() == customer_ID) {
            hist_to_deliver.push_back (this->repairs[i]);
        }
    }

    return hist_to_deliver;
}

vector<RepairRecord> RepairManager::getJobsByRepairMan(const string &Repair_ManID) const {
    vector <RepairRecord> hist_to_deliver;
    for (int i = 0; i < this->repairs.size(); i++) {
        if (this->repairs[i].get_RepairManID() == Repair_ManID) {
            hist_to_deliver.push_back (this->repairs[i]);
        }
    }

    return hist_to_deliver;
}

vector<RepairRecord> RepairManager::getPendingJobs() const {
    vector <RepairRecord> temp_pending_stuff;
    for (int i = 0; i < this->repairs.size(); i++) {
        if (this->repairs[i].get_Status() == "Pending") {
            temp_pending_stuff.push_back (this->repairs[i]);
        }
    }

    return temp_pending_stuff;
}

bool RepairManager::assignRepairMan(const string &__repair_ID, const string &repairManID) {
    for (int i = 0; i < this->repairs.size(); i++) {
        if (this->repairs[i].get_RepairID() == __repair_ID) {
            if (this->repairs[i].get_Status() == "Completed") {
                throw "This Repair is Already Done.\n";
                return 0;
            } else {
                this->repairs[i].set_RepairManID (repairManID);
                return 1;
            }
        }
    }

    throw "No Such Repair ID.\n";
    return 0;
}

bool RepairManager::updateStatus(const string &repair_iD, const string &newSts) {
    for (int i = 0; i < this->repairs.size(); i++) {
        if (this->repairs[i].get_RepairID() == repair_iD) {
            this->repairs[i].set_Status (newSts);
            return 1;
        }
    }

    throw "Can't Update Status.\n";
    return 0;
}