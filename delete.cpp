//                                                      بسم الله الرحمن الرحيم                                                 //
// program: delete.cpp
// Description:
// Author:  Abdallah Gasem
// Date: 10-12-2024
// Version: 1.0
/* file run command: g++ -o delete delete.cpp; if ($?) { ./delete } */
// ----------------------------------------------------------------------------------------------------------------------------- //

#include <bits/stdc++.h>
#include "AppFuns.cpp"

using namespace std;

// void deleteDoctor(fstream &file, AvailList &doctorsAvailList, const string &doctorId)
// {
//     // int temp;
//     // Find the record to be deleted by finding its index and retrieving the byte offset
//     // int recordIndex = searchID(docPrimInFile, doctorId.c_str(), true);
//     // testing
//     int recordIndex = 29;

//     if (recordIndex == -1)
//     {
//         cout << "Error: Doctor with ID " << doctorId << " not found.\n";
//         return;
//     }

//     // Read the current record's data
//     file.seekg(recordIndex + 2, ios::beg);
//     string doctorRecord;
//     getline(file, doctorRecord);

//     // Calculate the length of the record
//     int recordLength = doctorRecord.length();

//     // Read the avail list head
//     file.seekg(0, ios::beg);
//     string tempAvail;
//     getline(file, tempAvail);
//     int availHead = stoi(tempAvail); // This points to the first available record in the list

//     // Mark the current record as deleted by replacing it with "*|availHead|recordLength|"
//     file.seekp(recordIndex + 2, ios::beg);
//     string marker = "*|" + to_string(availHead) + "|" + to_string(recordLength) + "|";
//     file << marker << string(recordLength - marker.size(), ' ') << flush; // Overwrite remaining space with blanks
//     // Update the avail list to point to the current record
//     doctorsAvailList.updateHead(file, recordIndex);

//     // delete the doctorId form the primary index

//     cout << "Doctor with ID " << doctorId << " deleted successfully.\n";
// }

short search(fstream &file, char id[] , bool doc)
{
    
	file.seekg(2, ios::beg);

	short Recordlen, Namelen, Idlen;
	bool b = 0;
	char c;

	while (file.good())
	{ 

		file.read((char *)&Recordlen, sizeof(Recordlen));
		file.read((char*)&c, sizeof(c));
		if (c != '*'){
                    // 330420235Reham042023
                    // 35
			file.seekg(-1, ios::cur);
			file.read((char*)& Namelen, sizeof(Namelen));
			file.seekg(Namelen, ios::cur);

			file.read((char*)&Idlen, sizeof(Idlen));
			char tmp[50];
			file.read(tmp, Idlen);
			tmp[Idlen] = '\0';
			if (strcmp(tmp, id) == 0)
			{
				short h = -1 * (Namelen + Idlen + 2 + 2);
				file.seekg(h, ios::cur);
				b = 1;
				break;

			}
			else
			{

				short h = -1 * (Namelen + Idlen + 2 + 2);
				file.seekg(h, ios::cur);
				file.seekg(Recordlen, ios::cur);
			}
		}
		else
                    // 
			file.seekg(Recordlen - 1, ios::cur);

	}
	short temp = file.tellg();
	// file.close();
	if (b == 1)
		return temp;
	else
		return -1;


}

void Delete(fstream &file, char id[], bool doc)
{
    // First Fit , Best Fit , Worst Fit (Avail List)
    // 10 50 20 100
    // 10 20 50 100
    // 100 50 20 10
    // New : 15

	short M = -1;
    // if (doc)
    // {
    //     file.open("Doctors.txt", ios::in | ios::out | ios::binary);
    // }
    // else
    // {
    //     file.open("Appointments.txt", ios::in | ios::out | ios::binary);   
    // }
    
    
	short  Offset = searchID(file, id, doc), Size; /// read offset
	file.seekg(Offset , ios::beg);
        //3305Reham
	file.seekg(Offset - 2, ios::beg);
	file.read((char*)&Size, sizeof(Size)); /// read size
	if (Offset == -1)
		cout << "Not valid ID " << endl;
	else
	{
		file.seekg(0, ios::beg);
		short header;
		file.read((char*)&header, sizeof(header)); /// read header

		if (header == -1) /// if list is empty
		{
			file.seekg(0, ios::beg);
			file.write((char*)&Offset, sizeof(Offset));
			file.seekg(Offset, ios::beg);
			file.write("*", 1);
			file.write((char*)&M, sizeof(M));
			file.close();
		}
		else /// list is not empty
		{ // 100 20 15 10 --> -1
                    
       
			file.seekg(0, ios::beg);
			file.read((char*)&header, sizeof(header)); /// read header
			short HeaderSize;
			file.seekg(header - 2, ios::beg);
			file.read((char*)&HeaderSize, sizeof(HeaderSize)); /// read header size
			if (Size >= HeaderSize) /// if deleted size >= header size
			{
				file.seekg(0, ios::beg);
				file.write((char*)&Offset, sizeof(Offset)); /// write offset in the head
				file.seekg(Offset, ios::beg);
				file.write("*", 1);
				file.write((char*)&header, sizeof(header)); /// write header offset in deleted item
				file.close();

			}
			else /// deleted size  < header size
			{
                            // prev new next  
                           
				file.seekg(0, ios::beg);
				short OffsetPre, SizeNext, TempOffset;
				OffsetPre = header;
				file.seekg(header + 1, ios::beg); 
				while (1)
				{ // 100 50 10--> 5 -->-1
                                    

					file.read((char*)&TempOffset, sizeof(TempOffset));
					if (TempOffset == -1) /// if reached the end of list
					{
						file.seekg(Offset, ios::beg);
						file.write("*", 1);
						file.write((char*)&M, sizeof(M));
						file.seekg(OffsetPre + 1, ios::beg);
						file.write((char*)&Offset, sizeof(Offset));
						file.close();
						break;
					}
					else
					{
						file.seekg(TempOffset - 2, ios::beg);
						file.read((char*)&SizeNext, sizeof(SizeNext));
						if (Size >= SizeNext) /// if  temp size <= deleted size
						{
							file.seekg(Offset, ios::beg);
							file.write("*", 1);
							file.write((char*)&TempOffset, sizeof(TempOffset));
							file.seekg(OffsetPre + 1, ios::beg);
							file.write((char*)&Offset, sizeof(Offset));
							file.close();
							break;

						}
						else /// move to the next record
						{
							OffsetPre = TempOffset;
							file.seekg(OffsetPre + 1, ios::beg);
						}
					}



				}

				file.close();
			}

		}
	}



}

int main() {


    // cout << searchID(docPrimInFile,id.c_str(),true) << endl;    // expected: 29|1|tarek hamed|helmyt el zton , and its offset
    // cout << searchID(appPrimInFile,id.c_str(),false) << endl;   // expected: 14|1|9/11/2001|1  , and its offset
    string id = "1";
    char ID[id.length()];
    strcpy(ID,id.c_str());

    fstream test("test.txt", ios::in | ios::out | ios::binary);
    cout << search(test, ID ,true);
    test.close();
    return 0;
}