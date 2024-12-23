#include <queue>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
const int RECORD_SIZE = 32; // 30 bytes for name + 2 bytes for offset
const int HEADER_SIZE = 2;
int incSecDoc()
{
	fstream File("SecondaryIndexForName.txt",  ios::binary |ios::in | ios::out);
	short num;
	File.read((char*)&num, sizeof(num));
	num++;
	File.seekg(0,ios::beg); 
	File.write((char*)&num, sizeof(num));
	return num;
}
int retSecDoc()
{
	// Open the file
    fstream File("SecondaryIndexForName.txt", ios::binary | ios::in | ios::out);
    // Check if the file is empty
    File.seekg(0, ios::end); // Move to the end of the file
    if (File.tellg() == 0) {
        // File is empty, initialize with default value (0)
        short num = 0;
        File.seekp(0, ios::beg); // Move to the beginning for writing
        File.write((char*)&num, sizeof(num));
        File.close(); // Close the file
        return num;
    }

    // File is not empty, read the value
    File.seekg(0, ios::beg); // Move to the beginning for reading
    short num;
    File.read((char*)&num, sizeof(num));
    File.close(); // Close the file
    return num;
}
int decSecDoc()
{
	fstream File("SecondaryIndexForName.txt",  ios::binary |ios::in | ios::out);
	short num;
	File.read((char*)&num, sizeof(num));
	num--;
	File.seekg(0,ios::beg); 
	File.write((char*)&num, sizeof(num));
	return num;
}
int incLLDoc()
{
	fstream File("LLIndexForName.txt",  ios::binary |ios::in | ios::out);
	short num;
	File.read((char*)&num, sizeof(num));
	num++;
	File.seekg(0,ios::beg); 
	File.write((char*)&num, sizeof(num));
	return num;
}
int retLLDoc()
{
	// Open the file
    fstream File("LLIndexForName.txt", ios::binary | ios::in | ios::out);
    // Check if the file is empty
    File.seekg(0, ios::end); // Move to the end of the file
    if (File.tellg() == 0) {
        // File is empty, initialize with default value (0)
        short num = 0;
        File.seekp(0, ios::beg); // Move to the beginning for writing
        File.write((char*)&num, sizeof(num));
        File.close(); // Close the file
        return num;
    }
    // File is not empty, read the value
    File.seekg(0, ios::beg); // Move to the beginning for reading
    short num;
    File.read((char*)&num, sizeof(num));
    File.close(); // Close the file
    return num;
}
int decLLDoc()
{
	fstream File("LLIndexForName.txt",  ios::binary |ios::in | ios::out);
	short num;
	File.read((char*)&num, sizeof(num));
	num--;
	File.seekg(0,ios::beg); 
	File.write((char*)&num, sizeof(num));
	return num;
}
void insertLastRecordDoc(char name[30], char ID1[15])
{
	fstream NameSec("SecondaryIndexForName.txt", ios::binary | ios::in | ios::out);
            NameSec.seekp(2+32*retSecDoc(), ios::beg);
            NameSec.write(name, 30);
			short point =retLLDoc();
            NameSec.write((char*)&point, sizeof(point));
            incSecDoc();
            NameSec.close();
            fstream NameLink("LLIndexForName.txt", ios::binary | ios::in | ios::out);
            NameLink.seekg(2+19*retLLDoc(),ios::beg);
			NameLink.write((char*)&point, sizeof(point));
            NameLink.write(ID1, 15);
            short nega=-1; // Fixed-size "-1"
            NameLink.write((char*)&nega, sizeof(nega));
			cout<<"i am here";
            NameLink.close();
            incLLDoc();
}
void insertLastRecordDoc(char name[30], char ID1[15],short point)
{
	fstream NameSec("SecondaryIndexForName.txt", ios::binary | ios::in | ios::out);
            NameSec.seekp(2+32*retSecDoc(), ios::beg);
            NameSec.write(name, 30);
            NameSec.write((char*)&point, sizeof(point));
            incSecDoc();
            NameSec.close();
            fstream NameLink("LLIndexForName.txt", ios::binary | ios::in | ios::out);
            NameLink.seekg(2+19*retLLDoc(),ios::beg);
			NameLink.write((char*)&point, sizeof(point));
            NameLink.write(ID1, 15);
            short nega=-1; // Fixed-size "-1"
            NameLink.write((char*)&nega, sizeof(nega));
            NameLink.close();
            incLLDoc();
}
void update_pointersDoc(short point_num)
{
	short i=0,tmp;
	short sec_num=retSecDoc();
	fstream NameSec("SecondaryIndexForName.txt", ios::binary | ios::in | ios::out);
	NameSec.seekg(32,ios::beg);
	while(i<sec_num)
	{
		NameSec.read((char*)&tmp,sizeof(tmp));
		if(point_num<tmp)
		{
			NameSec.seekg(-2,ios::cur);
			tmp++;
			NameSec.write((char*)&tmp,sizeof(tmp));
		}
		NameSec.seekg(30,ios::cur);
		i++;
	}
}
void insertsortednameDoc(char name[30], char ID1[15]) // Insert sorted in the name file
{
    fstream NameSec("SecondaryIndexForName.txt", ios::binary | ios::in | ios::out);
	short CntNameSec=retSecDoc();
	short CntNameLL=retLLDoc();
    short first = 0,  last = CntNameSec - 1, mid;
    bool found = false;
    char temp[30];
    // Binary search
	short same_elem_pointer;
	// short same_elem_place;
	while (first <= last) { // Use <= instead of <
    	mid = (first + last) / 2;
    	NameSec.seekg(mid * 32 + 2, ios::beg); // Read the name at position mid
    	NameSec.read(temp, 30);
		// cout<<"temp = "<<temp<<" , name = "<<name<<endl;
    	if (strcmp(temp, name) == 0) {
        	found = true;
			NameSec.read((char*)&same_elem_pointer,sizeof(same_elem_pointer));
    	    break; // Exit the loop if a match is found
	    } else if (strcmp(temp, name) > 0) {
        	last = mid - 1; // Search in the left half
    	} else {
   	    	first = mid + 1; // Search in the right half
    	}
	}
	NameSec.close();
    if (!found) {
        if(CntNameLL==0)
        {
			insertLastRecordDoc(name,ID1);
        }
		else
        {
			fstream NameSec("SecondaryIndexForName.txt", ios::binary | ios::in | ios::out);
            NameSec.seekg(2, ios::beg);
			int i = 0 ;
            short offplace = -1;
			short bigger_pointer;
			while (i < CntNameSec){ /// this loop to get the place    ERROR here
				char s[30];
				NameSec.read(s, sizeof(s));
				if (strcmp(name, s) < 0){
					offplace = NameSec.tellg();
					NameSec.read((char*)&bigger_pointer,sizeof(bigger_pointer));
					offplace -= 30;
					// cout<<"offplace = "<<offplace<<endl<<"bigger_pointer = "<<bigger_pointer;
					break;
				}
				else{
					NameSec.seekg(2, ios::cur);
				}
				i++ ;
			}
			NameSec.close();
			if (offplace == -1){ ///it is the biggest  one
				insertLastRecordDoc(name,ID1);
			}
			else{ /// it is some where in th middle
				fstream NameSec("SecondaryIndexForName.txt", ios::binary | ios::in | ios::out);
				fstream NameLink("LLIndexForName.txt", ios::binary | ios::in | ios::out);
    			// Read the last record in secondary index file and ll file
				char endname[30],endID[15];
				short endpointer;
				NameSec.seekg(2+32*(retSecDoc()-1),ios::beg);
				NameSec.read(endname,30);
				NameSec.read((char*)&endpointer,sizeof(endpointer));
				NameLink.seekg(4+19*(retLLDoc()-1),ios::beg);
				NameLink.read(endID,15);
				// Shift records
				char tmpname1[30],tmpname2[30],tmpID1[15],tmpID2[15];
				short tmpPoint1,tmpPoint2,tmpNext1,tmpNext2;
				NameSec.seekg(offplace,ios::beg);
				NameSec.read(tmpname1,30);
				NameSec.read((char*)&tmpPoint1,sizeof(tmpPoint1));
				while(NameSec.read(tmpname2, 30) && NameSec.read((char*)&tmpPoint2, sizeof(tmpPoint2)))
				{

					NameSec.seekg(-32,ios::cur);
					NameSec.write(tmpname1,30);
					tmpPoint1++;
					NameSec.write((char*)&tmpPoint1,sizeof(tmpPoint1));
					memcpy(tmpname1, tmpname2, 30);
					tmpPoint1=tmpPoint2;
				}
				NameLink.seekg(4+bigger_pointer*19,ios::beg);
				NameLink.read(tmpID1,15);
				NameLink.read((char*)&tmpNext1,sizeof(tmpNext1));
				while(NameLink.good())
				{
					NameLink.seekg(2,ios::cur);
					NameLink.read(tmpID2,15);
					NameLink.read((char*)&tmpNext2,sizeof(tmpNext2));
					NameLink.seekg(-17,ios::cur);
					NameLink.write(tmpID1,15);
					if(tmpNext1!=-1)
					{
						tmpNext1++;
					}
					NameLink.write((char*)&tmpNext1,sizeof(tmpNext1));
					memcpy(tmpID1, tmpID2, 15);
					tmpNext1=tmpNext2;
				}
				NameSec.close();
				NameLink.close();
				NameSec.open("SecondaryIndexForName.txt", ios::binary | ios::in | ios::out);
				NameLink.open("LLIndexForName.txt", ios::binary | ios::in | ios::out);
				// Write the new record at offplace
				NameSec.seekp(offplace,ios::beg);
				NameSec.write(name,30);
				NameLink.seekg(4+bigger_pointer*19,ios::beg);
				NameLink.write(ID1,15);
				short nega=-1; // Fixed-size "-1"
            	NameLink.write((char*)&nega, sizeof(nega));
				NameSec.close();
				NameLink.close();
				// Append the last record back to the end
				insertLastRecordDoc(endname,endID,endpointer+1);
			}
		}
	}
	else if(found&&same_elem_pointer<retLLDoc()-1)
	{
		update_pointersDoc(same_elem_pointer);
			fstream NameLink("LLIndexForName.txt", ios::binary | ios::in | ios::out);
			//update next pointer of the similar name
			same_elem_pointer++;
			NameLink.seekg(same_elem_pointer*19,ios::beg);
			short Next_point,Next_order;
			NameLink.read((char*)&Next_point,sizeof(Next_point));
			while(Next_point!=-1)
			{
				cout<<endl<<"same pointer ="<<same_elem_pointer<<"Next_point = "<<Next_point<<"current place = "<<NameLink.tellg();
				NameLink.seekg(17,ios::cur);
				same_elem_pointer++;
				NameLink.read((char*)&Next_point,sizeof(Next_point));
			}
			NameLink.read((char*)&Next_order,sizeof(Next_order));
			NameLink.seekg(-4,ios::cur);
			NameLink.write((char*)&Next_order,sizeof(Next_order));
			cout<<"same_elem_pointer = "<<same_elem_pointer;
    		// Read the last record in secondary index file and ll file
				char endID[15];
				NameLink.seekg(4+19*(retLLDoc()-1),ios::beg);
				NameLink.read(endID,15);
				// Shift records
				char tmpID1[15],tmpID2[15];
				short tmpNext1,tmpNext2;
				NameLink.seekg(4+same_elem_pointer*19,ios::beg);
				NameLink.read(tmpID1,15);
				NameLink.read((char*)&tmpNext1,sizeof(tmpNext1));
				while(NameLink.good())
				{
					NameLink.seekg(2,ios::cur);
					NameLink.read(tmpID2,15);
					NameLink.read((char*)&tmpNext2,sizeof(tmpNext2));
					NameLink.seekg(-17,ios::cur);
					NameLink.write(tmpID1,15);
					if(tmpNext1!=-1)
					{
						tmpNext1++;
					}
					NameLink.write((char*)&tmpNext1,sizeof(tmpNext1));
					memcpy(tmpID1, tmpID2, 15);
					tmpNext1=tmpNext2;
				}
				NameLink.close();
				NameLink.open("LLIndexForName.txt", ios::binary | ios::in | ios::out);
				// Write the new record at offplace
				NameLink.seekg(4+same_elem_pointer*19,ios::beg);
				NameLink.write(ID1,15);
				short nega=-1; // Fixed-size "-1"
            	NameLink.write((char*)&nega, sizeof(nega));
				//write the last record
            	NameLink.seekg(2+19*retLLDoc(),ios::beg);
				NameLink.write((char*)&CntNameLL, sizeof(CntNameLL));
            	NameLink.write(endID, 15);
            	NameLink.write((char*)&nega, sizeof(nega));
            	NameLink.close();
            	incLLDoc();
	}else
	{
			update_pointersDoc(same_elem_pointer);
			fstream NameLink("LLIndexForName.txt", ios::binary | ios::in | ios::out);
			//update next pointer of the similar name
			NameLink.seekg(2+19*retLLDoc()-2,ios::beg);
			NameLink.write((char*)&CntNameLL,sizeof(CntNameLL));
			NameLink.write((char*)&CntNameLL,sizeof(CntNameLL));
			NameLink.write(ID1,15);
			short nega=-1; // Fixed-size "-1"
            NameLink.write((char*)&nega, sizeof(nega));
			cout<<"same_elem_pointer = "<<same_elem_pointer<<"  i am here";
            	NameLink.close();
            	incLLDoc();
	}
}
queue<string>  searchSecondarydoc(char name[30])
{
	fstream NameSec("SecondaryIndexForName.txt", ios::binary | ios::in | ios::out);
	short CntNameSec=retSecDoc();
	short same_elem_pointer;
	short first = 0,  last = CntNameSec - 1, mid;;
	bool found = false;
	char temp[30];
	while (first <= last) { // Use <= instead of <
    	mid = (first + last) / 2;
    	NameSec.seekg(mid * 32 + 2, ios::beg); // Read the name at position mid
    	NameSec.read(temp, 30);
		// cout<<"temp = "<<temp<<" , name = "<<name<<endl;
    	if (strcmp(temp, name) == 0) {
        	found = true;
			NameSec.read((char*)&same_elem_pointer,sizeof(same_elem_pointer));
    	    break; // Exit the loop if a match is found
	    } else if (strcmp(temp, name) > 0) {
        	last = mid - 1; // Search in the left half
    	} else {
   	    	first = mid + 1; // Search in the right half
    	}
	}
	NameSec.close();
	queue<string> ids;
	if(!found)
	{
		cout<<"element not found";
	}else
	{
		short Next_point;
		fstream NameLink("LLIndexForName.txt", ios::binary | ios::in | ios::out);
		NameLink.seekg(4+same_elem_pointer*19,ios::beg);
		char cur_id[15];
		NameLink.read(cur_id,15);
		NameLink.read((char*)&Next_point,sizeof(Next_point));
		string s= string(cur_id);
		ids.push(s);
		while(Next_point!=-1)
		{
			NameLink.seekg(2,ios::cur);
			NameLink.read(cur_id,15);
			s= string(cur_id);
			ids.push(s);
			NameLink.read((char*)&Next_point,sizeof(Next_point));
		}
	}
		return ids;
}
// void update_LL(short del_point)
// {
// 	fstream NameLink("LLIndexForName.txt", ios::binary | ios::in | ios::out);
// 	NameLink.seekg(2,ios::beg);
// 	for(short i=0;i<retLLDoc();i++)
// 	{
// 		NameLink.write((char*)&i , sizeof(i));
//         NameLink.seekg(17,ios::cur);
// 	}
// 	NameLink.seekg(17,ios::beg);
// 	for(int i=0;i<retLLDoc();i++)
// 	{
// 		short tmp;
// 		NameLink.read((char*)&tmp , sizeof(tmp));		
// 		if(tmp!=-1)
// 		{
// 			NameLink.seekg(-2,ios::cur);
// 			short x=i+1;
// 			NameLink.write((char*)&x , sizeof(x));		
// 		}
// 	}
// }
void deletebyNameLL(short del_point,short num)
{
	fstream NameLink("LLIndexForName.txt", ios::binary | ios::in | ios::out);
	short CntNameLL=retLLDoc();
	NameLink.seekg(del_point*19+4+19, ios::beg);
	short x=del_point;
	while(x < CntNameLL)              /// start to shift
	{
        char tempID[15];
        short tempof;
        NameLink.read(tempID , 15);
        NameLink.read((char*)&tempof , sizeof(tempof)) ;
        NameLink.seekg(-38+2,ios::cur) ;
        NameLink.write(tempID , 15);
        NameLink.write((char*)&tempof , sizeof(tempof)) ;
        NameLink.seekg(19,ios::cur) ;
        x++;
    }
    NameLink.close() ;
    decLLDoc();
	num--;
	// update_LL(del_point);
	if (num>0)
	{
		deletebyNameLL(del_point,num);
	}
	// same_elem_pointer++;
}
void update_sec(short mid)
{
	fstream NameSec("SecondaryIndexForName.txt", ios::binary | ios::in | ios::out);
	NameSec.seekg(mid*32+30+2,ios::beg);
	short tmp,i=mid;
	while (i<retSecDoc())
	{
		NameSec.read((char*)&tmp,sizeof(tmp));
		tmp--;
		NameSec.seekg(-2,ios::cur);
		NameSec.write((char*)&tmp,sizeof(tmp));
		i++;
	}
	
}
void deletebyName(char name[30])
{
	fstream NameSec("SecondaryIndexForName.txt", ios::binary | ios::in | ios::out);
	short CntNameSec=retSecDoc();
	short same_elem_pointer,next_elem_pointer;
	short first = 0,  last = CntNameSec - 1, mid;;
	bool found = false;
	char temp[30];
	while (first <= last) { // Use <= instead of <
    	mid = (first + last) / 2;
    	NameSec.seekg(mid * 32 + 2, ios::beg); // Read the name at position mid
    	NameSec.read(temp, 30);
    	if (strcmp(temp, name) == 0) {
        	found = true;
			NameSec.read((char*)&same_elem_pointer,sizeof(same_elem_pointer));
    	    break; // Exit the loop if a match is found
	    } else if (strcmp(temp, name) > 0) {
        	last = mid - 1; // Search in the left half
    	} else {
   	    	first = mid + 1; // Search in the right half
    	}
	}
	NameSec.seekg(30, ios::cur);
	NameSec.read((char*)&next_elem_pointer,sizeof(next_elem_pointer));
	short j=next_elem_pointer-same_elem_pointer;
	if (!found)
        return ;

        // NameSec.seekg(((CntNameSec-1) * 32+2 ), ios::beg);

        NameSec.seekg(mid*32+32 +2, ios::beg);

        int i = mid/32 ;
        while(i < CntNameSec-1)               /// start to shift
        {
            char tempname[30] ;
            short tempof ;
            NameSec.read(tempname , 30) ;
            NameSec.read((char*)&tempof , sizeof(tempof)) ;
            NameSec.seekg(-64,ios::cur) ;
            NameSec.write(tempname , 30) ;
            NameSec.write((char*)&tempof , sizeof(tempof)) ;
            NameSec.seekg(32,ios::cur) ;
            i++ ;
        }
        decSecDoc();
		update_sec(mid);
        NameSec.close() ;
		deletebyNameLL(same_elem_pointer,j);
}
