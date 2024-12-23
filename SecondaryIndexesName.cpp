#include <queue>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
const int RECORD_SIZE = 32; // 30 bytes for name + 2 bytes for offset
const int HEADER_SIZE = 2;
int incSec()
{
	fstream File("SecondaryIndexForName.txt",  ios::binary |ios::in | ios::out);
	short num;
	File.read((char*)&num, sizeof(num));
	num++;
	File.seekg(0,ios::beg); 
	File.write((char*)&num, sizeof(num));
	return num;
}
int retSec()
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
int decSec()
{
	fstream File("SecondaryIndexForName.txt",  ios::binary |ios::in | ios::out);
	short num;
	File.read((char*)&num, sizeof(num));
	num--;
	File.seekg(0,ios::beg); 
	File.write((char*)&num, sizeof(num));
	return num;
}
int incLL()
{
	fstream File("LLIndexForName.txt",  ios::binary |ios::in | ios::out);
	short num;
	File.read((char*)&num, sizeof(num));
	num++;
	File.seekg(0,ios::beg); 
	File.write((char*)&num, sizeof(num));
	return num;
}
int retLL()
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
int decLL()
{
	fstream File("LLIndexForName.txt",  ios::binary |ios::in | ios::out);
	short num;
	File.read((char*)&num, sizeof(num));
	num--;
	File.seekg(0,ios::beg); 
	File.write((char*)&num, sizeof(num));
	return num;
}
void insertLastRecord(char name[30], char ID1[15])
{
	fstream NameSec("SecondaryIndexForName.txt", ios::binary | ios::in | ios::out);
            NameSec.seekp(2+32*retSec(), ios::beg);
            NameSec.write(name, 30);
			short point =retLL();
            NameSec.write((char*)&point, sizeof(point));
            incSec();
            NameSec.close();
            fstream NameLink("LLIndexForName.txt", ios::binary | ios::in | ios::out);
            NameLink.seekg(2+19*retLL(),ios::beg);
			NameLink.write((char*)&point, sizeof(point));
            NameLink.write(ID1, 15);
            short nega=-1; // Fixed-size "-1"
            NameLink.write((char*)&nega, sizeof(nega));
            NameLink.close();
            incLL();
}
void insertLastRecord(char name[30], char ID1[15],short point)
{
	fstream NameSec("SecondaryIndexForName.txt", ios::binary | ios::in | ios::out);
            NameSec.seekp(2+32*retSec(), ios::beg);
            NameSec.write(name, 30);
            NameSec.write((char*)&point, sizeof(point));
            incSec();
            NameSec.close();
            fstream NameLink("LLIndexForName.txt", ios::binary | ios::in | ios::out);
            NameLink.seekg(2+19*retLL(),ios::beg);
			NameLink.write((char*)&point, sizeof(point));
            NameLink.write(ID1, 15);
            short nega=-1; // Fixed-size "-1"
            NameLink.write((char*)&nega, sizeof(nega));
            NameLink.close();
            incLL();
}
void update_pointers(short point_num)
{
	short i=0,tmp;
	short sec_num=retSec();
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
void insertsortedname(char name[30], char ID1[15]) // Insert sorted in the name file
{
    fstream NameSec("SecondaryIndexForName.txt", ios::binary | ios::in | ios::out);
	short CntNameSec=retSec();
	short CntNameLL=retLL();
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
			insertLastRecord(name,ID1);
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
					break;
				}
				else{
					NameSec.seekg(2, ios::cur);
				}
				i++ ;
			}
			NameSec.close();
			if (offplace == -1){ ///it is the biggest  one
				insertLastRecord(name,ID1);
			}
			else{ /// it is some where in th middle
				fstream NameSec("SecondaryIndexForName.txt", ios::binary | ios::in | ios::out);
				fstream NameLink("LLIndexForName.txt", ios::binary | ios::in | ios::out);
    			// Read the last record in secondary index file and ll file
				char endname[30],endID[15];
				short endpointer;
				NameSec.seekg(2+32*(retSec()-1),ios::beg);
				NameSec.read(endname,30);
				NameSec.read((char*)&endpointer,sizeof(endpointer));
				NameLink.seekg(4+19*(retLL()-1),ios::beg);
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
				insertLastRecord(endname,endID,endpointer+1);
			}
		}
	}
	else if(found&&same_elem_pointer<retLL()-1)
	{
		update_pointers(same_elem_pointer);
			fstream NameLink("LLIndexForName.txt", ios::binary | ios::in | ios::out);
			//update next pointer of the similar name
			same_elem_pointer++;
			NameLink.seekg(same_elem_pointer*19,ios::beg);
			short Next_point,Next_order;
			NameLink.read((char*)&Next_point,sizeof(Next_point));
			while(Next_point!=-1)
			{
				NameLink.seekg(17,ios::cur);
				same_elem_pointer++;
				NameLink.read((char*)&Next_point,sizeof(Next_point));
			}
			NameLink.read((char*)&Next_order,sizeof(Next_order));
			NameLink.seekg(-4,ios::cur);
			NameLink.write((char*)&Next_order,sizeof(Next_order));
    		// Read the last record in secondary index file and ll file
				char endID[15];
				NameLink.seekg(4+19*(retLL()-1),ios::beg);
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
            	NameLink.seekg(2+19*retLL(),ios::beg);
				NameLink.write((char*)&CntNameLL, sizeof(CntNameLL));
            	NameLink.write(endID, 15);
            	NameLink.write((char*)&nega, sizeof(nega));
            	NameLink.close();
            	incLL();
	}else
	{
			update_pointers(same_elem_pointer);
			fstream NameLink("LLIndexForName.txt", ios::binary | ios::in | ios::out);
			//update next pointer of the similar name
			NameLink.seekg(2+19*retLL()-2,ios::beg);
			NameLink.write((char*)&CntNameLL,sizeof(CntNameLL));
			NameLink.write((char*)&CntNameLL,sizeof(CntNameLL));
			NameLink.write(ID1,15);
			short nega=-1; // Fixed-size "-1"
            NameLink.write((char*)&nega, sizeof(nega));
            	NameLink.close();
            	incLL();
	}
}
queue<string>  search(char name[30])
{
	fstream NameSec("SecondaryIndexForName.txt", ios::binary | ios::in | ios::out);
	short CntNameSec=retSec();
	short same_elem_pointer;
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
		ids.push(string(cur_id));
		while(Next_point!=-1)
		{
			cout<<"current id = "<<cur_id;
			cout<<"next point = "<<Next_point<<endl;
			NameLink.seekg(2,ios::cur);
			NameLink.read(cur_id,15);
			ids.push(string(cur_id));			
			NameLink.read((char*)&Next_point,sizeof(Next_point));
		}
		NameLink.close();
	}
	return ids;
	// 2
	// 2 15 2
	// 2 15 2
	// 2 15 2
	// 2 15 2
	// 2 15 2
	// 2 15 2
}
void update_LL(short del_point)
{
	fstream NameLink("LLIndexForName.txt", ios::binary | ios::in | ios::out);
	short x=0,tmp;
	NameLink.seekg(2,ios::beg);
	for(short i=0;i<retLL();i++)
	{
		NameLink.write((char*)&x , sizeof(x));
        NameLink.seekg(17,ios::cur);
		x++;
	}
	short curRecord,Next_point;
	NameLink.seekg(del_point*19+2,ios::beg);
	NameLink.read((char*)&curRecord , sizeof(curRecord));
	NameLink.seekg(15,ios::cur);
	for(int i=0;i<retLL()-del_point;i++)
	{
		NameLink.read((char*)&Next_point , sizeof(Next_point));		
		if(Next_point!=-1)
		{
			NameLink.seekg(-2,ios::cur);
			curRecord++;
			NameLink.write((char*)&curRecord , sizeof(curRecord));		
		}
		NameLink.read((char*)&curRecord , sizeof(curRecord));
        NameLink.seekg(17,ios::cur);
	}
}
void update_sec(short mid,short num)
{
	fstream NameSec("SecondaryIndexForName.txt", ios::binary | ios::in | ios::out);
	while(num>0)
	{
		short i=mid;
    	short tmp = 0;
    	for (short i = mid; i < retSec() - 1; i++) {
        // Move to the position of num field for row i
        NameSec.seekg(2 + i * 32 + 30, ios::beg); 
        NameSec.read((char*)&tmp, sizeof(tmp));
        tmp--;  // Decrement the number
        // Write the updated number back
        NameSec.seekp(-2, ios::cur);
        NameSec.write((char*)&tmp, sizeof(tmp));
	    }
		num--;
	}
	
	
}
void deletebyNameLL(short del_point,short num,short mid)
{
	fstream NameLink("LLIndexForName.txt", ios::binary | ios::in | ios::out);
	short CntNameLL=retLL();
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
    NameLink.close();
    decLL();
	num--;
	update_LL(del_point);
	if(num>0)
	{
		deletebyNameLL(del_point,num,mid);
	}
}
void deletebyName(char name[30])
{
	fstream NameSec("SecondaryIndexForName.txt", ios::binary | ios::in | ios::out);
	short CntNameSec=retSec();
	short same_elem_pointer,next_elem_pointer;
	short first = 0,  last = CntNameSec - 1, mid;;
	bool found = false;
	char temp[30];
	while (first <= last) { 
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
	short j;
	if (mid<retSec()-1)
	{
		NameSec.read((char*)&next_elem_pointer,sizeof(next_elem_pointer));
		j=next_elem_pointer-same_elem_pointer;
	}else
	{
		j=0;
	}
	
	if (!found)
        return ;


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
		if(j!=0)
		{
			update_sec(mid,j);
		}
        decSec();
        NameSec.close() ;
		deletebyNameLL(same_elem_pointer,j,mid);
}
void update_by_name(char name[30],char new_name[30])
{
	queue<string>doctorQ = search(name);
	deletebyName(name);
    while (!doctorQ.empty())
    {
		insertsortedname(new_name, (char*)&doctorQ.front());
		doctorQ.pop();
    }
}

// int main() {
//     cout << endl << "first" << endl;
//     char name1[30] = "ab";
//     char id1[15] = "10";
//     insertsortedname(name1, id1);

//     cout << endl << "second" << endl;
//     char name2[30] = "bc";
//     char id2[15] = "20";
//     insertsortedname(name2, id2);

//     cout << endl << "third" << endl;
//     char name3[30] = "cz";
//     char id3[15] = "30";
//     insertsortedname(name3, id3);

//     cout << endl << "fourth" << endl;
//     char name4[30] = "az";
//     char id4[15] = "40";
//     insertsortedname(name4, id4);

//     cout << endl << "fifth" << endl;
//     char name5[30] = "bb";
//     char id5[15] = "50";
//     insertsortedname(name5, id5);

// 	cout << endl << "sixth" << endl;
//     char name6[30] = "aa";
//     char id6[15] = "60";
//     insertsortedname(name6, id6);

// 	cout << endl << "seventh" << endl;
//     char name7[30] = "aa";
//     char id7[15] = "70";
//     insertsortedname(name7, id7);
	
// 	cout << endl << "eightth" << endl;
//     char name8[30] = "aa";
//     char id8[15] = "80";
//     insertsortedname(name8, id8);
	
// 	cout << endl << "ninth" << endl;
//     char name9[30] = "bc";
//     char id9[15] = "90";
//     insertsortedname(name9, id9);

// 	cout << endl << "tenth" << endl;
//     char name10[30] = "cz";
//     char id10[15] = "100";
//     insertsortedname(name10, id10);
	
// 	cout << endl << "eleventh" << endl;
//     char name11[30] = "z";
//     char id11[15] = "110";
//     insertsortedname(name11, id11);
// 	char s[30]="aa";
// 	deletebyName(s);
//     return 0;

// 	// char s[30]="aa";
// 	// queue<string> resultQueue = search(s);

//     // while (!resultQueue.empty()) {
//     //     std::cout << "Front element: " << resultQueue.front() << std::endl;
//     //     resultQueue.pop();
//     // }
// }
