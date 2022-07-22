/*****************************************
** File:    hash.cpp
** Project: CMSC 341 Project 4, Fall 2021
** Author:  Gurjinder Singh
** Date:    12/8/2021
** E-mail:  gsingh10@umbc.edu
** Desc: Project 4, create a hashtable to
 * represent a file system, with rehashing and
 * efficient access of files.
 * (Other files dont heave headers like this
 * because they were completely unchanged
 * except hash.h)
 *
***********************************************/
#include "hash.h"
#include "math.h"
HashTable::HashTable(unsigned size, hash_fn hash){
    m_hash = hash;
    m_size1 = 0;
    m_numDeleted1 = 0;
    if (isPrime(size)) {//if prime
        if (size <= MAXPRIME) {
            if (size >= MINPRIME) {
                m_capacity1 = size;
                m_table1 = new File[m_capacity1];
            }
        }
    } else {//assign calculated prime number from size
        m_capacity1 = findNextPrime(size);
        m_table1 = new File[m_capacity1];
    }
    m_newTable = TABLE1;

}

HashTable::~HashTable(){
    m_capacity1 = 0;
    m_capacity2 = 0;
    m_size1 = 0;
    m_size2 = 0;
    m_numDeleted1 = 0;
    m_numDeleted2 = 0;
    delete[] m_table1;
    delete[] m_table2;
}

File HashTable::getFile(string name, unsigned int diskBlock){//searches both tables for given values
    for (unsigned int i = 0; i < tableSize(TABLE1); i++) {
        if (m_table1[i].key() == name && m_table1[i].diskBlock() == diskBlock) {
            return m_table1[i];//if found in table1
        }
    }
    for (unsigned int i = 0; i < tableSize(TABLE2); i++) {
        if (m_table2[i].key() == name && m_table2[i].diskBlock() == diskBlock) {
            return m_table2[i];//if found in table2
        }
    }
    return EMPTY;//if not found in either table
}

bool HashTable::insert(File file) {
    bool output = false;
    if (m_newTable == TABLE1) {
        if (m_table1[m_hash(file.key()) % tableSize(m_newTable)] == EMPTY ||
                m_table1[m_hash(file.key()) % tableSize(m_newTable)] == DELETED) {//if the current index being looked at is empty, insert file here
            m_table1[m_hash(file.key())% tableSize(m_newTable)] = file;
            output = true;
            m_size1++;
        } else {//when the expected index is occupied
            int num = probe(file);
            m_table1[(m_hash(file.key()) % tableSize(m_newTable)) + (num*num) % tableSize(m_newTable)] = file;
            output = true;
            m_size1++;
        }
    } else {//same exact as for table 1 ^^^^ (theres probably a better way to do this)
        if (m_table2[m_hash(file.key()) % tableSize(m_newTable)] == EMPTY ||
            m_table2[m_hash(file.key()) % tableSize(m_newTable)] == DELETED) {
            m_table2[m_hash(file.key())% tableSize(m_newTable)] = file;
            output = true;
            m_size2++;
        } else {
            int num = probe(file);
            m_table2[(m_hash(file.key()) % tableSize(m_newTable)) + (num*num) % tableSize(m_newTable)] = file;
            output = true;
            m_size2++;
        }
    }
    reHashCall();//call to function where rehashing is checked and done.
    return output;
}

bool HashTable::remove(File file){//loops through both tables and attempts to delete the file, if deleted returns true, if not returns false
    reHashCall();//call to function where rehashing is checked and done.
    bool output = false;
    for (unsigned int i = 0; i < tableSize(TABLE1); i++) {
        if (m_table1[i] == file) {
            m_table1[i] = DELETED;
            output = true;
            m_numDeleted1++;
        }
    }
    for (unsigned int i = 0; i < tableSize(TABLE2); i++) {
        if (m_table2[i] == file) {
            m_table2[i] = DELETED;
            output = true;
            m_numDeleted2++;
        }
    }
    reHashCall();//call to function where rehashing is checked and done.
    return output;
}

float HashTable::lambda(TABLENAME tablename) const {

    if (tablename == TABLE1) {
        return (m_size1-m_numDeleted1)/m_capacity1;
    } else {
        return (m_size2-m_numDeleted1)/m_capacity2;
    }
    /*
    int occupied;
    for (int i = 0; i < tableSize(tablename); i++) {//loop from minsize, to table size
        if (tablename == TABLE1) {//for table1
            if (m_table1[i] == EMPTY) {
                //do nothing
            } else {//increments occupied count
                occupied++;
            }
        } else {//for table2, same as above
            if (m_table2[i] == EMPTY) {
                //do nothing
            } else {
                occupied++;
            }
        }
    }
    return occupied/tableSize(tablename);
     */
}

