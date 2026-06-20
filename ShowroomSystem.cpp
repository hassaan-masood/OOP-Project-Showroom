#include "ShowroomSystem.h"

void ShowroomSystem::loadEverything() {
        string temp_id, temp_name, temp_email, temp_phone, temp_username, temp_password;
        string temp_cnic, temp_hireDate, temp_address, temp_registration_date, temp_specialization;
        int temp_sales_ct;
        double temp_salary;
        string temp;

        this->inventory.loadAll();
        this->salesManager.loadAll();
        this->repairManager.loadAll();

        int num = number_of_models("admin.txt");

        ifstream read("admin.txt");
        if (!read) {
            throw "\"admin.txt\" File Can't be Opened.\n";
        } getline(read, temp, '\n');

        introLines.push_back (temp);

        for (int i = 0; i < num; i++) {
            string a_id, a_name, a_username, a_password;
            getline(read, a_id, '|');
            getline(read, a_name, '|');
            getline(read, a_username, '|');
            getline(read, a_password, '\n');

            Admin nw_admin(a_id, a_name, a_username, a_password);
            this->admins.push_back(nw_admin);
        } read.close();

        num = number_of_models("salesman.txt");

        read.open("salesman.txt");
        if (!read) {
            throw "\"salesman.txt\" File Can't be Opened.\n";
        } getline(read, temp, '\n');

        introLines.push_back (temp);

        for (int i = 0; i < num; i++) {
            getline(read, temp_id, '|');
            getline(read, temp_name, '|');
            getline(read, temp_email, '|');
            getline(read, temp_phone, '|');
            getline(read, temp_cnic, '|');
            getline(read, temp_hireDate, '|');
            read >> temp_salary;
            read.ignore();
            read >> temp_sales_ct;
            read.ignore();
            getline(read, temp_username, '|');
            getline(read, temp_password, '\n');

            Salesman nw_sm(temp_id, temp_name, temp_email, temp_phone, temp_username,
            temp_password, temp_cnic, temp_hireDate, temp_salary, temp_sales_ct);
            this->salesmen.push_back(nw_sm);
        } read.close();

        num = number_of_models("customer.txt");

        read.open("customer.txt");
        if (!read) {
            throw "\"customer.txt\" File Can't be Opened.\n";
        } getline(read, temp, '\n');

        introLines.push_back(temp);

        for (int i = 0; i < num; i++) {
            getline(read, temp_id, '|');
            getline(read, temp_name, '|');
            getline(read, temp_email, '|');
            getline(read, temp_phone, '|');
            getline(read, temp_cnic, '|');
            getline(read, temp_address, '|');
            getline(read, temp_registration_date, '|');
            getline(read, temp_username, '|');
            getline(read, temp_password, '\n');

            Customer nw_cust(temp_id, temp_name, temp_email, temp_phone, temp_username,
            temp_password, temp_cnic, temp_address, temp_registration_date);
            this->customers.push_back(nw_cust);
        } read.close();

        num = number_of_models("repairman.txt");

        read.open("repairman.txt");
        if (!read) {
            throw "\"repairman.txt\" File Can't be Opened.\n";
        } getline(read, temp, '\n');

        introLines.push_back(temp);

        for (int i = 0; i < num; i++) {
            getline(read, temp_id, '|');
            getline(read, temp_name, '|');
            getline(read, temp_email, '|');
            getline(read, temp_phone, '|');
            getline(read, temp_cnic, '|');
            getline(read, temp_hireDate, '|');
            read >> temp_salary;
            read.ignore();
            getline(read, temp_specialization, '|');
            getline(read, temp_username, '|');
            getline(read, temp_password, '\n');

            RepairMan nw_rm(temp_id, temp_name, temp_email, temp_phone, temp_username,
            temp_password, temp_cnic, temp_hireDate, temp_salary, temp_specialization);
            this->repairmen.push_back(nw_rm);
        
        } read.close();
    }

