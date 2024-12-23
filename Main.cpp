//                                                         بسم الله الرحمن الرحيم                                                         //
// program: main program
// Description: Healthcare Management System
// Author:  Abdallah Gasem, Ahmed Karam, Abdelrahman Lotfy, Rawan Alnaggary
// Date: 06-12-2024
// Version: 1.0
/* file run command: g++ -o main main.cpp; if ($?) { ./main } */
// ------------------------------------------------------------------------------------------------------------------------------ //

#include <bits/stdc++.h>
#include "AppFuns.cpp"
using namespace std;

const string doctorsFile = "Doctors.txt", docPrimaryIndexFile = "DocPrimaryIndex.txt";
const string appointmentsFile = "Appointments.txt", appPrimaryIndexFile = "AppPrimaryIndex.txt";

void initializeFiles()
{
    docFile.open(doctorsFile, ios::in | ios::out);
    if (!docFile.is_open())
    {
        docFile.open(doctorsFile, ios::out);
        docFile << "-1\n";
        docFile.close();
        docFile.open(doctorsFile, ios::in | ios::out);
    }

    appFile.open(appointmentsFile, ios::in | ios::out);
    if (!appFile.is_open())
    {
        appFile.open(appointmentsFile, ios::out);
        appFile << "-1\n";
        appFile.close();
        appFile.open(appointmentsFile, ios::in | ios::out);
    }

    appPrimInFile.open(appPrimaryIndexFile, ios::in | ios::out | ios::binary);
    docPrimInFile.open(docPrimaryIndexFile, ios::in | ios::out | ios::binary);
}

//__________________________________________________________Query Handling_______________________________________________________//

// 1- Query Parsing
//helper
/**
 * @brief Splits a given string into substrings based on a specified delimiter.
 *
 * This function takes a target string and a delimiter character, and splits the
 * target string into substrings wherever the delimiter character is found. The
 * resulting substrings are stored in a vector of strings.
 *
 * @param target The string to be split.
 * @param dilmeter The character used as the delimiter to split the string.
 * @return A vector of strings containing the substrings obtained by splitting
 *         the target string by the delimiter.
 */
// vector <string> split( string target , char delimiter )
// {
//     // declaring the vector that holds the substrings and a substr var
//     vector<string>splittedVec;
//     string substr = "" ;
//     int len = target.size() ;


//     for (int i = 0 ; i < len ; i++)
//     {
//         // appending charachters till meeting the delimiter :

//         if (target[i] != delimiter)
//         {
//             substr.push_back(char(target[i])) ;
//         }

//             // pushing the splitted substr as it has met the delimiter :
//         else
//         {
//             splittedVec.push_back(substr) ;
//             substr.clear() ;
//         }

//     }

//     // including the last substring in the vector
//     splittedVec.push_back(substr) ;

//     return splittedVec ;

// }

/**
 * @brief Parses a SQL-like query string into its components: SELECT, FROM, and WHERE parameters.
 *
 * This function takes a query string, converts it to lowercase to maintain case insensitivity,
 * splits the query into individual words, and then extracts the parameters for the SELECT, FROM,
 * and WHERE clauses. The parameters are returned as a vector of vectors of strings.
 *
 * @param query The SQL-like query string to be parsed.
 * @return A vector of vectors of strings, where:
 *         - The first vector contains the SELECT parameters.
 *         - The second vector contains the FROM parameters.
 *         - The third vector contains the WHERE parameters.
 */
