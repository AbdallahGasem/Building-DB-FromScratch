#include <bits/stdc++.h>
#include "Doctor.cpp"
#include "AvailList.cpp"
#include "PrimaryIndexes.cpp"
#include "SecondaryIndexes.cpp"
#include "SecondaryIndexesDoc_Id.cpp"
using namespace std;

fstream docFile, docPrimInFile;
int doctorsAvailHead;

void getDoctorsAvailHead(fstream &file)
{
    file.seekg(0, ios::beg);
    string temp;
    getline(file, temp);
    doctorsAvailHead = stoi(temp);
}

string readDoctors(fstream &file)
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

void printDoctorInfo (fstream& file, const string& doctorID)
{
    char ID[doctorID.size() + 1];
    strcpy(ID, doctorID.c_str());
    int index = searchID(docPrimInFile, ID, true);
    if (index == -1)
    {
        cout<<"Error: Doctor with ID " << doctorID <<" does not exist.\n";
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

vector <string> split( string target , char delimiter )
{
    // declaring the vector that holds the substrings and a substr var
    vector<string>splittedVec;
    string substr = "" ;
    int len = target.size() ;


    for (int i = 0 ; i < len ; i++)
    {
        // appending charachters till meeting the delimiter :

        if (target[i] != delimiter)
        {
            substr.push_back(char(target[i])) ;
        }

            // pushing the splitted substr as it has met the delimiter :
        else
        {
            splittedVec.push_back(substr) ;
            substr.clear() ;
        }

    }

    // including the last substring in the vector
    splittedVec.push_back(substr) ;

    return splittedVec ;

}

string GetDoctorName (fstream& file, const string& doctorID)
{
    char ID[doctorID.size() + 1];
    strcpy(ID, doctorID.c_str());
    int index = searchID(docPrimInFile, ID, true);
    if (index == -1)
    {
        cout<<"Error: Doctor with ID " << doctorID <<" does not exist.\n";
        return "";
    }

    file.clear();
    file.seekp(index, ios::beg);
    string record;
    vector<string> recordInfo;
    getline(file, record);

    recordInfo = split(record,'|');

    return recordInfo[2];
}
// string GetAppName (fstream& file, const string& appointmentID)
// {
//     char ID[appointmentID.size() + 1];
//     strcpy(ID, appointmentID.c_str());
//     int index = searchID(appPrimInFile, ID, true);
//     if (index == -1)
//     {
//         cout<<"Error: Doctor with ID " << appointmentID <<" does not exist.\n";
//         return "";
//     }

//     file.clear();
//     file.seekp(index, ios::beg);
//     string record;
//     vector<string> recordInfo;
//     getline(file, record);

//     recordInfo = split(record,'|');

//     return recordInfo[2];
// }

void addDoctor(fstream &file, AvailList &availList, Doctor doctor)
{
    // Check for duplicate doctor ID
    if (searchID(docPrimInFile, doctor.doctorID, true) != -1)
    {
        cout << "Error: Doctor with ID " << doctor.doctorID << " already exists.\n";
        return;
    }

    // Prepare the record
    string record = to_string(strlen(doctor.doctorID) + strlen(doctor.doctorName) + strlen(doctor.address) + 3) + "|" + doctor.doctorID + "|" + doctor.doctorName + "|" + doctor.address;
    int sizeRequired = record.size()-2;
    int freeIndex = availList.findSuitableNode(file, sizeRequired);

    if (freeIndex != -1)
    {
        // Reuse free space
        file.clear();
        file.seekp(freeIndex, ios::beg);
        short index = file.tellp();
        insertPrim(docPrimInFile, doctor.doctorID, index, true);
        insertsortednameDoc(doctor.doctorName, doctor.doctorID);
    }
    else
    {
        // Append to the end
        file.clear();
        file.seekp(0, ios::end);
        short index = file.tellp();
        insertPrim(docPrimInFile, doctor.doctorID, index, true);
        insertsortednameDoc(doctor.doctorName, doctor.doctorID);
    }

    file << record << endl;
    file.flush(); // Force write to disk
    cout << "Doctor added successfully.\n";
}

void updateDoctorName(fstream &file, const string &doctorID, const string &newName)
{
    char ID[doctorID.size() + 1];
    strcpy(ID, doctorID.c_str());
    int index = searchID(docPrimInFile, ID, true);
    if (index == -1)
    {
        cout << "Error: Doctor with ID " << doctorID << " does not exist.\n";
        return;
    }

    file.clear();
    file.seekp(index, ios::beg);
    string record;
    getline(file, record);

    // Get the delimiter right before the doctor name
    int firstDel = record.find('|', record.find('|') + 1);
    // Get the delimiter right after the doctor name
    int secondDel = record.find('|', firstDel + 1);
    string oldName = record.substr(firstDel + 1, secondDel - firstDel - 1);
    string update = record;

    if (newName.size() <= oldName.size())
    {
        update.replace(firstDel + 1, oldName.size(), newName);
        // If new name is smaller, pad the remaining characters as empty spaces
        update.insert(update.size(), oldName.size() - newName.size(), ' ');
    }
    else
    {
        cout << "New name is too big to update" << endl;
        return;
    }

    file.seekp(index, ios::beg);
    file << update << endl;
    file.flush(); // Force write to disk
    cout << "Doctor name updated successfully.\n";
}

void deleteRecord(fstream &file, AvailList &doctorsAvailList, const string &doctorId , bool doc)
{
    fstream idxFile;
    int recordIndex;
    if (doc)
    {
        idxFile.open("DocPrimaryIndex.txt");
    }
    else
    {
        idxFile.open("AppPrimaryIndex.txt");
    }

    // file status validation
    if(idxFile.is_open()){
        //Find the record to be deleted by finding its index and retrieving the byte offset
        try
        {
            recordIndex = searchID(idxFile, doctorId.c_str(), doc) ;

        }
        catch(const std::exception& e)
        {
            cerr << "ERROR! Deleting a record, SearchID() Exception\n";
            std::cerr << e.what() << '\n';
        }
        
        idxFile.close();
    }
    else
    {
        cerr << "ERROR! Deleting a Record, Index File Didnot open\n";
        return;
    }

    // validating record existence
    if (recordIndex == -1)
    {
        cout << "Error: Doctor with ID " << doctorId << " not found.\n";
        return;
    }

    //Read the current record's data
    file.seekg(recordIndex+2, ios::beg);
    string doctorRecord;
    getline(file, doctorRecord);

    //Calculate the length of the record
    int recordLength = doctorRecord.length();

    //Read the avail list head
    file.seekg(0, ios::beg);
    string tempAvail;
    getline(file, tempAvail);
    int availHead = stoi(tempAvail);  // This points to the first available record in the list

    //Mark the current record as deleted by replacing it with "*|availHead|original record size"
    file.seekp(recordIndex+2, ios::beg);
    file << "*|" << availHead << "|" << recordLength << "|";  // Mark the record as deleted, with its size
    //Update the avail list to point to the current record
    doctorsAvailList.updateHead(file, recordIndex);
    file.flush();

    // deleting the recordId form the primary index
    deletePrim(docPrimInFile,doctorId.c_str(),doc);
    cout << "Doctor with ID " << doctorId << " deleted successfully.\n";
}