void ShowroomSystem::run() {
        this->loadEverything();

        int choice;
        string inp_username, inp_password;

        while (true) {
            system("clear");
            cout << "                            |〣--------------------------|〣 Welcome to Showroom System 〣|--------------------------〣|\n\n";

            cout << "1. Login\n";
            cout << "2. Admin Login\n";
            cout << "3. Exit\n";
            cout << "Enter Choice: ";
            cin >> choice;

            if (choice == 3) {
                system("clear");
                cout << "Goodbye.\n";
                break;
            }

            cout << "Enter Username: ";
            cin >> inp_username;
            cout << "Enter Password: ";
            cin >> inp_password;

            if (choice == 2) {
                bool found = false;
                for (int i = 0; i < this->admins.size(); i++) {
                    if (this->admins[i].getUsername() == inp_username && this->admins[i].checkPassword(inp_password)) {
                        found = true;
                        int ad_choice;

                        while (true) {
                            system("clear");
                            this->admins[i].showMenu();
                            cout << "Enter Choice: ";
                            cin >> ad_choice;

                            if (ad_choice == 20) {
                                break;
                            } 
                            
                            string modelID, salesmanID, repairmanID, customerID, repairID;
                            int newQty;
                            double newSalary;

                            try {
                                if (ad_choice == 1) {
                                    system("clear");
                                    this->admins[i].addCar(this->inventory);
                                } else if (ad_choice == 2) {
                                    system("clear");
                                    this->admins[i].addBike(this->inventory);
                                } else if (ad_choice == 3) {
                                    system("clear");
                                    this->admins[i].addScooty(this->inventory);
                                } else if (ad_choice == 4) {
                                    system("clear");
                                    cout << "Enter Model ID to Remove: ";
                                    cin >> modelID;
                                    this->admins[i].removeVehicleModel(this->inventory, modelID);
                                    cout << "Model Removed Successfully.\n";
                                } else if (ad_choice == 5) {
                                    system("clear");
                                    cout << "Enter Model ID: ";
                                    cin >> modelID;
                                    cout << "Enter New Quantity: ";
                                    cin >> newQty;
                                    this->admins[i].updateStock(this->inventory, modelID, newQty);
                                    this->inventory.saveAll();
                                    cout << "Stock Updated Successfully.\n";
                                } else if (ad_choice == 6) {
                                    system("clear");
                                    this->admins[i].viewAllVehicles(this->inventory);
                                } else if (ad_choice == 7) {
                                    system("clear");
                                    this->admins[i].addCustomer(this->customers);
                                    this->saveEverything();
                                    cout << "Customer Added Successfully.\n";
                                } else if (ad_choice == 8) {
                                    system("clear");
                                    cout << "Enter Customer ID to Remove: ";
                                    cin >> customerID;
                                    this->admins[i].removeCustomer(this->customers, customerID);
                                    this->saveEverything();
                                    cout << "Customer Removed Successfully.\n";
                                } else if (ad_choice == 9) {
                                    system("clear");
                                    this->admins[i].addSalesman(this->salesmen);
                                    this->saveEverything();
                                    cout << "Salesman Added Successfully.\n";
                                } else if (ad_choice == 10) {
                                    system("clear");
                                    cout << "Enter Salesman ID to Remove: ";
                                    cin >> salesmanID;
                                    this->admins[i].removeSalesman(this->salesmen, salesmanID);
                                    this->saveEverything();
                                    cout << "Salesman Removed Successfully.\n";
                                } else if (ad_choice == 11) {
                                    system("clear");
                                    this->admins[i].addRepairMan(this->repairmen);
                                    this->saveEverything();
                                    cout << "RepairMan Added Successfully.\n";
                                } else if (ad_choice == 12) {
                                    system("clear");
                                    cout << "Enter RepairMan ID to Remove: ";
                                    cin >> repairmanID;
                                    this->admins[i].removeRepairMan(this->repairmen, repairmanID);
                                    this->saveEverything();
                                    cout << "RepairMan Removed Successfully.\n";
                                } else if (ad_choice == 13) {
                                    system("clear");
                                    string emp_id, emp_type;
                                    cout << "Enter Employee Type (Salesman/RepairMan): ";
                                    cin >> emp_type;
                                    cout << "Enter Employee ID: ";
                                    cin >> emp_id;
                                    cout << "Enter New Salary: ";
                                    cin >> newSalary;
                                    if (emp_type == "Salesman") {
                                        bool emp_found = false;
                                        for (int j = 0; j < this->salesmen.size(); j++) {
                                            if (this->salesmen[j].getID() == emp_id) {
                                                this->admins[i].paySalary(this->salesmen[j], newSalary);
                                                this->saveEverything();
                                                emp_found = true;
                                                break;
                                            }
                                        }
                                        if (!emp_found) cout << "Salesman Not Found.\n";
                                    } else if (emp_type == "RepairMan") {
                                        bool emp_found = false;
                                        for (int j = 0; j < this->repairmen.size(); j++) {
                                            if (this->repairmen[j].getID() == emp_id) {
                                                this->admins[i].paySalary(this->repairmen[j], newSalary);
                                                this->saveEverything();
                                                emp_found = true;
                                                break;
                                            }
                                        }
                                        if (!emp_found) cout << "RepairMan Not Found.\n";
                                    } else {
                                        cout << "Invalid Employee Type.\n";
                                    }
                                } else if (ad_choice == 14) {
                                    system("clear");
                                    this->admins[i].viewAllCustomers(this->customers);
                                } else if (ad_choice == 15) {
                                    system("clear");
                                    this->admins[i].viewAllSalesmen(this->salesmen);
                                } else if (ad_choice == 16) {
                                    system("clear");
                                    this->admins[i].viewAllRepairmen(this->repairmen);
                                } else if (ad_choice == 17) {
                                    system("clear");
                                    this->admins[i].viewPendingRepairs(this->repairManager);
                                } else if (ad_choice == 18) {
                                    system("clear");
                                    cout << "Enter Repair ID: ";
                                    cin >> repairID;
                                    cout << "Enter RepairMan ID: ";
                                    cin >> repairmanID;
                                    this->repairManager.assignRepairMan(repairID, repairmanID);
                                    this->repairManager.saveAll();
                                    cout << "RepairMan Assigned Successfully.\n";
                                } else if (ad_choice == 19) {
                                    system("clear");
                                    this->admins[i].viewSalesReport(this->salesManager);
                                } else {
                                    cout << "Invalid Choice.\n";
                                }
                            } catch (const char* err) {
                                cout << "Error: " << err;
                            }

                            cout << "\nPress Enter to Continue...";
                            cin.ignore();
                            cin.get();
                        }
                        break;
                    }
                }

                if (!found) {
                    cout << "Invalid Admin Credentials.\n";
                    cout << "\nPress Enter to Continue...";
                    cin.ignore();
                    cin.get();
                }

            } else if (choice == 1) {
                Person* logged_in = this->authenticateLogin(inp_username, inp_password);

                if (logged_in == nullptr) {
                    cout << "Invalid Credentials.\n";
                    cout << "\nPress Enter to Continue...";
                    cin.ignore();
                    cin.get();
                    continue;
                }

                string role = logged_in->getRole();
                int usr_choice;

                if (role == "Customer") {
                    Customer* cust = (Customer*)logged_in;
                    while (true) {
                        system("clear");
                        cust->showMenu();
                        cout << "Enter Choice: ";
                        cin >> usr_choice;

                        if (usr_choice == 8) break;

                        try {
                            if (usr_choice == 1) {
                                system("clear");
                                cust->browse_cars(this->inventory);
                            } else if (usr_choice == 2) {
                                system("clear");
                                cust->browse_bikes(this->inventory);
                            } else if (usr_choice == 3) {
                                system("clear");
                                cust->browse_scooties(this->inventory);
                            } else if (usr_choice == 4) {
                                system("clear");
                                cust->browse_vehicles(this->inventory);
                            } else if (usr_choice == 5) {
                                system("clear");
                                cust->viewMyPurchases(this->salesManager);
                            } else if (usr_choice == 6) {
                                system("clear");
                                cust->viewMyRepairHistory(this->repairManager);
                            } else if (usr_choice == 7) {
                                system("clear");
                                string plateNum, vehType, partName;
                                cout << "Enter Your Vehicle Plate Number: ";
                                cin >> plateNum;
                                cout << "Enter Vehicle Type (Car/Bike/Scooty): ";
                                cin >> vehType;
                                cout << "Enter Part to Repair: ";
                                cin.ignore();
                                getline(cin, partName);
                                cust->requestVehicleRepair(this->repairManager, plateNum, vehType, partName);
                                this->repairManager.saveAll();
                                cout << "Repair Request Submitted Successfully.\n";
                            } else {
                                cout << "Invalid Choice.\n";
                            }
                        } catch (const char* err) {
                            cout << "Error: " << err;
                        }

                        cout << "\nPress Enter to Continue...";
                        cin.ignore();
                        cin.get();
                    }

                } else if (role == "Salesman") {
                    Salesman* sm = (Salesman*)logged_in;
                    while (true) {
                        system("clear");
                        sm->showMenu();
                        cout << "Enter Choice: ";
                        cin >> usr_choice;

                        if (usr_choice == 5) {
                            break;
                        }

                        try {
                            if (usr_choice == 1) {
                                system("clear");
                                string modelID, custID;
                                cout << "Enter Model ID to Sell: ";
                                cin >> modelID;
                                cout << "Enter Customer ID: ";
                                cin >> custID;
                                Customer* cust = Customer::getCustomer_ById(custID);
                                sm->sellVehicle(this->inventory, *cust, this->salesManager, modelID);
                                sm->incrementSalesCount();
                                this->saveEverything();
                                delete cust;
                                cout << "Vehicle Sold Successfully.\n";
                            } else if (usr_choice == 2) {
                                system("clear");
                                sm->viewMyProfile();
                            } else if (usr_choice == 3) {
                                system("clear");
                                sm->viewMySalesHistory(this->salesManager);
                            } else {
                                cout << "Invalid Choice.\n";
                            }
                        } catch (const char* err) {
                            cout << "Error: " << err;
                        }

                        cout << "\nPress Enter to Continue...";
                        cin.ignore();
                        cin.get();
                    }

                } else if (role == "RepairMan") {
                    RepairMan* rm = (RepairMan*)logged_in;
                    while (true) {
                        system("clear");
                        rm->showMenu();
                        cout << "Enter Choice: ";
                        cin >> usr_choice;

                        if (usr_choice == 4) break;

                        try {
                            if (usr_choice == 1) {
                                system("clear");
                                rm->viewPendingJobs(this->repairManager);
                            } else if (usr_choice == 2) {
                                system("clear");
                                string repairID;
                                cout << "Enter Repair ID to Accept: ";
                                cin >> repairID;
                                rm->acceptJob(this->repairManager, repairID);
                                cout << "Job Accepted Successfully.\n";
                            } else if (usr_choice == 3) {
                                system("clear");
                                string repairID;
                                cout << "Enter Repair ID to Mark Completed: ";
                                cin >> repairID;
                                rm->markJobCompleted(this->repairManager, repairID);
                                cout << "Job Marked as Completed.\n";
                            } else {
                                cout << "Invalid Choice.\n";
                            }
                        } catch (const char* err) {
                            cout << "Error: " << err;
                        }

                        cout << "\nPress Enter to Continue...";
                        cin.ignore();
                        cin.get();
                    }
                }

            } else {
                cout << "Invalid Choice.\n";
                cout << "\nPress Enter to Continue...";
                cin.ignore();
                cin.get();
            }
        }
    }
 