vector<vector<string>> queryParser( string query ){

    /* maintaining case insenstivity */
    // std::transform: Applies a function to each character in the string.
    // str.begin() and str.end() define the input range.
    // The result is stored back in the original string (str.begin() as the output iterator).
    // ::tolower is the transformation function that converts each character to lowercase.
    transform(query.begin(), query.end(), query.begin(), ::tolower);    // or use for each by refrence to access each char in the string and apply tolower()!

    /* slicing */
    vector<string> slicedQuery;
    int fromIdx , whereIdx;
    slicedQuery = split(query,' ');     // note that slicedQuery[0] = select , always!

    // input validation
    if (slicedQuery.size() < 6)     // select <....> from <....> where <....>
    {
        cerr << "ERROR! Query Parsing, i/p Invalid Query Strucutre" << endl;
        return {};
    }



    /* Grooming */
    // distance(s.begin(), it): This function calculates the number of elements between
    // the beginning of the container s (s.begin()) and the iterator it (which points to the found element).
    // This effectively gives the index of the found element.

    // finding the index of 'from'
    auto fIt = find(slicedQuery.begin(), slicedQuery.end(),"from");
    if (fIt == slicedQuery.end()) {
        cerr << "Error: 'from' keyword not found in query.\n";
        return {};
    }
    fromIdx = distance(slicedQuery.begin(), fIt);

    // finding the index of 'where'
    auto wIt = find(slicedQuery.begin(), slicedQuery.end(),"where");
    if (wIt == slicedQuery.end()) {
        cerr << "Error: 'where' keyword not found in query.\n";
        return {};
    }
    whereIdx = distance(slicedQuery.begin(), wIt);

    // select paramaters
    vector<string> selectParams;
    for (int i = 1 ; i < fromIdx ; i++)     // start from one 'select' resides in slicedQuery[0]
    {
        selectParams.push_back(slicedQuery[i]);
    }

    // from paramaters
    vector<string> fromParams;
    for (int i = fromIdx + 1 ; i < whereIdx ; i++)
    {
        fromParams.push_back(slicedQuery[i]);
    }

    // where parameters : later need to handle the spaces between condition atrribute and condition value
    // now (attr=value) is a must with no spaces between!
    vector<string> whereParams;
    for (int i = whereIdx + 1; i < slicedQuery.size() ; i++)
    {
        whereParams.push_back(slicedQuery[i]);
    }

    // main vector the Executer will work on
    vector<vector<string>> mainVector(3);
    mainVector = {selectParams, fromParams, whereParams};

    /*----------------- Debugging --------------------*/

    // cout << "select parameters, mainvector[0]:\n";
    // cout << "{";
    // for (int i = 0; i < selectParams.size(); i++)
    // {
    //     cout << selectParams[i] << ",";
    // }
    // cout << "}\n";
    // cout << "from parameters, mainvector[1]:\n";
    // cout << "{";
    // for (int i = 0; i < fromParams.size(); i++)
    // {
    //     cout  << fromParams[i] << ",";
    // }
    // cout << "}\n";
    // cout << "where parameters, mainvector[2]:\n";
    // cout << "{";
    // for (int i = 0; i < whereParams.size(); i++)
    // {
    //     cout << whereParams[i] << ",";
    // }
    // cout << "}\n";
    /*----------------- Debugging --------------------*/


    // query structure validation
    if (mainVector.size() < 2 || mainVector[1].empty() || mainVector[0].empty()) {
        cerr << "ERROR! Query Parsing, mainVector Invalid Strucutre" << endl;
        return {};
    }

    return mainVector;

}

// 2- Query validation
// helper
/**
 * @brief Splits a string into two parts based on the first occurrence of a delimiter.
 *
 * @param target The string to be split.
 * @param delimiter The character used to split the string.
 * @param frstPt If true, returns the part of the string before the delimiter;
 *               if false, returns the part of the string after the delimiter. Default is true.
 * @return std::string The substring based on the value of frstPt.
 */
string splitPart(string target, char delimiter , bool frstPt = true)
{
    string substr1 = "";
    string substr2 = "";
    int delIdx = 0;

    for (char ch : target)
    {
        if (ch != delimiter)
        {
            substr1.push_back(ch);
            delIdx++;
        }
        else
        {
            break;
        }
    }

    if (!frstPt)
    {
        for (int i = delIdx + 1 ; i < target.length() ; i++)
        {
            substr2.push_back(target[i]);
        }

        return substr2;

    }

    return substr1;
}


/**
 * @brief Validates if the given table name exists in the statistics file.
 *
 * This function reads the "statistics.txt" file to check if the provided table name
 * exists within the file. It returns true if the table name is found, otherwise false.
 *
 * @param tableName The name of the table to validate.
 * @return true if the table name exists in the statistics file, false otherwise.
 *
 * @note The function will print an error message if the file cannot be opened or if the table name is invalid.
 * @note The function catches and prints any exceptions that occur during execution.
 */
bool validateTable(string tableName){
    string table;
    vector<string>tables;

    try
    {
        fstream stats("statistics.txt",ios::in);
        if (!stats.is_open())
        {
            cerr << "ERROR! Validating Table, File Didnt Open...\n";
        }
        stats.clear();

        while (getline(stats,table))
        {
            table = splitPart(table,'|');
            tables.push_back(table);
        }

        stats.close();

        for (int i = 0; i < tables.size(); i++)
        {
            if (tables[i] == tableName)
            {
                return true;
            }

        }

    }
    catch(const exception& e)
    {
        cerr << "\nvalidateTableFunctionException:\n";
        cerr << e.what() << '\n';
    }


    cout << "Invalid Table Name!\n";
    return false;
}


