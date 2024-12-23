#include <bits/stdc++.h>
#include "Appointment.cpp"
#include "DocFuns.cpp"
using namespace std;

fstream appFile, appPrimInFile;
int appointmentsAvailHead;

void getAppointmentsAvailHead (fstream& file)
{
    file.seekg(0,ios::beg);
    string temp;
    getline(file,temp);
    appointmentsAvailHead = stoi(temp);
}

string readAppointments (fstream& file)
{
    string result, temp;
    file.seekg(0, ios::beg);
    while (!file.eof())
    {
        string filter;
        getline(file, temp);

        if (temp == "-1") continue;
        for (int i = temp.find('|') + 1; i < temp.length(); i++)
        {
            if (temp[i] == '|')
                filter += " - ";
            else
                filter += temp[i];
        }

        result += filter;
        result += '\n';
    }
    result.pop_back();
    return result;
}

void printAppointmentInfo (fstream& file, const string& appointmentID)
{
    char ID[appointmentID.size() + 1];
    strcpy(ID, appointmentID.c_str());
    short index = searchID(appPrimInFile, ID, false);
    if (index == -1)
    {
        cout<<"Error: Appointment with ID " <<appointmentID <<" does not exist.\n";
        return;
    }

    file.clear();
    file.seekp(index, ios::beg);
    string record, recordInfo;
    getline(file, record);

    for (int i = record.find('|')+1; i < record.length(); i++)
    {
        if (record[i] == '|')
            recordInfo += " - ";
        else
            recordInfo += record[i];
    }

    cout<<recordInfo <<'\n';
}

void addAppointment (fstream& file, AvailList& availList, Appointment appointment)
{
    // Check for duplicate doctor ID
    if (searchID(appPrimInFile, appointment.appointmentID, false) != -1)
    {
        cout<<"Error: Appointment with ID " <<appointment.appointmentID <<" already exists.\n";
        return;
    }

    // Prepare the record
    string record = to_string(strlen(appointment.appointmentID) + strlen(appointment.appointmentDate) + strlen(appointment.doctorID) + 3)
                    + "|" + appointment.appointmentID + "|" + appointment.appointmentDate + "|" + appointment.doctorID;
    int sizeRequired = record.size()-2;
    int freeIndex = availList.findSuitableNode(file, sizeRequired);

    if (freeIndex != -1)
    {
        // Reuse free space
        file.clear();
        file.seekp(freeIndex, ios::beg);
        short index = file.tellp();
        insertPrim(appPrimInFile, appointment.appointmentID, index, false);
    }
    else
    {
        // Append to the end
        file.clear();
        file.seekp(0, ios::end);
        short index = file.tellp();
        insertPrim(appPrimInFile, appointment.appointmentID, index, false);
    }

    file<<record <<endl;
    file.flush(); // Force write to disk
    cout<<"Appointment added successfully.\n";
}

void updateAppointmentDate (fstream& file, const string& appointmentID, const string& newDate)
{
    char ID[appointmentID.size() + 1];
    strcpy(ID, appointmentID.c_str());
    short index = searchID(appPrimInFile, ID, false);
    if (index == -1)
    {
        cout<<"Error: Appointment with ID " <<appointmentID <<" does not exist.\n";
        return;
    }

    file.clear();
    file.seekp(index, ios::beg);
    string record;
    getline(file, record);

    // Get the delimiter right before the appointment date
    int firstDel = record.find('|', record.find('|') + 1);
    // Get the delimiter right before the appointment date
    int secondDel = record.find('|', firstDel + 1);
    string oldDate = record.substr(firstDel + 1, secondDel - firstDel - 1);
    string update = record;

    if(newDate.size() <= oldDate.size())
    {
        update.replace(firstDel + 1, oldDate.size(), newDate);
        // If new date is smaller, pad the remaining characters as empty spaces
        update.insert(update.size(), oldDate.size() - newDate.size(), ' ');
    }
    else
    {
        cout<<"New date is too big to update" <<endl;
        return;
    }

    file.seekp(index, ios::beg);
    file<<update <<endl;
    file.flush(); // Force write to disk
    cout<<"Appointment date updated successfully.\n";
}
