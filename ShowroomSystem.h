#ifndef SHOWROOMSYSTEM_H
#define SHOWROOMSYSTEM_H

#include "Inventory.h"
#include "SalesManager.h"
#include "RepairManager.h"
#include "Admin.h"
#include "Salesman.h"
#include "Customer.h"
#include "RepairMan.h"

class ShowroomSystem {
    Inventory inventory;
    SalesManager salesManager;
    RepairManager repairManager;
 
    vector<Admin> admins;
    vector<Salesman> salesmen;
    vector<Customer> customers;
    vector<RepairMan> repairmen;
    vector <string> introLines;
 
public:
    void loadEverything();

    void run();
 
    Person* authenticateLogin(const string &username, const string &password);

    void saveEverything() const;
};

#endif