/**
 * @brief Validates the attributes of a given table by checking against a statistics file.
 *
 * This function reads the table data from a statistics file and checks if the provided attributes
 * exist in the specified table. It supports wildcard attributes "all" and "*".
 *
 * @param tableName The name of the table to validate attributes for.
 * @param attributes A vector of attribute names to validate.
 * @return true if all attributes are valid, false otherwise.
 *
 * @note The function expects a statistics file named "statistics.txt" in the same directory.
 *
 * @throws std::exception if an error occurs during file operations.
 */
bool validateAttr(string tableName , vector<string>attributes ){

    string table;
    vector<string>tables;
    vector<string>splittedTargetTable;
    int targetTableIdx;
    bool existFlag = false;

    try
    {
        // retreive tables data from the stat file
        fstream stats("statistics.txt",ios::in);
        if (!stats.is_open())
        {
            cerr << "ERROR! Validating Attributes, File Didnt Open...\n";
        }

        while (getline(stats,table))
        {
            tables.push_back(table);
        }
        stats.close();

        // triming table names leaving attributes only


        // accessing target table
        auto it = find_if(tables.begin(), tables.end(), [&tableName](const string& element) {
            return element.find(tableName+'|') != string::npos; // Check if the element starts with "table|"
        });

        if (it == tables.end()) {
            cout << "ERROR! Validating attributes, Target Table not found!\n";
            return false;
        }

        targetTableIdx = distance(tables.begin(), it);

        // split target table to access attributes
        splittedTargetTable = split(tables[targetTableIdx],'|');

        // matching all atrributes with the retrieved ones from stat file
        for (int i = 0; i < attributes.size(); i++)
        {
            if (attributes[i] == "all" || attributes[i] == "*")
            {
                existFlag = true;
                continue;
            }

            else if (splittedTargetTable[1].find(attributes[i]) != string:: npos )
            {
                existFlag = true;
            }

            else
            {
                // as we need all attributes to match!
                existFlag = false;
                cout << "there is no attribute " << "(" << attributes[i] << ")" << " " << "in table " << "(" << tableName << ")" << endl;
                break;
            }
        }

        if (existFlag)
        {
            return true;
        }
        else
        {
            cout << "ERROR! Invalid attribute! \n";
        }

    }
    catch(const exception& e)
    {
        cerr << "\nvalidateAttributesFunctionException:\n";
        cerr << e.what() << '\n';
    }


    return false;
}


/**
 * @brief Validates a query by checking the table name and its attributes.
 *
 * This function validates a query by ensuring that the specified table name
 * exists and that the provided attributes are valid for that table.
 *
 * @param tableName The name of the table to validate.
 * @param attributes A vector of attribute names to validate against the table.
 * @return true if both the table name and attributes are valid, false otherwise.
 */
bool queryValidator(string tableName , vector<string>attributes )
{
    return validateTable(tableName) && validateAttr(tableName , attributes);
}


