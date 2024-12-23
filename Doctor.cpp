#include <bits/stdc++.h>
using namespace std;

class Doctor {
public:
    char doctorID[15]{}; //PRIMARY KEY
    char doctorName[30]{};
    char address[30]{};

    Doctor()
    {
        for (int i = 0; i < 29; i++)
        {
            if (i < 14)
            {
                doctorID[i] = '_';
                doctorName[i]= '_';
                address[i]= '_';
            }
            else
            {
                doctorName[i]= '_';
                address[i]= '_';
            }

        }
    }

    Doctor (const string& id, const string& name, const string& address) : Doctor()
    {

        strcpy(this->doctorID, id.c_str());
        strcpy(this->doctorName, name.c_str());
        strcpy(this->address, address.c_str());
    }

    void printDoctor()
    {
        for (int i = 0; i < 14; i++)
        {
            if (doctorID[i]=='_') break;
            cout<<doctorID[i];
        }
        cout<<endl;
        for (int i = 0; i < 29; i++)
        {
            if (doctorName[i]=='_') break;
            cout<<doctorName[i];
        }
        cout<<endl;
        for (int i = 0; i < 29; i++)
        {
            if (address[i]=='_') break;
            cout<<address[i];
        }
        cout<<endl;
    }
};
