#include <bits/stdc++.h>
using namespace std;

int static cntDoc, cntApp;

void insertPrim (fstream& file, const char id[], short offset, bool doc)
{
    int cnt;
    if (doc)
        cnt = cntDoc;
    else
        cnt = cntApp;

    file.clear();
    file.seekp(0, ios::beg);

    int ID = stoi(id);
    if (cnt == 0)
    {
        file.write((char*)& ID, sizeof(ID));
        char space = ' ';
        file.write(&space, sizeof(space));
        file.write((char*)& offset, sizeof(offset));
        char line = '\n';
        file.write(&line, sizeof(line));
    }
    else
    {
        int temp = 0;
        short place;
        bool found = false;
        file.read((char*)& temp, sizeof(temp));
        while (file.good())
        {
            if (ID < temp)
            {
                file.seekp(-4, ios::cur);
                place = file.tellp();
                found = true;
                break;
            }
            file.seekp(4, ios::cur);
            file.read((char*)& temp, sizeof(temp));
        }

        if (found)
        {
            int idEnd, id1, id2;
            short offsetEnd, offset1, offset2;

            file.clear();
            file.seekp((cnt-1) * 8, ios::beg);
            file.read((char*)& idEnd, sizeof(idEnd));
            file.seekp(1, ios::cur);
            file.read((char*)& offsetEnd, sizeof(offsetEnd));

            file.clear();
            file.seekp(place, ios::beg);
            file.read((char*)& id1, sizeof(id1));
            file.seekp(1, ios::cur);
            file.read((char*)& offset1, sizeof(offset1));

            while(file.good())
            {
                file.seekp(1, ios::cur);
                file.read((char*)& id2, sizeof(id2));
                file.seekp(1, ios::cur);
                file.read((char*)& offset2, sizeof(offset2));

                file.seekp(-7, ios::cur);

                file.write((char*)& id1, sizeof(id1));
                file.seekp(1, ios::cur);
                file.write((char*)& offset1, sizeof(offset1));

                id1 = id2; offset1 = offset2;
            }
            file.clear();
            file.seekp(0, ios::end);
            file.write((char*)& idEnd, sizeof(idEnd));
            char space = ' ';
            file.write(&space, sizeof(space));
            file.write((char*)& offsetEnd, sizeof(offsetEnd));
            char line = '\n';
            file.write(&line, sizeof(line));

            file.clear();
            file.seekp(place, ios::beg);
            file.write((char*)& ID, sizeof(ID));
            file.seekp(1, ios::cur);
            file.write((char*)& offset, sizeof(offset));
        }
        else
        {
            file.clear();
            file.seekp(0, ios::end);
            file.write((char*)& ID, sizeof(ID));
            char space = ' ';
            file.write(&space, sizeof(space));
            file.write((char*)& offset, sizeof(offset));
            char line = '\n';
            file.write(&line, sizeof(line));
        }
    }

    if (doc)
        cntDoc++;
    else
        cntApp++;

    file.flush();
}

void deletePrim (fstream& file, const char id[], bool doc)
{
    int cnt;
    if (doc)
        cnt = cntDoc;
    else
        cnt = cntApp;

    file.clear();
    file.seekp(0, ios::beg);

    int ID = stoi(id), temp;
    short beg = 0, end = cnt-1, mid;
    bool found = false;

    while (!found)
    {
        mid = (beg + end) / 2;
        file.seekp(mid * 8, ios::beg);
        file.read((char*)&temp, sizeof(temp));

        if (ID == temp)
            found = true;
        else if (ID <  temp)
            end = mid - 1;
        else
            beg = mid + 1;
    }

    int num; short offset;
    file.clear();
    file.seekp((mid+1) * 8, ios::beg);

    while (file.good())
    {
        file.read((char*)& num, sizeof(num));
        file.seekp(1, ios::cur);
        file.read((char*)& offset, sizeof(offset));

        file.seekp(-15, ios::cur);

        file.write((char*)& num, sizeof(num));
        file.seekp(1, ios::cur);
        file.write((char*)& offset, sizeof(offset));

        file.seekp(9, ios::cur);
    }

    if (doc)
        cntDoc--;
    else
        cntApp--;

    file.flush();
}

short searchID (fstream& file, const char id[], bool doc)
{
    int cnt;
    if (doc)
        cnt = cntDoc;
    else
        cnt = cntApp;

    file.clear();
    file.seekg(0, ios::beg);

    int ID = stoi(id), temp = 0;
    short beg = 0, end = cnt-1, mid;
    bool found = false;

    while (beg <= end && !found)
    {
        mid = (beg + end) / 2;
        file.seekg(mid * 8, ios::beg);
        file.read((char*)&temp, sizeof(temp));

        if (ID == temp)
            found = true;
        else if (ID <  temp)
            end = mid - 1;
        else
            beg = mid + 1;
    }

    if (found)
    {
        short offset;
        file.seekg((mid*8) + 5, ios::beg);
        file.read((char*)& offset, sizeof(offset));
        return offset;
    }
    else
        return -1;
}
