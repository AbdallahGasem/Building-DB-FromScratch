#include <queue>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
int incSec()
{
	fstream File("SecondaryIndexForDoc_Id.txt",  ios::binary |ios::in | ios::out);
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
    fstream File("SecondaryIndexForDoc_Id.txt", ios::binary | ios::in | ios::out);
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
	fstream File("SecondaryIndexForDoc_Id.txt",  ios::binary |ios::in | ios::out);
	short num;
	File.read((char*)&num, sizeof(num));
	num--;
	File.seekg(0,ios::beg); 
	File.write((char*)&num, sizeof(num));
	return num;
}
int incLL()
{
	fstream File("LLIndexForDoc_Id.txt",  ios::binary |ios::in | ios::out);
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
    fstream File("LLIndexForDoc_Id.txt", ios::binary | ios::in | ios::out);
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
	fstream File("LLIndexForDoc_Id.txt",  ios::binary |ios::in | ios::out);
	short num;
	File.read((char*)&num, sizeof(num));
	num--;
	File.seekg(0,ios::beg); 
	File.write((char*)&num, sizeof(num));
	return num;
}
void insertLastRecord(char Doc_Id[15], char ID1[15])
{
	fstream Doc_IdSec("SecondaryIndexForDoc_Id.txt", ios::binary | ios::in | ios::out);
            Doc_IdSec.seekp(2+17*retSec(), ios::beg);
            Doc_IdSec.write(Doc_Id, 15);
			short point =retLL();
            Doc_IdSec.write((char*)&point, sizeof(point));
            incSec();
            Doc_IdSec.close();
            fstream Doc_IdLink("LLIndexForDoc_Id.txt", ios::binary | ios::in | ios::out);
            Doc_IdLink.seekg(2+19*retLL(),ios::beg);
			Doc_IdLink.write((char*)&point, sizeof(point));
            Doc_IdLink.write(ID1, 15);
            short nega=-1; // Fixed-size "-1"
            Doc_IdLink.write((char*)&nega, sizeof(nega));
			cout<<"i am here";
            Doc_IdLink.close();
            incLL();
}
void insertLastRecord(char Doc_Id[15], char ID1[15],short point)
{
	fstream Doc_IdSec("SecondaryIndexForDoc_Id.txt", ios::binary | ios::in | ios::out);
            Doc_IdSec.seekp(2+17*retSec(), ios::beg);
            Doc_IdSec.write(Doc_Id, 15);
            Doc_IdSec.write((char*)&point, sizeof(point));
            incSec();
            Doc_IdSec.close();
            fstream Doc_IdLink("LLIndexForDoc_Id.txt", ios::binary | ios::in | ios::out);
            Doc_IdLink.seekg(2+19*retLL(),ios::beg);
			Doc_IdLink.write((char*)&point, sizeof(point));
            Doc_IdLink.write(ID1, 15);
            short nega=-1; // Fixed-size "-1"
            Doc_IdLink.write((char*)&nega, sizeof(nega));
            Doc_IdLink.close();
            incLL();
}
void update_pointers(short point_num)
{
	short i=0,tmp;
	short sec_num=retSec();
	fstream Doc_IdSec("SecondaryIndexForDoc_Id.txt", ios::binary | ios::in | ios::out);
	Doc_IdSec.seekg(17,ios::beg);
	while(i<sec_num)
	{
		Doc_IdSec.read((char*)&tmp,sizeof(tmp));
		if(point_num<tmp)
		{
			Doc_IdSec.seekg(-2,ios::cur);
			tmp++;
			Doc_IdSec.write((char*)&tmp,sizeof(tmp));
		}
		Doc_IdSec.seekg(15,ios::cur);
		i++;
	}
}
void insertsortedDoc_Id(char Doc_Id[15], char ID1[15]) // Insert sorted in the Doc_Id file
{
    fstream Doc_IdSec("SecondaryIndexForDoc_Id.txt", ios::binary | ios::in | ios::out);
	short CntDoc_IdSec=retSec();
	short CntDoc_IdLL=retLL();
    short first = 0,  last = CntDoc_IdSec - 1, mid;
    bool found = false;
    char temp[15];
    // Binary search
	short same_elem_pointer;
	// short same_elem_place;
	while (first <= last) { // Use <= instead of <
    	mid = (first + last) / 2;
    	Doc_IdSec.seekg(mid * 17 + 2, ios::beg); // Read the Doc_Id at position mid
    	Doc_IdSec.read(temp, 15);
		// cout<<"temp = "<<temp<<" , Doc_Id = "<<Doc_Id<<endl;
    	if (strcmp(temp, Doc_Id) == 0) {
        	found = true;
			Doc_IdSec.read((char*)&same_elem_pointer,sizeof(same_elem_pointer));
    	    break; // Exit the loop if a match is found
	    } else if (strcmp(temp, Doc_Id) > 0) {
        	last = mid - 1; // Search in the left half
    	} else {
   	    	first = mid + 1; // Search in the right half
    	}
	}
	Doc_IdSec.close();
    if (!found) {
        if(CntDoc_IdLL==0)
        {
			insertLastRecord(Doc_Id,ID1);
        }
		else
        {
			fstream Doc_IdSec("SecondaryIndexForDoc_Id.txt", ios::binary | ios::in | ios::out);
            Doc_IdSec.seekg(2, ios::beg);
			int i = 0 ;
            short offplace = -1;
			short bigger_pointer;
			while (i < CntDoc_IdSec){ /// this loop to get the place    ERROR here
				char s[15];
				Doc_IdSec.read(s, sizeof(s));
				if (strcmp(Doc_Id, s) < 0){
					offplace = Doc_IdSec.tellg();
					Doc_IdSec.read((char*)&bigger_pointer,sizeof(bigger_pointer));
					offplace -= 15;
					// cout<<"offplace = "<<offplace<<endl<<"bigger_pointer = "<<bigger_pointer;
					break;
				}
				else{
					Doc_IdSec.seekg(2, ios::cur);
				}
				i++ ;
			}
			Doc_IdSec.close();
			if (offplace == -1){ ///it is the biggest  one
				insertLastRecord(Doc_Id,ID1);
			}
			else{ /// it is some where in th middle
				fstream Doc_IdSec("SecondaryIndexForDoc_Id.txt", ios::binary | ios::in | ios::out);
				fstream Doc_IdLink("LLIndexForDoc_Id.txt", ios::binary | ios::in | ios::out);
    			// Read the last record in secondary index file and ll file
				char endDoc_Id[15],endID[15];
				short endpointer;
				Doc_IdSec.seekg(2+17*(retSec()-1),ios::beg);
				Doc_IdSec.read(endDoc_Id,15);
				Doc_IdSec.read((char*)&endpointer,sizeof(endpointer));
				Doc_IdLink.seekg(4+19*(retLL()-1),ios::beg);
				Doc_IdLink.read(endID,15);
				// Shift records
				char tmpDoc_Id1[15],tmpDoc_Id2[15],tmpID1[15],tmpID2[15];
				short tmpPoint1,tmpPoint2,tmpNext1,tmpNext2;
				Doc_IdSec.seekg(offplace,ios::beg);
				Doc_IdSec.read(tmpDoc_Id1,15);
				Doc_IdSec.read((char*)&tmpPoint1,sizeof(tmpPoint1));
				while(Doc_IdSec.read(tmpDoc_Id2, 15) && Doc_IdSec.read((char*)&tmpPoint2, sizeof(tmpPoint2)))
				{

					Doc_IdSec.seekg(-17,ios::cur);
					Doc_IdSec.write(tmpDoc_Id1,15);
					tmpPoint1++;
					Doc_IdSec.write((char*)&tmpPoint1,sizeof(tmpPoint1));
					memcpy(tmpDoc_Id1, tmpDoc_Id2, 15);
					tmpPoint1=tmpPoint2;
				}
				Doc_IdLink.seekg(4+bigger_pointer*19,ios::beg);
				Doc_IdLink.read(tmpID1,15);
				Doc_IdLink.read((char*)&tmpNext1,sizeof(tmpNext1));
				while(Doc_IdLink.good())
				{
					Doc_IdLink.seekg(2,ios::cur);
					Doc_IdLink.read(tmpID2,15);
					Doc_IdLink.read((char*)&tmpNext2,sizeof(tmpNext2));
					Doc_IdLink.seekg(-17,ios::cur);
					Doc_IdLink.write(tmpID1,15);
					if(tmpNext1!=-1)
					{
						tmpNext1++;
					}
					Doc_IdLink.write((char*)&tmpNext1,sizeof(tmpNext1));
					memcpy(tmpID1, tmpID2, 15);
					tmpNext1=tmpNext2;
				}
				Doc_IdSec.close();
				Doc_IdLink.close();
				Doc_IdSec.open("SecondaryIndexForDoc_Id.txt", ios::binary | ios::in | ios::out);
				Doc_IdLink.open("LLIndexForDoc_Id.txt", ios::binary | ios::in | ios::out);
				// Write the new record at offplace
				Doc_IdSec.seekp(offplace,ios::beg);
				Doc_IdSec.write(Doc_Id,15);
				Doc_IdLink.seekg(4+bigger_pointer*19,ios::beg);
				Doc_IdLink.write(ID1,15);
				short nega=-1; // Fixed-size "-1"
            	Doc_IdLink.write((char*)&nega, sizeof(nega));
				Doc_IdSec.close();
				Doc_IdLink.close();
				// Append the last record back to the end
				insertLastRecord(endDoc_Id,endID,endpointer+1);
			}
		}
	}
	else if(found&&same_elem_pointer<retLL()-1)
	{
		update_pointers(same_elem_pointer);
			fstream Doc_IdLink("LLIndexForDoc_Id.txt", ios::binary | ios::in | ios::out);
			//update next pointer of the similar Doc_Id
			same_elem_pointer++;
			Doc_IdLink.seekg(same_elem_pointer*19,ios::beg);
			short Next_point,Next_order;
			Doc_IdLink.read((char*)&Next_point,sizeof(Next_point));
			while(Next_point!=-1)
			{
				Doc_IdLink.seekg(17,ios::cur);
				same_elem_pointer++;
				Doc_IdLink.read((char*)&Next_point,sizeof(Next_point));
			}
			Doc_IdLink.read((char*)&Next_order,sizeof(Next_order));
			Doc_IdLink.seekg(-4,ios::cur);
			Doc_IdLink.write((char*)&Next_order,sizeof(Next_order));
    		// Read the last record in secondary index file and ll file
				char endID[15];
				Doc_IdLink.seekg(4+19*(retLL()-1),ios::beg);
				Doc_IdLink.read(endID,15);
				// Shift records
				char tmpID1[15],tmpID2[15];
				short tmpNext1,tmpNext2;
				Doc_IdLink.seekg(4+same_elem_pointer*19,ios::beg);
				Doc_IdLink.read(tmpID1,15);
				Doc_IdLink.read((char*)&tmpNext1,sizeof(tmpNext1));
				while(Doc_IdLink.good())
				{
					Doc_IdLink.seekg(2,ios::cur);
					Doc_IdLink.read(tmpID2,15);
					Doc_IdLink.read((char*)&tmpNext2,sizeof(tmpNext2));
					Doc_IdLink.seekg(-17,ios::cur);
					Doc_IdLink.write(tmpID1,15);
					if(tmpNext1!=-1)
					{
						tmpNext1++;
					}
					Doc_IdLink.write((char*)&tmpNext1,sizeof(tmpNext1));
					memcpy(tmpID1, tmpID2, 15);
					tmpNext1=tmpNext2;
				}
				Doc_IdLink.close();
				Doc_IdLink.open("LLIndexForDoc_Id.txt", ios::binary | ios::in | ios::out);
				// Write the new record at offplace
				Doc_IdLink.seekg(4+same_elem_pointer*19,ios::beg);
				Doc_IdLink.write(ID1,15);
				short nega=-1; // Fixed-size "-1"
            	Doc_IdLink.write((char*)&nega, sizeof(nega));
				//write the last record
            	Doc_IdLink.seekg(2+19*retLL(),ios::beg);
				Doc_IdLink.write((char*)&CntDoc_IdLL, sizeof(CntDoc_IdLL));
            	Doc_IdLink.write(endID, 15);
            	Doc_IdLink.write((char*)&nega, sizeof(nega));
            	Doc_IdLink.close();
            	incLL();
	}else
	{
			update_pointers(same_elem_pointer);
			fstream Doc_IdLink("LLIndexForDoc_Id.txt", ios::binary | ios::in | ios::out);
			//update next pointer of the similar Doc_Id
			Doc_IdLink.seekg(2+19*retLL()-2,ios::beg);
			Doc_IdLink.write((char*)&CntDoc_IdLL,sizeof(CntDoc_IdLL));
			Doc_IdLink.write((char*)&CntDoc_IdLL,sizeof(CntDoc_IdLL));
			Doc_IdLink.write(ID1,15);
			short nega=-1; // Fixed-size "-1"
            Doc_IdLink.write((char*)&nega, sizeof(nega));
            	Doc_IdLink.close();
            	incLL();
	}
}
queue<string>  searchSecondaryApp(char Doc_Id[15])
{
	fstream Doc_IdSec("SecondaryIndexForDoc_Id.txt", ios::binary | ios::in | ios::out);
	short CntDoc_IdSec=retSec();
	short same_elem_pointer;
	short first = 0,  last = CntDoc_IdSec - 1, mid;;
	bool found = false;
	char temp[15];
	while (first <= last) { // Use <= instead of <
    	mid = (first + last) / 2;
    	Doc_IdSec.seekg(mid * 17 + 2, ios::beg); // Read the Doc_Id at position mid
    	Doc_IdSec.read(temp, 15);
		// cout<<"temp = "<<temp<<" , Doc_Id = "<<Doc_Id<<endl;
    	if (strcmp(temp, Doc_Id) == 0) {
        	found = true;
			Doc_IdSec.read((char*)&same_elem_pointer,sizeof(same_elem_pointer));
    	    break; // Exit the loop if a match is found
	    } else if (strcmp(temp, Doc_Id) > 0) {
        	last = mid - 1; // Search in the left half
    	} else {
   	    	first = mid + 1; // Search in the right half
    	}
	}
	Doc_IdSec.close();
	queue<string> ids;
	if(!found)
	{
		cout<<"element not found";
	}else
	{
		short Next_point;
		fstream Doc_IdLink("LLIndexForDoc_Id.txt", ios::binary | ios::in | ios::out);
		Doc_IdLink.seekg(4+same_elem_pointer*19,ios::beg);
		char cur_id[15];
		Doc_IdLink.read(cur_id,15);
		Doc_IdLink.read((char*)&Next_point,sizeof(Next_point));
		string s= string(cur_id);
		ids.push(s);
		while(Next_point!=-1)
		{
			Doc_IdLink.seekg(2,ios::cur);
			Doc_IdLink.read(cur_id,15);
			s= string(cur_id);
			ids.push(s);
			Doc_IdLink.read((char*)&Next_point,sizeof(Next_point));
		}
	}
		return ids;
}
void update_LL(short del_point)
{
	fstream Doc_IdLink("LLIndexForDoc_Id.txt", ios::binary | ios::in | ios::out);
	short x=0,tmp;
	Doc_IdLink.seekg(2,ios::beg);
	for(short i=0;i<retLL();i++)
	{
		Doc_IdLink.write((char*)&x , sizeof(x));
        Doc_IdLink.seekg(17,ios::cur);
		x++;
	}
	short curRecord,Next_point;
	Doc_IdLink.seekg(del_point*19+2,ios::beg);
	Doc_IdLink.read((char*)&curRecord , sizeof(curRecord));
	Doc_IdLink.seekg(15,ios::cur);
	for(int i=0;i<retLL()-del_point;i++)
	{
		Doc_IdLink.read((char*)&Next_point , sizeof(Next_point));		
		if(Next_point!=-1)
		{
			Doc_IdLink.seekg(-2,ios::cur);
			curRecord++;
			Doc_IdLink.write((char*)&curRecord , sizeof(curRecord));		
		}
		Doc_IdLink.read((char*)&curRecord , sizeof(curRecord));
        Doc_IdLink.seekg(17,ios::cur);
	}
}
void deletebyDoc_IdLL(short del_point,short num,short mid)
{
	fstream Doc_IdLink("LLIndexForDoc_Id.txt", ios::binary | ios::in | ios::out);
	short CntDoc_IdLL=retLL();
	Doc_IdLink.seekg(del_point*19+4+19, ios::beg);
	short x=del_point;
	while(x < CntDoc_IdLL)              /// start to shift
	{
        char tempID[15];
        short tempof;
        Doc_IdLink.read(tempID , 15);
        Doc_IdLink.read((char*)&tempof , sizeof(tempof)) ;
        Doc_IdLink.seekg(-38+2,ios::cur) ;
        Doc_IdLink.write(tempID , 15);
        Doc_IdLink.write((char*)&tempof , sizeof(tempof)) ;
        Doc_IdLink.seekg(19,ios::cur) ;
        x++;
    }
    Doc_IdLink.close() ;
    decLL();
	num--;
	update_LL(del_point);
	if (num>0)
	{
		deletebyDoc_IdLL(del_point,num,mid);
	}
}
void update_sec(short mid,short num)
{
	fstream Doc_IdSec("SecondaryIndexForDoc_Id.txt", ios::binary | ios::in | ios::out);
	while(num>0)
	{
		short i=mid;
    	short tmp = 0;
    	for (short i = mid; i < retSec() - 1; i++) {
        // Move to the position of num field for row i
        Doc_IdSec.seekg(2 + i * 17 + 15, ios::beg); 
        Doc_IdSec.read((char*)&tmp, sizeof(tmp));
        tmp--;  // Decrement the number
        // Write the updated number back
        Doc_IdSec.seekp(-2, ios::cur);
        Doc_IdSec.write((char*)&tmp, sizeof(tmp));
	    }
		num--;
	}
	
}
void deletebyDoc_Id(char Doc_Id[15])
{
	fstream Doc_IdSec("SecondaryIndexForDoc_Id.txt", ios::binary | ios::in | ios::out);
	short CntDoc_IdSec=retSec();
	short same_elem_pointer,next_elem_pointer;
	short first = 0,  last = CntDoc_IdSec - 1, mid;;
	bool found = false;
	char temp[15];
	while (first <= last) {
    	mid = (first + last) / 2;
    	Doc_IdSec.seekg(mid * 17 + 2, ios::beg); // Read the Doc_Id at position mid
    	Doc_IdSec.read(temp, 15);
    	if (strcmp(temp, Doc_Id) == 0) {
        	found = true;
			Doc_IdSec.read((char*)&same_elem_pointer,sizeof(same_elem_pointer));
    	    break; // Exit the loop if a match is found
	    } else if (strcmp(temp, Doc_Id) > 0) {
        	last = mid - 1; // Search in the left half
    	} else {
   	    	first = mid + 1; // Search in the right half
    	}
	}
	Doc_IdSec.seekg(15, ios::cur);
	short j;
	if (mid<retSec()-1)
	{
		Doc_IdSec.read((char*)&next_elem_pointer,sizeof(next_elem_pointer));
		j=next_elem_pointer-same_elem_pointer;
	}else
	{
		j=0;
	}
	
	if (!found)
        return ;

        Doc_IdSec.seekg(mid*17+17 +2, ios::beg);

        int i = mid/17 ;
        while(i < CntDoc_IdSec-1)               /// start to shift
        {
            char tempDoc_Id[15] ;
            short tempof ;
            Doc_IdSec.read(tempDoc_Id , 15) ;
            Doc_IdSec.read((char*)&tempof , sizeof(tempof)) ;
            Doc_IdSec.seekg(-34,ios::cur) ;
            Doc_IdSec.write(tempDoc_Id , 15) ;
            Doc_IdSec.write((char*)&tempof , sizeof(tempof)) ;
            Doc_IdSec.seekg(17,ios::cur) ;
            i++ ;
        }
		if(j!=0)
		{
			update_sec(mid,j);
		}
        decSec();
        Doc_IdSec.close() ;
		deletebyDoc_IdLL(same_elem_pointer,j,mid);
}
void update_by_name(char Doc_Id[15],char new_Doc_Id[15])
{
	queue<string>doctorQ = searchSecondaryApp(Doc_Id);
	deletebyDoc_Id(Doc_Id);
    while (!doctorQ.empty())
    {
		insertsortedDoc_Id(new_Doc_Id, (char*)&doctorQ.front());
		doctorQ.pop();
    }
}


