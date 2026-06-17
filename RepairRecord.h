#ifndef REPAIRRECORD_H
#define REPAIRRECORD_H

#include <string>
#include <iostream>

using namespace std;

class RepairRecord {
    string repair_Id;
    string Customer_Id;
    string Plate_Number;
    string vehicle_type;
    string Part_Changed;
    double cost___chunna; 
    string RepairManID;
    string status;
    string requestDate;
 
public:
    RepairRecord();

    RepairRecord(string _repair_Id, string _Customer_Id, string _Plate_Number,
    string _vehicle_type, string _Part_Changed, double _cost___chunna,
    string _repairManID, string _status, string _requestDate);

    RepairRecord (const RepairRecord& other);

    string get_RepairID() const;

    string get_CustomerID() const;

    string get_PlateNumber() const;

    string get_VehicleType() const;

    string get_PartChanged() const;

    double get_Cost() const;

    string get_RepairManID() const;

    string get_Status() const;

    string get_RequestDate() const;
 
    void set_Status(const string &nw_sts);

    void set_RepairManID(const string &id);

    void Disp_RepairRecord ();
};

#endif