// 3- executing the query [dispatching]
void queryExecuter(string query){

    // mainVector[0] --> select paramters (attributes) can take all \ *
    // mainVector[1] --> from paramters (tables) , the current logic operate on only a table at a time [you must devolop a logic to handle joins later]
    // mainVector[2] --> where paramters (conditions)

    /* parse the query */
    vector<vector<string>> mainVector = queryParser(query);

    // i/p Validation
    if(mainVector.size() < 3)
    {
        cerr << "--- the program aborted by the Parser due to invalid query structure! ---\n";
        return;
    }

    /* Validation */
    string tableName =  mainVector[1][0];
    vector<string> attrs = mainVector[0];

    if (!queryValidator(tableName, attrs)) {
        cerr << "--- the program aborted by the validator! ---\n";
        return;
    }

    /* Dispatching */
    // from , where, select
    // basically select is a search statment, therefore we need to
    //      - what table we are searching for?
    //      - what attributes do we want to retrieve ?
    //      - is there an attribute that has an index? use the index : search sequentially
    string targetTable = mainVector[1][0];      // from
    vector<string> conditions = mainVector[2];  // where
    vector<string> selectAtrributes = mainVector[0];  // select

    string condAttribute, condAttributeValue;
    docFile.open(doctorsFile, ios::in | ios::binary);
    appFile.open(appointmentsFile, ios::in | ios::binary);
    condAttribute = splitPart(conditions[0],'=');
    condAttributeValue = splitPart(conditions[0],'=',false);


    /* note that the query is lowercased so make any comparisons in the lowercase discarding the typing convention(pascal) */

    if (targetTable == "doctors")
    {
        // Select all from Doctors where Doctor ID=’xxx’;   // use pk
        if (selectAtrributes[0] == "all" || selectAtrributes[0] == "doctorid,doctorname,address" || selectAtrributes[0] == "*")
        {
            if (condAttribute == "doctorid")
            {
                /*------------------------------ Debugging ------------------------------*/
                // cout << "we are in the right place the search function isnt working!\n";
                // cout << "targetTable: " << targetTable << endl;
                // cout << "the Select param is: " << selectAtrributes[0] << endl;
                // cout << "the condition attribute is: " << condAttribute << " and its value = " << condAttributeValue << endl;
                /*------------------------------ Debugging ------------------------------*/


                // use primary index primary index //
                cout << "\n_____________________________________________\n";
                // print info
                printDoctorInfo(docFile,condAttributeValue);
                cout << "_____________________________________________\n";
            }
            
        }

        // Select all from Doctors where Doctorname=’xxx’;
        else if ( selectAtrributes[0] == "all" && condAttribute == "doctorname" ) {
            // use secondary index  //
            queue<string>doctorQ = searchSecondarydoc(const_cast<char*>(condAttributeValue.c_str()));

            cout << "\n_____________________________________________\n";
            while (!doctorQ.empty())
            {
                printDoctorInfo(docFile,doctorQ.front());
                doctorQ.pop();
                cout << endl;
            }
            cout << "_____________________________________________\n";

        }

        // Select doctorname from Doctors where Doctorid=’xxx’;
        else if ( selectAtrributes[0] == "doctorname" && condAttribute == "doctorid") {
            // use secondary index  //  condAttribute = doctorId

            cout << "\n_____________________________________________\n";
            cout << GetDoctorName(docFile,condAttributeValue) << endl;
            cout << "_____________________________________________\n";
           
        }

        else
        {
            // search sequentially
        }

    }

    else if (targetTable == "appointments")
    {
        // Select all from Appointments where appointmentID=’xxx’;  // use pk
        if (selectAtrributes[0] == "all" || selectAtrributes[0] == "appointmentid,appointmentdate,doctorid" || selectAtrributes[0] == "*")
        {
            if (condAttribute == "appointmentid")
            {

                /*------------------------------ Debugging ------------------------------*/
                // cout << "we are in the right place the search function isnt working!\n";
                // cout << "targetTable: " << targetTable << endl;
                // cout << "the Select param is: " << selectAtrributes[0] << endl;
                // cout << "the condition attribute is: " << condAttribute << endl;
                /*------------------------------ Debugging ------------------------------*/

                // use primary index primary index //
                cout << "\n_____________________________________________\n";
                printAppointmentInfo(appFile,condAttribute);
                cout << "_____________________________________________\n";
            }
            
        }

        else if ( selectAtrributes[0] == "all" && condAttribute == "doctorid" ) {
            
            // use secondary index  //
            queue<string>appointmentQ = searchSecondaryApp(const_cast<char*>(condAttributeValue.c_str()));
            while (!appointmentQ.empty())
            {
                printAppointmentInfo(docFile,appointmentQ.front());
                appointmentQ.pop();
            }
        }

        else if ( condAttribute == "appointmentid" && selectAtrributes[0] == "doctorid" ) {
            
            cout << "\n_____________________________________________\n";
            // cout << GetAppName(docFile,condAttributeValue) << endl;
            cout << "_____________________________________________\n";
        }

        else
        {
            // search sequentially
        }

    }

    docFile.close();
    appFile.close();

}

//__________________________________________________________Launcher_____________________________________________________________//

