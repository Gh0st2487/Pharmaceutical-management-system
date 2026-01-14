#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
using namespace std;

const int maxx = 10;
bool flag = false;

class drug
{
protected:
    char name[50];
    float price;
    int quantity;

public:
    drug() {}
    drug(char *name, float pr, int q)
    {
        strcpy_s(this->name, name);
        quantity = q;
        price = pr;
    }

    virtual void display() = 0;

    int getQuantity()
    {
        return quantity;
    }

    float getPrice()
    {
        return price;
    }

    void reduceQuantity(int qty)
    {
        quantity -= qty;
    }

    void editQuantity(int qty)
    {
        quantity = qty;
    }

    void editPrice(float newPrice)
    {
        price = newPrice;
    }

    char *getName()
    {
        return name;
    }
};

class tablets : public drug
{
protected:
    char manufacturer[30];

public:
    tablets() {}
    tablets(char *manu, char *name, float pr, int q) : drug(name, pr, q)
    {
        strcpy_s(manufacturer, manu);
    }

    void display() override
    {
        cout << "Name: " << name << endl;
        cout << "Price: " << price << endl;
        cout << "Quantity: " << quantity << endl;
        cout << "manufacturer: " << manufacturer << endl;
    }
};

class syrup : public drug
{
    char taste[40];

public:
    syrup() {}
    syrup(char *taste, char *name, int q, float pr) : drug(name, pr, q)
    {
        strcpy_s(this->taste, taste);
    }

    void display() override
    {
        cout << "Name: " << name << endl;
        cout << "Quantity: " << quantity << endl;
        cout << "Price: " << price << endl;
        cout << "Taste: " << taste << endl;
    }
};

class inventory
{
    int current;
    float total=0.0;

public:
    inventory()
    {
        current = 0;
    }

    void delete_drug_tab(char *nm)
    {
        tablets obj;
        fstream file("tab_inventory.dat", ios::binary | ios::app | ios::in);
        fstream temp("temp.dat", ios::binary | ios::app | ios::in);

        while (file.read((char *)&obj, sizeof(tablets)))
        {
            if ((strcmp(obj.getName(), nm)) != 0)
            {

                temp.write((char *)&obj, sizeof(tablets));
            }
        }

        file.close();
        temp.close();
        remove("tab_inventory.dat");
        rename("temp.dat", "tab_inventory.dat");
    }

    void delete_drug_syr(char *nm)
    {
        syrup obj;
        fstream file("syr_inventory.dat", ios::binary | ios::app | ios::in);
        fstream temp("temp.dat", ios::binary | ios::app | ios::in);

        while (file.read((char *)&obj, sizeof(syrup)))
        {
            if ((strcmp(obj.getName(), nm)) != 0)
            {

                temp.write((char *)&obj, sizeof(syrup));
            }
        }

        file.close();
        temp.close();
        remove("syr_inventory.dat");
        rename("temp.dat", "syr_inventory.dat");
    }

    void read_from_file()
    {
        tablets obj;
        cout << "Tablets: " << endl;
        ifstream file("tab_inventory.dat", ios::binary);
        while (file.read((char *)&obj, sizeof(tablets)))
        {
            obj.display();
        }
        file.close();

        cout << "\nSyrups: \n"
             << endl;
        syrup sobj;
        ifstream filee("syr_inventory.dat", ios::binary);
        while (filee.read((char *)&sobj, sizeof(syrup)))
        {
            sobj.display();
        }
        filee.close();
    }

    void write_in_file_tab(tablets t)
    {
        ofstream file("tab_inventory.dat", ios::binary | ios::out | ios::app);
        if (!file)
        {
            cout << "file not found" << endl;
            return;
        }

        file.write((char *)&t, sizeof(tablets));
    }

    void write_in_file_syr(syrup s)
    {
        ofstream file("syr_inventory.dat", ios::binary | ios::out | ios::app);
        if (!file)
        {
            cout << "file not found" << endl;
            return;
        }

        file.write((char *)&s, sizeof(syrup));
    }

    void get_Total()
    {
        cout << "Total amount: " << total << endl;
        // total = 0;
    }

