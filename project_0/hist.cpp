/*****************************************
** File:    hist.cpp
** Project: CMSC 341 Project 0, Fall 2021
** Author:  Gurjinder Singh
** Date:    9/13/2021
** E-mail:  gsingh10@umbc.edu
***********************************************/
#include "hist.h"

Data::Data():m_arraySize(DATAARRAYSIZE){
    m_data = new int [m_arraySize];
    for (int i = 0; i < DATAARRAYSIZE; i++) {//set all indexes to 0
        m_data[i] = 0;
    }

}
Data::~Data(){
    for (int i = 0; i < DATAARRAYSIZE; i++) {//set all indexes to 0 (for clearing the data)
        m_data[i] = 0;
    }
    delete []m_data;
}
void Data::simulateData(int students){
    try {
        if (students > 0) {
            Random r(MINGRADE, MAXGRADE);
            for (int i = 0; i < students; i++) {//Randomly generates student grades
                int num = r.getNormRand();
                if (num >= 0 && num <= 9) {
                    m_data[0]+=1;
                } else if (num >= 10 && num <= 19) {
                    m_data[1]+=1;
                } else if (num >= 20 && num <= 29) {
                    m_data[2]+=1;
                } else if (num >= 30 && num <= 39) {
                    m_data[3]+=1;
                } else if (num >= 40 && num <= 49) {
                    m_data[4]+=1;
                } else if (num >= 50 && num <= 59) {
                    m_data[5]+=1;
                } else if (num >= 60 && num <= 69) {
                    m_data[6]+=1;
                } else if (num >= 70 && num <= 79) {
                    m_data[7]+=1;
                } else if (num >= 80 && num <= 89) {
                    m_data[8]+=1;
                } else if (num >= 90 && num <= 100) {
                    m_data[9]+=1;
                }
            }
        } else {
            throw exception();
        }
    } catch (exception &e) {
        cout << "Warning, trying to simulate data for student size less than 1" << endl;
    }
}
void Data::dump(int item){
    string itemS = to_string(item);
    string histogramOfData =
            "\nHistogram for item " + itemS +
            "\n10  " + string(m_data[0], '*') +
            "\n20  " + string(m_data[1], '*') +
            "\n30  " + string(m_data[2], '*') +
            "\n40  " + string(m_data[3], '*') +
            "\n50  " + string(m_data[4], '*') +
            "\n60  " + string(m_data[5], '*') +
            "\n70  " + string(m_data[6], '*') +
            "\n80  " + string(m_data[7], '*') +
            "\n90  " + string(m_data[8], '*') +
            "\n100 " + string(m_data[9], '*') +
            "\n";

    cout << histogramOfData;
}
int Data::getData(int index){
    return m_data[index];
}
int Data::getArraySize(){
    return m_arraySize;
}
// Overloaded equality operator. It returns true if two objects are the same.
// The "same objects" means they are the same pointers and carry the same value
// in every correpsonding cell.
// For testing inequality we can simply negate the return value of this operator.
bool operator==(const Data& lhs, const Data& rhs){
    if (&lhs == &rhs)
    {
        for (int i=0;i<lhs.m_arraySize;i++){
            if (lhs.m_data[i] != rhs.m_data[i])
                return false;
        }
        return true;
    }
    else return false;
}

Hist::Hist(int items){
    try {
        if (items > 0) {
            m_table = new Data[items];
            m_items = items;

        } else {
            m_items = 0;
            throw out_of_range("");
            cout << "WARNING: Number of items is less than 1";
        }
    } catch (exception &e) {
        cout << e.what() << "\n";
        cout << "WARNING: Number of items is less than 1";
    }
}
Hist::~Hist(){
    if (m_items > 0) {
        delete []m_table;// Delete table pointer array
    }
    m_items = 0;
}

void Hist::dump(){
    for (int i=0;i<m_items;i++){
        m_table[i].dump(i);
        cout << endl;
    }
}

void Hist::simulateData(int students){
    try {
        if (m_items > 1 && students > 0){
            for (int i = 0; i < m_items; i++) {//fill table with student data
                m_table[i].simulateData(students);
            }
        } else if (m_items == 0) {
            throw exception();
        }
    } catch (exception &e) {//case if there are no students
        cout << "\n*Simulate Data* Cannot simulate data for table size 0" << endl;
    }
}

Hist::Hist(const Hist & rhs){
    try {
        if (rhs.m_items > 0) {
            m_table = new Data[rhs.m_items];//initilize m_table
            m_items = rhs.m_items; // assign items var
            for (int i = 0; i < m_items; i++) {//loop tables
                for (int j = 0; j < DATAARRAYSIZE; j++) {//loop data
                    m_table[i].m_data[j] = rhs.m_table[i].m_data[j];//copy over data
                }
            }
        } else {
            m_items = 0;
            throw exception();//case if items is less than 1
        }
    } catch (exception &e) {
        cout << "\nWARNING: Trying to copy from a Histogram of size less than 1";
    }
}

const Hist & Hist::operator=(const Hist & rhs){//assignment operator overload
    Hist *copy = new Hist(rhs.m_items);//create a new obj to return
    if (this != &rhs) {//self assignment check
        try {
            if (rhs.m_items > 0) {
                copy->m_table = new Data[rhs.m_items];//copy over table size
                copy->m_items = rhs.m_items;//copy over num items
                for (int i = 0; i < m_items; i++) {//table loop
                    for (int j = 0; j < DATAARRAYSIZE; j++) {//data loop
                        copy->m_table[i].m_data[j] = rhs.m_table[i].m_data[j];//copy over data to new data
                    }
                }
            } else {
                copy->m_items = 0;
                throw exception();//case for when items is less than 0
            }
        } catch (exception &e) {
            cout << "\nWARNING: Trying to copy from a Histogram of size less than 1";
        }
    }
    return *copy;
}