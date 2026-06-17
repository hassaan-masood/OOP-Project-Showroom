#include "RepairRecord.h"

RepairRecord::RepairRecord() {
    this->repair_Id = "NULL";
    this->Customer_Id = "NULL";
    this->Plate_Number = "NULL";
    this->vehicle_type = "NULL";
    this->Part_Changed = "NULL";
    this->cost___chunna = 0;
    this->RepairManID = "NULL";
    this->status = "NULL";
    this->requestDate = "NULL";
}

RepairRecord::RepairRecord(string _repair_Id, string _Customer_Id, string _Plate_Number,
string _vehicle_type, string _Part_Changed, double _cost___chunna,
string _repairManID, string _status, string _requestDate) {
    this->repair_Id = _repair_Id;
    this->Customer_Id = _Customer_Id;
    this->Plate_Number = _Plate_Number;
    this->vehicle_type = _vehicle_type;
    this->Part_Changed = _Part_Changed;
    this->cost___chunna = _cost___chunna;
    this->RepairManID = _repairManID;
    this->status = _status;
    this->requestDate = _requestDate;
}

RepairRecord::RepairRecord (const RepairRecord& other) {
    this->repair_Id = other.repair_Id;
    this->Customer_Id = other.Customer_Id;
    this->Plate_Number = other.Plate_Number;
    this->vehicle_type = other.vehicle_type;
    this->Part_Changed = other.Part_Changed;
    this->cost___chunna = other.cost___chunna;
    this->RepairManID = other.RepairManID;
    this->status = other.status;
    this->requestDate = other.requestDate;
}

string RepairRecord::get_RepairID() const {
    return this->repair_Id;
}

string RepairRecord::get_CustomerID() const {
    return this->Customer_Id;
}

string RepairRecord::get_PlateNumber() const {
    return this->Plate_Number;
}

string RepairRecord::get_VehicleType() const {
    return this->vehicle_type;
}

string RepairRecord::get_PartChanged() const {
    return this->Part_Changed;
}

double RepairRecord::get_Cost() const {
    return this->cost___chunna;
}

string RepairRecord::get_RepairManID() const {
    return this->RepairManID;
}

string RepairRecord::get_Status() const {
    return this->status;
}

string RepairRecord::get_RequestDate() const {
    return this->requestDate;
}

void RepairRecord::set_Status(const string &nw_sts) {
    this->status = nw_sts;
}

void RepairRecord::set_RepairManID(const string &id) {
    this->RepairManID = id;
}

void RepairRecord::Disp_RepairRecord () {
    cout << "〣---------------------------------------------------------------------"
    "-----------------------------------------------------------------------〣\n";
    cout << "〣               Repair_ID:" << this->repair_Id << " ║ Customer_ID:" << this->Customer_Id <<
    " ║ Plate_Number:" << this->Plate_Number  << " ║ Vehicle_Type:" << this->vehicle_type <<
    " ║ Part:" << this->Part_Changed << endl << "〣                              Cost:" <<
    this->cost___chunna << " ║ Repair_ManID:" << this->RepairManID << " ║ Status:" << this->status <<
    " ║ Date:" << this->requestDate; 
    cout << "\n〣---------------------------------------------------------------------"
    "-----------------------------------------------------------------------〣\n";
}