    void sale(char *nam, int qty)
    {

        tablets obj;
        fstream file("tab_inventory.dat", ios::binary | ios::in | ios::out);
        bool fnd = false;
        while (file.read((char *)&obj, sizeof(obj)))
        {
            if ((strcmp(obj.getName(), nam)) == 0)
            {
                fnd = true;
                if (obj.getQuantity() > qty)
                {
                    obj.reduceQuantity(qty);
                    total += obj.getPrice();
                    int pos = file.tellg();
                    file.seekp(pos - sizeof(obj), ios::beg);
                    file.write((char *)&obj, sizeof(obj));
                }
                else
                {
                    cout << "Required quantity of drug is not available\nTotal qty: " << obj.getQuantity() << endl;
                }
                
                break;
            }
        }
        file.close();

        syrup sobj;
        fstream filee("syr_inventory.dat", ios::binary | ios::in | ios::out);

        while (filee.read((char *)&sobj, sizeof(sobj)))
        {
            if ((strcmp(sobj.getName(), nam)) == 0)
            {
                fnd = true;
                if (sobj.getQuantity() > qty)
                {
                    sobj.reduceQuantity(qty);
                    total += sobj.getPrice();
                    int pos = filee.tellg();
                    filee.seekp(pos - sizeof(sobj), ios::beg);
                    filee.write((char *)&sobj, sizeof(sobj));
                }
                else
                {
                    cout << "Required quantity of drug is not available\nTotal qty: " << sobj.getQuantity() << endl;
                }

                break;
            }
        }
        filee.close();
        if (fnd == false)
        {
            cout << "Entered drug is not available" << endl;
        }
    }

    void editDrugInformation_tab(char *nm)
    {
        int ch;
        cout << "press 1 to edit price\n press 2 to edit quantity";
        cin >> ch;

        if (ch == 1)
        {
            float npr;
            cout << "Enter new price: ";
            cin >> npr;

            tablets obj;
            bool found = false;
            fstream file("tab_inventory.dat", ios::binary | ios::in | ios::out);

            while (file.read((char *)&obj, sizeof(obj)))
            {
                if ((strcmp(obj.getName(), nm)) == 0)
                {
                    
                        
                        found == true;
                    obj.editPrice(npr);
                    int pos = file.tellg();
                    file.seekp(pos - sizeof(obj), ios::beg);
                    file.write((char *)&obj, sizeof(obj));
                    break;
                }
            }
            file.close();
            if (found == false) {
                cout << "Not found" << endl;
            }
        }

        if (ch == 2)
        {
            int n;
            cout << "Enter new quantity: ";
            cin >> n;

            tablets obj;
            bool found = false;
            fstream file("tab_inventory.dat", ios::binary | ios::in | ios::out);

            while (file.read((char *)&obj, sizeof(obj)))
            {
                if ((strcmp(obj.getName(), nm)) == 0)
                {
                    found == true;
                    obj.editQuantity(n);
                    int pos = file.tellg();
                    file.seekp(pos - sizeof(obj), ios::beg);
                    file.write((char *)&obj, sizeof(obj));
                    break;
                }
            }
            file.close();
        }
    }

    void editDrugInformation_syr(char *nm)
    {
        int ch;
        cout << "press 1 to edit price\n press 2 to edit quantity";
        cin >> ch;

        if (ch == 1)
        {
            float npr;
            cout << "Enter new price: ";
            cin >> npr;

            syrup obj;
            bool found = false;
            fstream file("syr_inventory.dat", ios::binary | ios::in | ios::out);

            while (file.read((char *)&obj, sizeof(obj)))
            {
                if ((strcmp(obj.getName(), nm)) == 0)
                {
                    found == true;
                    obj.editPrice(npr);
                    int pos = file.tellg();
                    file.seekp(pos - sizeof(obj), ios::beg);
                    file.write((char *)&obj, sizeof(obj));
                    break;
                }
            }
            file.close();
        }

        if (ch == 2)
        {
            int n;
            cout << "Enter new quantity: ";
            cin >> n;

            syrup obj;
            bool found = false;
            fstream file("syr_inventory.dat", ios::binary | ios::in | ios::out);

            while (file.read((char *)&obj, sizeof(obj)))
            {
                if ((strcmp(obj.getName(), nm)) == 0)
                {
                    found == true;
                    obj.editQuantity(n);
                    int pos = file.tellg();
                    file.seekp(pos - sizeof(obj), ios::beg);
                    file.write((char *)&obj, sizeof(obj));
                    break;
                }
            }
            file.close();
        }
    }
};

class admin
{
    string username;
    string password;

public:
    void reg(string unn, string ps)
    {

        ofstream file;
        file.open(unn + ".txt");
        file << unn << endl
             << ps;
        file.close();
        cout << "Account created succesfully!" << endl;
    }

    void login(string user, string pass)
    {
        ifstream read(user + ".txt");
        getline(read, username);
        getline(read, password);
        if (user == username && pass == password)
        {
            flag = true;
            cout << "Welcome " << username << endl;
        }
        else
        {
            cout << "Incorrect credentials!" << endl;
        }
    }
};

