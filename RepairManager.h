#ifndef REPAIRMANAGER_H
#define REPAIRMANAGER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "RepairRecord.h"
#include "Part.h"

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
    return num_of_lines;
    
}

class RepairManager {
    vector<RepairRecord> repairs;
    vector<Part> partPriceList;
    vector <string> intro_lines;

public:
    void loadAll();

    void saveAll() const;

    double getPartCost(const string &part_name) const;

    string requestRepair(const string &customerID, const string &plateNumber,
    const string &vehicleType, const string &partChanged, string &datee);

    vector<RepairRecord> getHistoryByCustomer(const string &customer_ID) const;

    vector<RepairRecord> getJobsByRepairMan(const string &Repair_ManID) const;

    vector<RepairRecord> getPendingJobs() const;

    bool assignRepairMan(const string &__repair_ID, const string &repairManID);

    bool updateStatus(const string &repair_iD, const string &newSts);
};

#endif