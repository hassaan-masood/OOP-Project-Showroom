#include "ShowroomSystem.h"

#include <iostream>
#include <unistd.h>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "raylib.h"
#include <string>
#include <vector>
#include <functional>

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
    if (num_of_lines < 0) {
        return 0;
    } 

    return num_of_lines;
}

const int SW = 3000, SH = 1800;
const Color BG      = { 15,  15,  20, 255 };
const Color PANEL   = { 28,  28,  38, 255 };
const Color ACCENT  = { 82, 130, 255, 255 };
const Color ACCENT2 = {255, 100,  80, 255 };
const Color TXT     = {220, 220, 230, 255 };
const Color TXTSUB  = {140, 140, 160, 255 };
const Color BTN     = { 45,  45,  62, 255 };
const Color BTNHOV  = { 60,  60,  85, 255 };
const Color BTNACT  = { 82, 130, 255, 255 };
const Color FIELD   = { 35,  35,  50, 255 };
const Color FIELDFOC= { 45,  45,  68, 255 };
const Color ERR     = {220,  60,  60, 255 };
const Color SUCCESS = { 60, 200, 120, 255 };

Font gFont;

struct InputBox {
    Rectangle rect;
    std::string label;
    std::string value;
    bool password = false;
    bool focused  = false;
    int  maxLen   = 64;

    void draw() const {
        Color bg = focused ? FIELDFOC : FIELD;
        DrawRectangleRec(rect, bg);
        DrawRectangleLinesEx(rect, focused ? 2.0f : 1.0f,
                             focused ? ACCENT : TXTSUB);
        DrawTextEx(gFont, label.c_str(),
        {rect.x, rect.y - 32}, 26, 1, TXTSUB);

        std::string disp = password
            ? std::string(value.size(), '*') : value;

        DrawTextEx(gFont, disp.c_str(),
        {rect.x + 12, rect.y + 14}, 30, 1, TXT);

        if (focused && (int)(GetTime() * 2) % 2 == 0) {
            float cx = rect.x + 12 +
            MeasureTextEx(gFont, disp.c_str(), 30, 1).x;
            DrawLineEx({cx, rect.y + 8}, {cx, rect.y + rect.height - 8},
            2, ACCENT);
        }
    }

    bool handleKey() {
        if (!focused) return false;
        int ch = GetCharPressed();
        bool changed = false;
        while (ch > 0) {
            if (ch >= 32 && (int)value.size() < maxLen) {
                value += (char)ch;
                changed = true;
            }
            ch = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !value.empty()) {
            value.pop_back();
            changed = true;
        }
        return changed;
    }

    bool hitTest(Vector2 mp) const {
        return CheckCollisionPointRec(mp, rect);
    }
};

struct Button {
    Rectangle rect;
    std::string label;
    Color col     = BTN;
    Color colHov  = BTNHOV;
    Color colAct  = BTNACT;
    Color textCol = TXT;
    float fontSize = 36;

    bool draw() const {
        Vector2 mp = GetMousePosition();
        bool hov  = CheckCollisionPointRec(mp, rect);
        bool down = hov && IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        bool clk  = hov && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

        Color bg = down ? colAct : (hov ? colHov : col);
        DrawRectangleRec(rect, bg);
        DrawRectangleLinesEx(rect, hov ? 2.0f : 1.0f,
        hov ? ACCENT : TXTSUB);

        Vector2 ts = MeasureTextEx(gFont, label.c_str(), fontSize, 1);
        DrawTextEx(gFont, label.c_str(),
            { rect.x + (rect.width  - ts.x) / 2,
            rect.y + (rect.height - ts.y) / 2 },
            fontSize, 1, textCol);
        return clk;
    }
};

static string gNotifMsg;
static double      gNotifTime = -99;
static bool        gNotifErr  = false;

void showNotif(const std::string& msg, bool isErr = false) {
    gNotifMsg  = msg;
    gNotifTime = GetTime();
    gNotifErr  = isErr;
}