float HashTable::deletedRatio(TABLENAME tableName) const {
    //int deleted = 0;
    //int alive = 0;

    if (tableName == TABLE1) {
        return m_numDeleted1/m_size1;
    } else {
        return m_numDeleted2/m_size2;
    }
    /*
    for (int i = 0; i < tableSize(tableName); i++) {//loop from minsize, to table size
        if (tableName == TABLE1) {//for table1
            if (m_table1[i] == DELETED) {//increment deleted count
                deleted++;
            } else if (m_table1[i] == EMPTY) {
                //do nothing if empty
            } else {//increment alive count
                alive++;
            }
        } else {//for table2, same as above
            if (m_table2[i] == DELETED) {
                deleted++;
            } else if (m_table2[i] == EMPTY) {
                //do nothing if empty
            } else {
                alive++;
            }
        }
    };
    return deleted/alive;
     */
}

void HashTable::reHashTable() {
    int capacity;
    if (m_newTable == TABLE1) {//determine which table to use for rehashing
        capacity = numEntries(TABLE1);
    } else {
        capacity = numEntries(TABLE2);
    }
    int target = capacity/4;
    int count = 0;

    if (m_newTable == TABLE1) {//for table 1
        while (target != 0) {
            if (m_table1[count] == EMPTY || m_table1[count] == DELETED) {
            } else {
                insert(m_table1[count]);//inserts into the m_newTable
                m_table1[count] = DELETED;//deletes old file
            }
            count++;
            target--;
        }
    } else {//for table2
        while (target != 0) {
            if (m_table2[count] == EMPTY || m_table2[count] == DELETED) {
            } else {
                insert(m_table2[count]);//inserts into the m_newTable
                m_table2[count] = DELETED;//deletes old file
            }
            count++;
            target--;
        }
    }

}

void HashTable::reHashCall() {
    if (lambda(m_newTable) >= .5 || deletedRatio(m_newTable) >= .8) {//rehash if load is 50% or deleteratio is 80% or more
        reHashTable();//function call to begin rehashing
        if (m_newTable == TABLE1) {
            m_newTable = TABLE2;//switch main tables being used
            m_capacity2 = findNextPrime((m_size1-m_numDeleted1)*4);//assign given equation for new size
            m_size2 = 0;//reset some vars
            m_numDeleted2 = 0;
        } else {
            m_newTable = TABLE1;
            m_capacity1 = findNextPrime((m_size1-m_numDeleted1)*4);
            m_size1 = 0;
            m_numDeleted1 = 0;
        }
    }
}

int HashTable::probe(File file) {//returns -1 if reaching max size before finding appropriate i value for quadratic probing
    int num = -1;
    for (int i = 0; m_hash(file.key()) % tableSize(m_newTable) + (i*i) % tableSize(m_newTable) < tableSize(m_newTable); i++) {
        if (m_newTable == TABLE1) {
            if (m_table1[m_hash(file.key()) % tableSize(m_newTable) + (i*i) % tableSize(m_newTable)] == EMPTY ||
                    m_table1[m_hash(file.key()) % tableSize(m_newTable) + (i*i) % tableSize(m_newTable)] == DELETED) {
                num = i;
                break;
            }
        } else {
            if (m_table2[m_hash(file.key()) % tableSize(m_newTable) + (i*i) % tableSize(m_newTable)] == EMPTY ||
                m_table2[m_hash(file.key()) % tableSize(m_newTable) + (i*i) % tableSize(m_newTable)] == DELETED) {
                num = i;
                break;
            }
        }
    }
    return num;
}

void HashTable::dump() const {
    cout << "Dump for table 1: " << endl;
    if (m_table1 != nullptr)
        for (unsigned int i = 0; i < m_capacity1; i++) {
            cout << "[" << i << "] : " << m_table1[i] << endl;
        }
    cout << "Dump for table 2: " << endl;
    if (m_table2 != nullptr)
        for (unsigned int i = 0; i < m_capacity2; i++) {
            cout << "[" << i << "] : " << m_table2[i] << endl;
        }
}

bool HashTable::isPrime(int number){//checks if the current number is prime
    bool result = true;
    for (int i = 2; i <= number / 2; ++i) {
        if (number % i == 0) {
            result = false;
            break;
        }
    }
    return result;
}

int HashTable::findNextPrime(int current){
    //we always stay within the range [MINPRIME-MAXPRIME]
    //the smallest prime starts at MINPRIME
    if (current < MINPRIME) current = MINPRIME-1;
    for (int i=current; i<MAXPRIME; i++) { 
        for (int j=2; j*j<=i; j++) {
            if (i % j == 0) 
                break;
            else if (j+1 > sqrt(i) && i != current) {
                return i;
            }
        }
    }
    //if a user tries to go over MAXPRIME
    return MAXPRIME;
}