void main_menu()
{
    
    cout << "Press 1 to Display thr inventory" << endl;
    cout << "Press 2 to purchase a medicine" << endl;
    cout << "Press 3 to edit any medicines info" << endl;
    cout << "Press 4 to delete medicine" << endl;
    cout << "Press 5 to add a new medicine to the inventory" << endl;
    cout << "Press 6 to register a new account for admin access" << endl;
    cout << "Press 7 to Display your bill" << endl;
    cout << "Press 8 to Login" << endl;
    cout << "Press 0 to exit the program" << endl;
}

int main()
{
    /* char unn[50], ps[30];
     float p; int sl;
     inventory ii;
     for (int i = 0; i < 2; i++) {
         cout << "enter manu: ";
         cin >> unn;
         cout << "enter name: ";
         cin >> ps;
         cout << "enter pr: ";
         cin >> p;
         cout << "enter qty: ";
         cin >> sl;
         syrup temp;
         temp = { unn,ps,sl,p };
         ii.write_in_file_syr(temp);
     }*/
    int ch = -99, qty, ch2, nqty;
    inventory ii;
    char nm[50], nname[50], taste[40], manu[30];
    string un, ps;
    float pr;
    admin a;

    cout << "###################################################" << endl;
    cout << "###################################################" << endl;
    cout << "######\t\t Welcome\t\t    #######" << endl;
    cout << "###################################################" << endl;
    cout << "###################################################" << endl;

    while (ch != 0)
    {
        cout << "*********************************************************" << endl;
        main_menu();
        cin >> ch;

        switch (ch)
        {
        case 1:
            ii.read_from_file();
            break;

        case 2:
            cout << "Enter drug name you want to purchase";
            cin >> nm;
            cout << "Enter quantity you want to purchase";
            cin >> qty;
            ii.sale(nm, qty);
            break;

        case 3:
            if (flag == false)
            {
                cout << "You need admin access for this function\nplease login first" << endl;
            }
            else
            {
                cout << "To edit any syrups info press 1\n press 2 to edit any tablets info: ";
                cin >> ch2;
                if (ch2 == 1)
                {
                    cout << "Enter syrup name: ";
                    cin >> nm;
                    ii.editDrugInformation_syr(nm);
                }
                else if (ch2 == 2)
                {
                    cout << "Enter tablets name: ";
                    cin >> nm;
                    ii.editDrugInformation_tab(nm);
                }
                else
                {
                    cout << "Invalid input!" << endl;
                }
            }
            break;

        case 4:
            if (flag == false)
            {
                cout << "You need admin access for this function\nplease login first" << endl;
            }
            else
            {
                cout << "To delete any syrups info press 1\n press 2 to delete any tablets info: ";
                cin >> ch2;
                if (ch2 == 1)
                {
                    cout << "Enter syrup name: ";
                    cin >> nm;
                    ii.delete_drug_syr(nm);
                }
                else if (ch == 2)
                {
                    cout << "Enter tablets name: ";
                    cin >> nm;
                    ii.delete_drug_tab(nm);
                }
                else
                {
                    cout << "Invalid input!" << endl;
                }
            }
            break;

        case 5:
            if (flag == false)
            {
                cout << "You need admin access for this function\nplease login first" << endl;
            }
            else
            {
                cout << "Enter name: ";
                cin >> nname;
                cout << "Enter quantity: ";
                cin >> nqty;
                cout << "enter price: ";
                cin >> pr;
                cout << "To add new syrups press 1\n press 2 to add new tablets: ";
                cin >> ch2;
                if (ch2 == 1)
                {
                    cout << "Enter taste: ";
                    cin >> taste;
                    syrup s;
                    s = {taste, nname, nqty, pr};
                    ii.write_in_file_syr(s);
                }
                else if (ch2 == 2)
                {
                    cout << "Enter manufacturer: ";
                    cin >> manu;
                    tablets t;
                    t = {manu, nname, pr, nqty};
                    ii.write_in_file_tab(t);
                }
                else
                {
                    cout << "Invalid input!" << endl;
                }
            }
            break;

        case 6:
            if (flag == false)
            {
                cout << "You need admin access to create an account\nplease ask an admin to login first" << endl;
            }
            else
            {
                cout << "Enter username: ";
                cin >> un;
                cout << "Enter password: ";
                cin >> ps;
                a.reg(un, ps);
            }
            break;

        case 7:
            ii.get_Total();
            break;

        case 8:
            cout << "Enter username: ";
            cin >> un;
            cout << "Enter password: ";
            cin >> ps;
            a.login(un, ps);
            break;
        }
    }
}