void drawNotif() {
    if (GetTime() - gNotifTime > 2.5) return;
    Color c = gNotifErr ? ERR : SUCCESS;
    DrawRectangle(0, SH - 64, SW, 64, { c.r, c.g, c.b, 200 });
    Vector2 ts = MeasureTextEx(gFont, gNotifMsg.c_str(), 24, 1);
    DrawTextEx(gFont, gNotifMsg.c_str(),
    { (SW - ts.x) / 2, SH - 46 }, 24, 1, WHITE);
}

void drawInfoList(const std::vector<std::string>& lines,
    Rectangle area, int& scrollY) {
    BeginScissorMode((int)area.x,(int)area.y,
    (int)area.width,(int)area.height);
    float y = area.y - scrollY;
    for (auto& l : lines) {
        DrawTextEx(gFont, l.c_str(), {area.x + 8, y}, 26, 1, TXT);
        y += 36;
    }
    EndScissorMode();
    float wh = GetMouseWheelMove();
    if (wh != 0 && CheckCollisionPointRec(GetMousePosition(), area))
    scrollY = std::max(0, scrollY - (int)(wh * 30));
}

bool drawFormModal(const std::string& title,
    std::vector<InputBox>& fields,
    const std::string& confirmLabel = "Confirm") {
    DrawRectangle(0, 0, SW, SH, {0,0,0,160});

    float mw = 780, mh = 140.0f + fields.size() * 100.0f + 80.0f;
    float mx = (SW - mw) / 2, my = (SH - mh) / 2;
    DrawRectangle((int)mx,(int)my,(int)mw,(int)mh, PANEL);
    DrawRectangleLinesEx({mx,my,mw,mh}, 2, ACCENT);

    DrawTextEx(gFont, title.c_str(), {mx+20, my+18}, 30, 1, ACCENT);

    for (int i = 0; i < (int)fields.size(); i++) {
        fields[i].rect = { mx+20, my + 80.0f + i*100.0f, mw-40, 64 };
    }

    Vector2 mp = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (auto& f : fields)
            f.focused = f.hitTest(mp);
    }
    for (auto& f : fields) { f.draw(); f.handleKey(); }

    Button bConfirm{{ mx+20,      my+mh-76, 280, 62 }, confirmLabel,
                    BTN, BTNHOV, {60,180,100,255}};
    Button bCancel {{ mx+mw-300,  my+mh-76, 280, 62 }, "Cancel",
    BTN, BTNHOV, ACCENT2};

    if (bConfirm.draw()) return true;
    if (bCancel.draw())  return false;  
    return false;
}

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

    enum Screen { MAIN, ADMIN_MENU, CUSTOMER_MENU,
                SALESMAN_MENU, REPAIRMAN_MENU };
    Screen screen = MAIN;

    bool loginOpen = false;
    bool adminLogin = false;
    std::vector<InputBox> loginFields = {
        {{}, "Username", "", false},
        {{}, "Password", "", true }
    };

    Person*   loggedPerson = nullptr;
    Admin*    loggedAdmin  = nullptr;

    std::vector<std::string> infoLines;
    int infoScroll = 0;
    bool showingInfo = false;

    bool      subModalOpen = false;
    std::string subModalTitle;
    std::vector<InputBox> subFields;
    std::function<void()> subConfirmCb;   

    auto openSub = [&](const std::string& title,
        std::vector<std::pair<std::string,bool>> fieldDefs,
        std::function<void()> cb) {
        subModalTitle = title;
        subFields.clear();
        for (auto& d : fieldDefs)
            subFields.push_back({{}, d.first, "", d.second});
        subConfirmCb = cb;
        subModalOpen = true;
        showingInfo  = false;
    };

    auto captureInventory = [&](const std::string& type) {
        infoLines.clear();
        infoScroll = 0;
        auto addVeh = [&](Vehicle* v) {
            infoLines.push_back(
                "ID:" + v->get_model_id() +
                "  Brand:" + v->get_brand() +
                "  Model:" + v->get_model_name() +
                "  Year:"  + std::to_string(v->get_year()) +
                "  Price:" + std::to_string((int)v->get_price()) +
                "  Qty:"   + std::to_string(v->get_quantity()) +
                "  Fuel:"  + v->get_fuel_type());
        };
        if (type == "Car" || type == "All")
            for (auto p : inventory.getCars())   addVeh(p);
        if (type == "Bike" || type == "All")
            for (auto p : inventory.getBikes())  addVeh(p);
        if (type == "Scooty" || type == "All")
            for (auto p : inventory.getScooties()) addVeh(p);
        showingInfo = true;
    };

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BG);

        if (screen == MAIN) {
            DrawTextEx(gFont, "Showroom Management System",
            {40, 50}, 52, 1, ACCENT);
            DrawTextEx(gFont, "Please log in to continue",
            {40, 125}, 30, 1, TXTSUB);

            Button bLogin    {{40, 240, 500, 90}, "Staff / Customer Login"};
            Button bAdmLogin {{40, 350, 500, 90}, "Admin Login"};
            Button bExit     {{40, 460, 500, 90}, "Exit", BTN, BTNHOV, ACCENT2};

            if (bLogin.draw())    { 
                adminLogin=false; loginOpen=true;
                loginFields[0].value=""; loginFields[1].value="";
            }
            if (bAdmLogin.draw()) { 
                adminLogin=true;  loginOpen=true;
                loginFields[0].value=""; loginFields[1].value=""; 
            }
            if (bExit.draw()) {
                EndDrawing();
                break; 
            }
            if (loginOpen) {
                bool confirmed = drawFormModal(
                    adminLogin ? "Admin Login" : "Staff / Customer Login",
                    loginFields, "Login");

                if (IsKeyPressed(KEY_ESCAPE)) {
                    loginOpen = false;
                }
                if (confirmed) {
                    loginOpen = false;
                    std::string uname = loginFields[0].value;
                    std::string pwd   = loginFields[1].value;

                    if (adminLogin) {
                        for (auto& a : admins) {
                            if (a.getUsername()==uname && a.checkPassword(pwd)) {
                                loggedAdmin = &a;
                                screen = ADMIN_MENU;
                                break;
                            }
                        }
                        if (!loggedAdmin) showNotif("Invalid admin credentials.", true);
                    } else {
                        loggedPerson = authenticateLogin(uname, pwd);
                        if (!loggedPerson) {
                            showNotif("Invalid credentials.", true);
                        } else {
                            std::string role = loggedPerson->getRole();
                            if (role == "Customer")  screen = CUSTOMER_MENU;
                            if (role == "Salesman")  screen = SALESMAN_MENU;
                            if (role == "RepairMan") screen = REPAIRMAN_MENU;
                        }
                    }
                }
            }
        }

        else if (screen == ADMIN_MENU) {
            DrawTextEx(gFont, "Admin Control Panel",
                    {40, 14}, 38, 1, ACCENT);
            Button bBack{{SW-240,12,220,66},"Logout",BTN,BTNHOV,ACCENT2};
            if (bBack.draw()) {
                loggedAdmin=nullptr; screen=MAIN;
                showingInfo=false; subModalOpen=false;
            }

            struct Item { std::string label; int id; };
            std::vector<Item> items = {
                {"Add Car",1},{"Add Bike",2},{"Add Scooty",3},
                {"Remove Vehicle",4},{"Update Stock",5},{"View All Vehicles",6},
                {"Add Customer",7},{"Remove Customer",8},
                {"Add Salesman",9},{"Remove Salesman",10},
                {"Add RepairMan",11},{"Remove RepairMan",12},
                {"Update Salary",13},
                {"View Customers",14},{"View Salesmen",15},{"View RepairMen",16},
                {"Pending Repairs",17},{"Assign RepairMan",18},
                {"Sales Report",19}
            };
            int cols=3; float bw=360,bh=76,pad=16;
            float startX=40, startY=96;
            for (int i=0;i<(int)items.size();i++){
                int c=i%cols, r=i/cols;
                Button b{{startX+c*(bw+pad), startY+r*(bh+pad), bw, bh},
                        items[i].label};
                if (b.draw() && !subModalOpen && !showingInfo) {
                    int id = items[i].id;
                    try {
                        if (id==1) openSub("Add Car",
                            {{"Model ID",false},{"Brand",false},{"Model Name",false},
                            {"Year",false},{"Color",false},{"Price",false},
                            {"Quantity",false},{"Electric (0/1)",false},
                            {"Engine CC",false},{"Mileage KMPH",false},
                            {"Fuel Type",false},{"Seating Capacity",false},
                            {"Transmission",false},{"Num Doors",false}},
                            [&](){
                                auto& f=subFields;
                                Car c(f[0].value,f[1].value,f[2].value,
                                    stoi(f[3].value),f[4].value,stod(f[5].value),
                                    stoi(f[6].value),(bool)stoi(f[7].value),
                                    stoi(f[8].value),stoi(f[9].value),f[10].value,
                                    stoi(f[11].value),f[12].value,stoi(f[13].value));
                                inventory.addCar(c); inventory.saveAll();
                                showNotif("Car added.");
                            });
                        else if (id==2) openSub("Add Bike",
                            {{"Model ID",false},{"Brand",false},{"Model Name",false},
                            {"Year",false},{"Color",false},{"Price",false},
                            {"Quantity",false},{"Electric (0/1)",false},
                            {"Engine CC",false},{"Mileage KMPH",false},
                            {"Fuel Type",false},{"Bike Type",false}},
                            [&](){
                                auto& f=subFields;
                                Bike b(f[0].value,f[1].value,f[2].value,
                                    stoi(f[3].value),f[4].value,stod(f[5].value),
                                    stoi(f[6].value),(bool)stoi(f[7].value),
                                    stoi(f[8].value),stoi(f[9].value),
                                    f[10].value,f[11].value);
                                inventory.addBike(b); inventory.saveAll();
                                showNotif("Bike added.");
                            });
                        else if (id==3) openSub("Add Scooty",
                            {{"Model ID",false},{"Brand",false},{"Model Name",false},
                            {"Year",false},{"Color",false},{"Price",false},
                            {"Quantity",false},{"Electric (0/1)",false},
                            {"Engine CC",false},{"Mileage KMPH",false},
                            {"Fuel Type",false},{"Storage Capacity L",false}},
                            [&](){
                                auto& f=subFields;
                                Scooty s(f[0].value,f[1].value,f[2].value,
                                    stoi(f[3].value),f[4].value,stod(f[5].value),
                                    stoi(f[6].value),(bool)stoi(f[7].value),
                                    stoi(f[8].value),stoi(f[9].value),
                                    f[10].value,stod(f[11].value));
                                inventory.addScooty(s); inventory.saveAll();
                                showNotif("Scooty added.");
                            });
                        else if (id==4) openSub("Remove Vehicle",
                            {{"Model ID",false}},
                            [&](){ inventory.removeModel(subFields[0].value);
                                inventory.saveAll(); showNotif("Removed."); });
                        else if (id==5) openSub("Update Stock",
                            {{"Model ID",false},{"New Quantity",false}},
                            [&](){ inventory.updateQuantity(subFields[0].value,
                                    stoi(subFields[1].value));
                                inventory.saveAll(); showNotif("Stock updated."); });
                        else if (id==6) { captureInventory("All"); }
                        else if (id==7) openSub("Add Customer",
                            {{"Customer ID",false},{"Name",false},{"Email",false},
                            {"Phone",false},{"CNIC",false},{"Address",false},
                            {"Username",false},{"Password",true}},
                            [&](){
                                auto& f=subFields;
                                time_t now=time(nullptr); tm* lt=localtime(&now);
                                std::stringstream ss; ss<<std::put_time(lt,"%d-%m-%Y");
                                Customer c(f[0].value,f[1].value,f[2].value,
                                    f[3].value,f[6].value,f[7].value,
                                    f[4].value,f[5].value,ss.str());
                                customers.push_back(c); saveEverything();
                                showNotif("Customer added.");
                            });
                        else if (id==8) openSub("Remove Customer",
                            {{"Customer ID",false}},
                            [&](){ loggedAdmin->removeCustomer(customers,subFields[0].value);
                                saveEverything(); showNotif("Customer removed."); });
                        else if (id==9) openSub("Add Salesman",
                            {{"ID",false},{"Name",false},{"Email",false},
                            {"Phone",false},{"CNIC",false},{"Salary",false},
                            {"Username",false},{"Password",true}},
                            [&](){
                                auto& f=subFields;
                                time_t now=time(nullptr); tm* lt=localtime(&now);
                                std::stringstream ss; ss<<std::put_time(lt,"%d-%m-%Y");
                                Salesman s(f[0].value,f[1].value,f[2].value,
                                    f[3].value,f[6].value,f[7].value,
                                    f[4].value,ss.str(),stod(f[5].value),0);
                                salesmen.push_back(s); saveEverything();
                                showNotif("Salesman added.");
                            });
                        else if (id==10) openSub("Remove Salesman",
                            {{"Salesman ID",false}},
                            [&](){ loggedAdmin->removeSalesman(salesmen,subFields[0].value);
                                saveEverything(); showNotif("Salesman removed."); });
                        else if (id==11) openSub("Add RepairMan",
                            {{"ID",false},{"Name",false},{"Email",false},
                            {"Phone",false},{"CNIC",false},{"Salary",false},
                            {"Specialization",false},{"Username",false},{"Password",true}},
                            [&](){
                                auto& f=subFields;
                                time_t now=time(nullptr); tm* lt=localtime(&now);
                                std::stringstream ss; ss<<std::put_time(lt,"%d-%m-%Y");
                                RepairMan r(f[0].value,f[1].value,f[2].value,
                                    f[3].value,f[7].value,f[8].value,
                                    f[4].value,ss.str(),stod(f[5].value),f[6].value);
                                repairmen.push_back(r); saveEverything();
                                showNotif("RepairMan added.");
                            });
                        else if (id==12) openSub("Remove RepairMan",
                            {{"RepairMan ID",false}},
                            [&](){ loggedAdmin->removeRepairMan(repairmen,subFields[0].value);
                                saveEverything(); showNotif("RepairMan removed."); });
                        else if (id==13) openSub("Update Salary",
                            {{"Employee Type (Salesman/RepairMan)",false},
                            {"Employee ID",false},{"New Salary",false}},
                            [&](){
                                std::string type=subFields[0].value;
                                std::string eid =subFields[1].value;
                                double sal=stod(subFields[2].value);
                                if(type=="Salesman"){
                                    for(auto& s:salesmen) if(s.getID()==eid){s.setSalary(sal);break;}
                                } else {
                                    for(auto& r:repairmen) if(r.getID()==eid){r.setSalary(sal);break;}
                                }
                                saveEverything(); showNotif("Salary updated.");
                            });
                        else if (id==14){
                            infoLines.clear(); infoScroll=0;
                            for(auto& c:customers)
                                infoLines.push_back(
                                    "ID:"+c.getID()+"  Name:"+c.getName()+
                                    "  Phone:"+c.getPhone()+"  CNIC:"+c.getCNIC());
                            showingInfo=true;
                        }
                        else if (id==15){
                            infoLines.clear(); infoScroll=0;
                            for(auto& s:salesmen)
                                infoLines.push_back(
                                    "ID:"+s.getID()+"  Name:"+s.getName()+
                                    "  Salary:"+std::to_string((int)s.getSalary())+
                                    "  Sales:"+std::to_string(s.getSalesCount()));
                            showingInfo=true;
                        }
                        else if (id==16){
                            infoLines.clear(); infoScroll=0;
                            for(auto& r:repairmen)
                                infoLines.push_back(
                                    "ID:"+r.getID()+"  Name:"+r.getName()+
                                    "  Spec:"+r.getSpecialization()+
                                    "  Salary:"+std::to_string((int)r.getSalary()));
                            showingInfo=true;
                        }
                        else if (id==17){
                            auto pend=repairManager.getPendingJobs();
                            infoLines.clear(); infoScroll=0;
                            for(auto& rr:pend)
                                infoLines.push_back(
                                    "RepID:"+rr.get_RepairID()+
                                    "  Plate:"+rr.get_PlateNumber()+
                                    "  Part:"+rr.get_PartChanged()+
                                    "  Cost:"+std::to_string((int)rr.get_Cost()));
                            if(infoLines.empty()) infoLines.push_back("No pending repairs.");
                            showingInfo=true;
                        }
                        else if (id==18) openSub("Assign RepairMan",
                            {{"Repair ID",false},{"RepairMan ID",false}},
                            [&](){ repairManager.assignRepairMan(
                                    subFields[0].value,subFields[1].value);
                                repairManager.saveAll();
                                showNotif("RepairMan assigned."); });
                        else if (id==19){
                            infoLines.clear(); infoScroll=0;
                            infoLines.push_back(
                                "Total Sales: "+std::to_string(salesManager.get_Total_Sales_Count()));
                            infoLines.push_back(
                                "Total Revenue: "+std::to_string((long long)salesManager.get_Total_Revenue()));
                            showingInfo=true;
                        }
                    } catch(const char* e){ showNotif(e,true); }
                }
            }

            if (showingInfo) {
                DrawRectangle(0,0,SW,SH,{0,0,0,160});
                Rectangle area={60,60,(float)SW-120,(float)SH-140};
                DrawRectangleRec(area,PANEL);
                DrawRectangleLinesEx(area,2,ACCENT);
                drawInfoList(infoLines,{area.x+8,area.y+8,area.width-16,area.height-56},infoScroll);
                Button bClose{{area.x+area.width-230,area.y+area.height-72,210,58},"Close"};
                if(bClose.draw()||IsKeyPressed(KEY_ESCAPE)) showingInfo=false;
            }

            if (subModalOpen) {
                bool confirmed = drawFormModal(subModalTitle, subFields);
                if (confirmed) {
                    try { subConfirmCb(); } catch(const char* e){ showNotif(e,true); }
                    subModalOpen=false;
                }
                if (IsKeyPressed(KEY_ESCAPE)) subModalOpen=false;
            }
        }

        else if (screen == CUSTOMER_MENU) {
            Customer* cust = (Customer*)loggedPerson;
            DrawTextEx(gFont,
                ("Welcome, " + cust->getName()).c_str(),
                {40,14},36,1,ACCENT);
            Button bBack{{SW-240,12,220,66},"Logout",BTN,BTNHOV,ACCENT2};
            if(bBack.draw()){
                loggedPerson=nullptr; screen=MAIN;
                showingInfo=false; subModalOpen=false;
            }

            struct Item{ std::string label; int id; };
            std::vector<Item> items={
                {"Browse Cars",1},{"Browse Bikes",2},{"Browse Scooties",3},
                {"Browse All",4},{"My Purchases",5},
                {"My Repair History",6},{"Request Repair",7}
            };
            float bw=380,bh=84,pad=16,sx=40,sy=104;
            int cols=3;
            for(int i=0;i<(int)items.size();i++){
                int c=i%cols,r=i/cols;
                Button b{{sx+c*(bw+pad),sy+r*(bh+pad),bw,bh},items[i].label};
                if(b.draw()&&!subModalOpen&&!showingInfo){
                    int id=items[i].id;
                    try{
                        if(id>=1&&id<=4){
                            std::string t=(id==1?"Car":id==2?"Bike":id==3?"Scooty":"All");
                            captureInventory(t);
                        } else if(id==5){
                            auto purch=salesManager.get_Purchases_By_Customer(cust->getID());
                            infoLines.clear(); infoScroll=0;
                            for(auto& sv:purch)
                                infoLines.push_back(
                                    "Plate:"+sv.getPlateNumber()+
                                    "  Model:"+sv.getModelID()+
                                    "  Brand:"+sv.getBrand()+
                                    "  Date:"+sv.getSaleDate()+
                                    "  Price:"+std::to_string((int)sv.getSalePrice()));
                            if(infoLines.empty()) infoLines.push_back("No purchases yet.");
                            showingInfo=true;
                        } else if(id==6){
                            auto hist=repairManager.getHistoryByCustomer(cust->getID());
                            infoLines.clear(); infoScroll=0;
                            for(auto& rr:hist)
                                infoLines.push_back(
                                    "RepID:"+rr.get_RepairID()+
                                    "  Plate:"+rr.get_PlateNumber()+
                                    "  Part:"+rr.get_PartChanged()+
                                    "  Status:"+rr.get_Status());
                            if(infoLines.empty()) infoLines.push_back("No repair history.");
                            showingInfo=true;
                        } else if(id==7){
                            openSub("Request Vehicle Repair",
                                {{"Plate Number",false},
                                {"Vehicle Type (Car/Bike/Scooty)",false},
                                {"Part to Repair",false}},
                                [&,cust](){
                                    cust->requestVehicleRepair(repairManager,
                                        subFields[0].value,subFields[1].value,
                                        subFields[2].value);
                                    repairManager.saveAll();
                                    showNotif("Repair request submitted.");
                                });
                        }
                    } catch(const char* e){ showNotif(e,true); }
                }
            }

            if(showingInfo){
                DrawRectangle(0,0,SW,SH,{0,0,0,160});
                Rectangle area={60,60,(float)SW-120,(float)SH-140};
                DrawRectangleRec(area,PANEL);
                DrawRectangleLinesEx(area,2,ACCENT);
                drawInfoList(infoLines,{area.x+8,area.y+8,area.width-16,area.height-56},infoScroll);
                Button bClose{{area.x+area.width-230,area.y+area.height-72,210,58},"Close"};
                if(bClose.draw()||IsKeyPressed(KEY_ESCAPE)) showingInfo=false;
            }
            if(subModalOpen){
                bool confirmed=drawFormModal(subModalTitle,subFields);
                if(confirmed){
                    try{ subConfirmCb(); } catch(const char* e){ showNotif(e,true); }
                    subModalOpen=false;
                }
                if(IsKeyPressed(KEY_ESCAPE)) subModalOpen=false;
            }
        }

        else if (screen == SALESMAN_MENU) {
            Salesman* sm = (Salesman*)loggedPerson;
            DrawTextEx(gFont,
                ("Salesman: " + sm->getName()).c_str(),
                {40,14},36,1,ACCENT);
            Button bBack{{SW-240,12,220,66},"Logout",BTN,BTNHOV,ACCENT2};
            if(bBack.draw()){
                loggedPerson=nullptr; screen=MAIN;
                showingInfo=false; subModalOpen=false;
            }

            struct Item{ std::string label; int id; };
            std::vector<Item> items={
                {"Sell Vehicle",1},{"My Profile",2},{"My Sales History",3}
            };
            float bw=440,bh=88,pad=18,sx=40,sy=110;
            for(int i=0;i<(int)items.size();i++){
                Button b{{sx,sy+i*(bh+pad),bw,bh},items[i].label};
                if(b.draw()&&!subModalOpen&&!showingInfo){
                    int id=items[i].id;
                    try{
                        if(id==1){
                            openSub("Sell Vehicle",
                                {{"Model ID",false},{"Customer ID",false}},
                                [&,sm](){
                                    Customer* c=Customer::getCustomer_ById(subFields[1].value);
                                    sm->sellVehicle(inventory,*c,salesManager,subFields[0].value);
                                    sm->incrementSalesCount();
                                    saveEverything(); delete c;
                                    showNotif("Vehicle sold successfully.");
                                });
                        } else if(id==2){
                            infoLines.clear(); infoScroll=0;
                            infoLines.push_back("ID:    "+sm->getID());
                            infoLines.push_back("Name:  "+sm->getName());
                            infoLines.push_back("Phone: "+sm->getPhone());
                            infoLines.push_back("CNIC:  "+sm->getCNIC());
                            infoLines.push_back("Hired: "+sm->getHireDate());
                            infoLines.push_back("Salary:"+std::to_string((int)sm->getSalary()));
                            infoLines.push_back("Sales: "+std::to_string(sm->getSalesCount()));
                            showingInfo=true;
                        } else if(id==3){
                            auto hist=salesManager.get_Sales_By_Salesman(sm->getID());
                            infoLines.clear(); infoScroll=0;
                            for(auto& sr:hist)
                                infoLines.push_back(
                                    "SaleID:"+sr.getSaleID()+
                                    "  Model:"+sr.getModelID()+
                                    "  Date:"+sr.getSaleDate()+
                                    "  Price:"+std::to_string((int)sr.getSalePrice()));
                            if(infoLines.empty()) infoLines.push_back("No sales yet.");
                            showingInfo=true;
                        }
                    } catch(const char* e){ showNotif(e,true); }
                }
            }

            if(showingInfo){
                DrawRectangle(0,0,SW,SH,{0,0,0,160});
                Rectangle area={60,60,(float)SW-120,(float)SH-140};
                DrawRectangleRec(area,PANEL);
                DrawRectangleLinesEx(area,2,ACCENT);
                drawInfoList(infoLines,{area.x+8,area.y+8,area.width-16,area.height-56},infoScroll);
                Button bClose{{area.x+area.width-230,area.y+area.height-72,210,58},"Close"};
                if(bClose.draw()||IsKeyPressed(KEY_ESCAPE)) showingInfo=false;
            }
            if(subModalOpen){
                bool confirmed=drawFormModal(subModalTitle,subFields);
                if(confirmed){
                    try{ subConfirmCb(); } catch(const char* e){ showNotif(e,true); }
                    subModalOpen=false;
                }
                if(IsKeyPressed(KEY_ESCAPE)) subModalOpen=false;
            }
        }

        else if (screen == REPAIRMAN_MENU) {
            RepairMan* rm = (RepairMan*)loggedPerson;
            DrawTextEx(gFont,
                ("RepairMan: " + rm->getName()).c_str(),
                {40,14},36,1,ACCENT);
            Button bBack{{SW-240,12,220,66},"Logout",BTN,BTNHOV,ACCENT2};
            if(bBack.draw()){
                loggedPerson=nullptr; screen=MAIN;
                showingInfo=false; subModalOpen=false;
            }

            struct Item{ std::string label; int id; };
            std::vector<Item> items={
                {"View Pending Jobs",1},{"Accept Job",2},
                {"Mark Job Completed",3},{"My Profile",4}
            };
            float bw=440,bh=88,pad=18,sx=40,sy=110;
            for(int i=0;i<(int)items.size();i++){
                Button b{{sx,sy+i*(bh+pad),bw,bh},items[i].label};
                if(b.draw()&&!subModalOpen&&!showingInfo){
                    int id=items[i].id;
                    try{
                        if(id==1){
                            auto pend=repairManager.getPendingJobs();
                            infoLines.clear(); infoScroll=0;
                            for(auto& rr:pend)
                                infoLines.push_back(
                                    "RepID:"+rr.get_RepairID()+
                                    "  Cust:"+rr.get_CustomerID()+
                                    "  Plate:"+rr.get_PlateNumber()+
                                    "  Part:"+rr.get_PartChanged()+
                                    "  Cost:"+std::to_string((int)rr.get_Cost()));
                            if(infoLines.empty()) infoLines.push_back("No pending jobs.");
                            showingInfo=true;
                        } else if(id==2){
                            openSub("Accept Job",
                                {{"Repair ID",false}},
                                [&,rm](){
                                    rm->acceptJob(repairManager,subFields[0].value);
                                    showNotif("Job accepted.");
                                });
                        } else if(id==3){
                            openSub("Mark Job Completed",
                                {{"Repair ID",false}},
                                [&,rm](){
                                    rm->markJobCompleted(repairManager,subFields[0].value);
                                    showNotif("Job marked completed.");
                                });
                        } else if(id==4){
                            infoLines.clear(); infoScroll=0;
                            infoLines.push_back("ID:    "+rm->getID());
                            infoLines.push_back("Name:  "+rm->getName());
                            infoLines.push_back("Phone: "+rm->getPhone());
                            infoLines.push_back("CNIC:  "+rm->getCNIC());
                            infoLines.push_back("Spec:  "+rm->getSpecialization());
                            infoLines.push_back("Salary:"+std::to_string((int)rm->getSalary()));
                            showingInfo=true;
                        }
                    } catch(const char* e){ showNotif(e,true); }
                }
            }

            if(showingInfo){
                DrawRectangle(0,0,SW,SH,{0,0,0,160});
                Rectangle area={60,60,(float)SW-120,(float)SH-140};
                DrawRectangleRec(area,PANEL);
                DrawRectangleLinesEx(area,2,ACCENT);
                drawInfoList(infoLines,{area.x+8,area.y+8,area.width-16,area.height-56},infoScroll);
                Button bClose{{area.x+area.width-230,area.y+area.height-72,210,58},"Close"};
                if(bClose.draw()||IsKeyPressed(KEY_ESCAPE)) showingInfo=false;
            }
            if(subModalOpen){
                bool confirmed=drawFormModal(subModalTitle,subFields);
                if(confirmed){
                    try{ subConfirmCb(); } catch(const char* e){ showNotif(e,true); }
                    subModalOpen=false;
                }
                if(IsKeyPressed(KEY_ESCAPE)) subModalOpen=false;
            }
        }

        drawNotif();
        EndDrawing();
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