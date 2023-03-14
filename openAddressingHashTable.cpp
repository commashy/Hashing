#include "openAddressingHashTable.h"

OpenAddressingHashTable::OpenAddressingHashTable(int m, int (*hash)(string, int)) : 
    HashTable(m, hash) {
        table = new Cell[m];
        for (int i=0; i<m; i++) {
            table[i].magic = nullptr;
            table[i].status = EMPTY;
        }
    }


OpenAddressingHashTable::~OpenAddressingHashTable() {
    for (int i=0; i<m; i++) {
        if (table[i].status != EMPTY) {
            delete table[i].magic;
            table[i].magic = nullptr;
        }
    }
    table = nullptr;
}


bool OpenAddressingHashTable::add(Magic* magic) {
    int C = 0;  // number of comparisons
    int R = 0;  // number of collisions
    int index = hash(magic->suffix, m);

    cout << "hash(\"" << magic->suffix << "\") = " << hash(magic->suffix, m) << endl;

    while (R<m) {
        if (activeCellCount > m/2) {
            cout << "Rehashing is needed!" << endl;
            performRehashing();
            C=0;
            R=0;
            index = hash(magic->suffix, m);
            continue;
        }

        if (table[index].status == EMPTY || table[index].status == DELETED) {
            // table[index].magic->prefix = magic->prefix;
            // table[index].magic->suffix = magic->suffix;
            // table[index].magic->price = magic->price;
            // table[index].magic->quantity = magic->quantity;
            table[index].magic = magic;
            table[index].status = ACTIVE;
            cout << magic->prefix + magic->suffix << " added at cell " << index << endl;

            activeCellCount++;
            C = R+1;
            comparisonCount += C;
            return true;
        }
        else {
            cout << magic->prefix + magic->suffix << " collided at cell " << index << endl;
            if (index == m-1)
                index = 0;
            else 
                index++;
            R++;
        }
    }
    comparisonCount += m;
    return false;
}


void OpenAddressingHashTable::performRehashing() {

    // create new table with double the size
    Cell* newTable = new Cell[m*2];
    for (int i=0; i<m*2; i++) {
        newTable[i].magic = nullptr;
        newTable[i].status = EMPTY;
    }

    // store old cells in temp table
    Cell* tempTable = new Cell[activeCellCount];
    for (int i=0; i<activeCellCount; i++) {
        tempTable[i].magic = nullptr;
        tempTable[i].status = EMPTY;
    }

    int index = 0;
    for (int i=0; i<m; i++) {
        if (table[i].status == ACTIVE) {
            tempTable[index] = table[i];
            index++;
        }
        if (index == activeCellCount)
            break;
    }
    cout << tempTable[0].magic->prefix << endl;

    // delete cells in old table
    for (int i=0; i<m; i++) {
        if (table[i].status == ACTIVE) {
            delete table[i].magic;
            table[i].magic = nullptr;
        }
    }
    table = newTable;
    newTable = nullptr;

    cout << tempTable[0].magic->suffix << endl;
    cout << tempTable[1].magic->suffix << endl;
    cout << tempTable[0].magic->prefix << endl;

    activeCellCount = 0;
    m *= 2;

    // add old cells to new table
    for (int i=0; i<index; i++) {
        add(tempTable[i].magic);
    }

    // delete cells in temp table
    for (int i=0; i<index; i++) {
        delete tempTable[i].magic;
        tempTable[i].magic = nullptr;
    }
    tempTable = nullptr;
    
    cout << "Rehashing is done!" << endl;
}


bool OpenAddressingHashTable::remove(string key) {
    int C = 1;  // number of comparisons
    // int R = 0;  // number of collisions
    int index = hash(key, m);
    cout << "hash(\"" << key << "\") = " << hash(key, m) << endl;

    for (int i=0; i<m; i++) {
        if (table[index].status == EMPTY) {
            cout << "visited cell " << index << " but could not find it" << endl;
            comparisonCount += C;
            cout << key << " cannot be removed" << endl;
            return false;
        }

        if (table[index].magic->suffix == key) {
            table[index].status = DELETED;
            cout << table[index].magic->prefix + table[index].magic->suffix << " removed at cell " << index << endl;
            delete table[index].magic;
            table[index].magic = nullptr;
            activeCellCount--;
            comparisonCount += C;
            return true;
        }
        else {
            cout << "visited cell " << index << " but could not find it" << endl;
            C++;
        }
        
        if (index == m-1)
            index = 0;
        else 
            index++;
    }
    cout << key << " cannot be removed" << endl;
    comparisonCount += m;
    return false;
}


Magic* OpenAddressingHashTable::get(string key) {
    int C = 1;  // number of comparisons
    // int R = 0;  // number of collisions
    int index = hash(key, m);
    cout << "hash(\"" << key << "\") = " << hash(key, m) << endl;

    for (int i=0; i<m; i++) {
        if (table[index].status == EMPTY) {
            cout << "visited cell " << index << " but could not find it" << endl;
            comparisonCount += C;
            cout << key << " cannot be found" << endl;
            return nullptr;
        }

        if (table[index].magic->suffix == key) {
            comparisonCount += C;
            return table[index].magic;
        }
        else {
            cout << "visited cell " << index << " but could not find it" << endl;
            C++;
        }
        
        if (index == m-1)
            index = 0;
        else 
            index++;
    }
    comparisonCount += m;
    cout << key << " cannot be found" << endl;
    return nullptr;
}


int OpenAddressingHashTable::getClusterCount() const {
    bool check = false;
    int count = 0;
    for (int i=0; i<m; i++) {
        if (table[i].status == ACTIVE) {
            if (!check) {
                count++;
                check = true;
            }
        }
        else    // if (table[i].status == EMPTY || table[i].status == DELETED) {
            check = false;
    }
    if (table[0].status == ACTIVE && table[m-1].status == ACTIVE) {
        count--;
    }

    return count;
}


int OpenAddressingHashTable::getLargestClusterSize() const {
    if (getClusterCount()==0)
        return 0;

    for (int i=0; i<m; i++) {

    }

    return 0;
}

string OpenAddressingHashTable::getClusterSizeSortedList() const {
    string str = "hi";
    return str;
}

