#include <bits/stdc++.h>
using namespace std;

class AvailList {
public:
    short head;

    AvailList() : head(-1) {}   // Initializing the head with -1

    // Retrieve the last updated head from the file
    void initialize (fstream &file)
    {
        file.seekg(0, ios::beg);
        string temp;
        getline(file, temp);
        head = stoi(temp);
    }

    // Update head to the new offset
    void updateHead (fstream &file, int newHead)
    {
        head = newHead;
        file.seekp(0, ios::beg);
        string strHead = to_string(head);
        if (strHead.size() == 1)
            strHead = '0' + strHead;
        file<<strHead;
    }

    void addNode (fstream &file, int index, int size)
    {
        file.seekp(index, ios::beg);
        string marker = "*|" + to_string(head) + "|";
        file << marker << string(size - marker.size(), ' '); // Mark fre
        updateHead(file, index);
    }

    int findSuitableNode (fstream &file, int sizeRequired)
    {
        int currentIndex = head;
        int previousIndex = -1;

        while (currentIndex != -1)
        {
            file.seekg(currentIndex, ios::beg);
            string line;
            getline(file, line);

            int nodeSize = stoi(line.substr(0, line.find('|'))); // Getting the size
            int nextIndex = stoi(line.substr(line.find('|') + 1)); // Getting next pointer

            if (nodeSize >= sizeRequired)
            {
                // Space found
                if (previousIndex == -1)
                {
                    updateHead(file, nextIndex); // Update head of avail list
                }
                else
                {
                    // Update previous node to skip current one
                    file.seekp(previousIndex, ios::beg);
                    string updatedNode = to_string(nodeSize) + "|" + to_string(nextIndex) + "|";
                    file<<updatedNode;
                }
                return currentIndex;
            }

            previousIndex = currentIndex;
            currentIndex = nextIndex;
        }

        return -1; // No suitable node found
    }
};