void launcher()
{

    //DRAFT START
    AvailList doctorsAvailList;
    doctorsAvailList.initialize(docFile);
    AvailList appointmentAvailList;
    appointmentAvailList.initialize(appFile);
    //DRAFT END

    int option = 0;

    // Doctor data
    string doctorId;
    string doctorName;
    string doctorNewName;   // For option-3
    string address;

    // Appointment data
    string appointmentId;
    string appointmentDate;
    string appointmentNewDate; // For option-4

    while (true)
    {
        // Menu
        cout<<"\nPlease Choose an Operation from the Following: \n";
        cout<<"1)  Add New Doctor \n";
        cout<<"2)  Add New Appointment \n";
        cout<<"3)  Update Doctor Name (Doctor ID) \n";
        cout<<"4)  Update Appointment Date (Appointment ID) \n";
        cout<<"5)  Delete Appointment (Appointment ID) \n";
        cout<<"6)  Delete Doctor (Doctor ID) \n";
        cout<<"7)  Print Doctor Info (Doctor ID) \n";
        cout<<"8)  Print Appointment Info (Appointment ID) \n";
        cout<<"9)  Write Query \n";
        cout<<"10) Read Doctors File \n";
        cout<<"11) Read Appointments File \n";
        cout<<"12) EXIT \n";
        cout<<">>";
        cin>>option;

        switch (option)
        {
            case 1:
            {
                /* Add New Doctor */

                cout<<"Enter Dr ID, Name & address respectively:\n";
                cin>>doctorId;
                cin.ignore(); // Clear the newline character left in the input buffer
                getline(cin, doctorName); // Read the full line for doctorName
                getline(cin, address);    // Read the full line for address
                Doctor doctor(doctorId, doctorName, address);
                addDoctor(docFile, doctorsAvailList,doctor );

                break;
            }

            case 2:
            {
                /* Add New Appointment */

                cout<<"Enter AppointmentId, date  & Doctor Id respectively:\n";
                cin>>appointmentId;
                cin.ignore(); // Clear the newline character left in the input buffer
                getline(cin, appointmentDate); // Read the full line for doctorName
                getline(cin, doctorId);       // Read the full line for address
                Appointment appointment(appointmentId, appointmentDate, doctorId);
                addAppointment (appFile, appointmentAvailList, appointment);

                break;
            }

            case 3:
            {
                /* Update Doctor Name (Doctor ID) */

                cout<<"Please enter the Doctor Id followed by the new name for the Doctor to be updated:\n";
                cin>>doctorId;
                cin.ignore();
                getline(cin, doctorNewName);
                updateDoctorName(docFile, doctorId, doctorNewName);

                break;
            }

            case 4:
            {
                /* Update Appointment Date (Appointment ID) */

                cout<<"Please enter the Appointment Id followed by the new Date for the appointment:\n";
                cin>>appointmentId;
                cin.ignore();
                getline(cin, appointmentNewDate);
                updateAppointmentDate(appFile, appointmentId, appointmentNewDate);

                break;
            }

            case 5:
            {
                /* Delete Appointment (Appointment ID) */

                // 1) search for the id if exists proceed else throw a warning
                // 2) apply the delete logic
                // - mark the record
                // - manage the avail list
                cout << "Enter Doctor ID to delete: ";
                cin >> appointmentId;
                cin.ignore();
                deleteRecord(appFile, appointmentAvailList, appointmentId,false);

                break;
            }

            case 6:
            {
                /* Delete Doctor (Doctor ID) */

                // 1) search for the id if exists proceed else throw a warning
                // 2) apply the delete logic
                // - mark the record
                // - manage the avail list

                cout << "Enter Doctor ID to delete: ";
                cin >> doctorId;
                cin.ignore();
                deleteRecord(docFile, doctorsAvailList, doctorId,true);

                break;
            }

            case 7:
            {
                /* Print Doctor Info (Doctor ID) */

                cout<<"Please enter the Doctor ID:\n";
                cin>>doctorId;
                cin.ignore();
                printDoctorInfo(docFile, doctorId);

                break;
            }

            case 8:
            {
                /* Print Appointment Info (Appointment ID) */

                cout<<"Please enter the Appointment ID:\n";
                cin>>appointmentId;
                cin.ignore();
                printAppointmentInfo(appFile, appointmentId);

                break;
            }

            case 9:
            {
                /* Write Query */

                string query;
                cout<<"Please type the query:\n";
                cout<<">>";
                cin.ignore();
                getline(cin, query);
                queryExecuter(query);

                break;
            }

            case 10:
            {
                /* Read Doctors File */

                string str = readDoctors(docFile);
                cout<<str;

                break;
            }

            case 11:
            {
                /* Read Appointments File */

                string str = readAppointments(appFile);
                cout<<str;

                break;
            }

            case 12:
            {
                /* EXIT */
                return;
            }

            default:
            {
                cout<<"INVALID COMMAND! try again...\n";
                break;
            }
        }
    }
}

int main()
{

    /* Run the whole program through: */
    initializeFiles();
    launcher();

    return 0;
}