Person* ShowroomSystem::authenticateLogin(const string &username, const string &password) {
        for (int i = 0; i < this->salesmen.size(); i++) {
            if (this->salesmen[i].getUsername() == username && this->salesmen[i].checkPassword(password)) {
                return &(this->salesmen[i]);
            }
        }
        for (int i = 0; i < this->customers.size(); i++) {
            if (this->customers[i].getUsername() == username && this->customers[i].checkPassword(password)) {
                return &(this->customers[i]);
            }
        }
        for (int i = 0; i < this->repairmen.size(); i++) {
            if (this->repairmen[i].getUsername() == username && this->repairmen[i].checkPassword(password)) {
                return &(this->repairmen[i]);
            }
        }

        return nullptr;
    }

void ShowroomSystem::saveEverything() const {
        ofstream write("salesman.txt");

        if (!write) {
            throw "\"salesman.txt\" File Can't be Opened.\n";
        } write << introLines[1] << endl;

        for (int i = 0; i < this->salesmen.size(); i++) {
            write << this->salesmen[i].getID() << "|" << this->salesmen[i].getName() << "|"
            << this->salesmen[i].getEmail() << "|" << this->salesmen[i].getPhone() << "|"
            << this->salesmen[i].getCNIC() << "|" << this->salesmen[i].getHireDate() << "|"
            << this->salesmen[i].getSalary() << "|" << this->salesmen[i].getSalesCount() << "|"
            << this->salesmen[i].getUsername() << "|" << this->salesmen[i].getPasscode() << endl;
        } write.close();

        write.open("customer.txt");

        if (!write) {
            throw "\"customer.txt\" File Can't be Opened.\n";
        } write << introLines[2] << endl;

        for (int i = 0; i < this->customers.size(); i++) {
            write << this->customers[i].getID() << "|" << this->customers[i].getName() << "|"
            << this->customers[i].getEmail() << "|" << this->customers[i].getPhone() << "|"
            << this->customers[i].getCNIC() << "|" << this->customers[i].getAddress() << "|"
            << this->customers[i].get_Date() << "|"
            << this->customers[i].getUsername() << "|" << this->customers[i].getPasscode() << endl;
        } write.close();

        write.open("repairman.txt");

        if (!write) {
            throw "\"repairman.txt\" File Can't be Opened.\n";
        } write << introLines[3] << endl;

        for (int i = 0; i < this->repairmen.size(); i++) {
            write << this->repairmen[i].getID() << "|" << this->repairmen[i].getName() << "|"
            << this->repairmen[i].getEmail() << "|" << this->repairmen[i].getPhone() << "|"
            << this->repairmen[i].getCNIC() << "|" << this->repairmen[i].getHireDate() << "|"
            << this->repairmen[i].getSalary() << "|" << this->repairmen[i].getSpecialization() << "|"
            << this->repairmen[i].getUsername() << "|" << this->repairmen[i].getPasscode() << endl;
        } write.close();   
    }