// int main() {

//     cout << endl << "first" << endl;
//     char Doc_Id1[15] = "ab";
//     char id1[15] = "10";
//     insertsortedDoc_Id(Doc_Id1, id1);

//     cout << endl << "second" << endl;
//     char Doc_Id2[15] = "bc";
//     char id2[15] = "20";
//     insertsortedDoc_Id(Doc_Id2, id2);

//     cout << endl << "third" << endl;
//     char Doc_Id3[15] = "cz";
//     char id3[15] = "30";
//     insertsortedDoc_Id(Doc_Id3, id3);

//     cout << endl << "fourth" << endl;
//     char Doc_Id4[15] = "az";
//     char id4[15] = "40";
//     insertsortedDoc_Id(Doc_Id4, id4);

//     cout << endl << "fifth" << endl;
//     char Doc_Id5[15] = "bb";
//     char id5[15] = "50";
//     insertsortedDoc_Id(Doc_Id5, id5);

// 	cout << endl << "sixth" << endl;
//     char Doc_Id6[15] = "aa";
//     char id6[15] = "60";
//     insertsortedDoc_Id(Doc_Id6, id6);

// 	cout << endl << "seventh" << endl;
//     char Doc_Id7[15] = "aa";
//     char id7[15] = "70";
//     insertsortedDoc_Id(Doc_Id7, id7);
	
// 	cout << endl << "eightth" << endl;
//     char Doc_Id8[15] = "aa";
//     char id8[15] = "80";
//     insertsortedDoc_Id(Doc_Id8, id8);
	
// 	cout << endl << "ninth" << endl;
//     char Doc_Id9[15] = "bc";
//     char id9[15] = "90";
//     insertsortedDoc_Id(Doc_Id9, id9);

// 	cout << endl << "tenth" << endl;
//     char Doc_Id10[15] = "cz";
//     char id10[15] = "100";
//     insertsortedDoc_Id(Doc_Id10, id10);
	
// 	cout << endl << "eleventh" << endl;
//     char Doc_Id11[15] = "z";
//     char id11[15] = "110";
//     insertsortedDoc_Id(Doc_Id11, id11);
//     return 0;
// // 	// char s[15]="aa";
// // 	// deletebyDoc_Id(s);

// // 	// char s[15]="aa";
// // 	// queue<string> resultQueue = search(s);

// //     // while (!resultQueue.empty()) {
// //     //     std::cout << "Front element: " << resultQueue.front() << std::endl;
// //     //     resultQueue.pop();
// //     // }
// }
