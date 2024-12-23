#include <bits/stdc++.h>
using namespace std;

class Appointment {
public:
    char appointmentID[15]{}; //PRIMARY KEY
    char appointmentDate[30]{};
    char doctorID[15]{}; //SECONDARY KEY

    Appointment()
    {
        for (int i = 0; i < 29; i++)
        {
            if (i < 14)
            {
                doctorID[i] = '_';
                appointmentDate[i]= '_';
                appointmentID[i]= '_';
            }
            else
            {
                appointmentDate[i]= '_';
            }

        }
    }

    Appointment(const string& id, const string& date , const string& docID) : Appointment()
    {

        strcpy(appointmentID, id.c_str());
        strcpy(appointmentDate, date.c_str());
        strcpy(doctorID, docID.c_str());
    }

    void printAppointment()
    {
        for (int i = 0; i < 14; i++)
        {
            if (appointmentID[i]=='_') break;
            cout<<appointmentID[i];
        }
        cout<<endl;
        for (int i = 0; i < 29; i++)
        {
            if (appointmentDate[i]=='_') break;
            cout<<appointmentDate[i];
        }
        cout<<endl;
        for (int i = 0; i < 14; i++)
        {
            if (doctorID[i]=='_') break;
            cout<<doctorID[i];
        }
        cout<<endl;
    }
};
