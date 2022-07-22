#include "swarm.h"
#include <random>

enum RANDOM {UNIFORM, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORM) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 and standard deviation of 20 
            m_normdist = std::normal_distribution<>(50,20);
        }
        else{
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
    }

    int getRandNum(){
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else{
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//uniform distribution

};

class Tester{
public:
    void testTreeBalance(Swarm* swarm);
    void testRemoveDead(Swarm* swarm);
    void testDuplicates(Swarm* swarm, int ID);
    void testCheckDead(Swarm* swarm);
    void testEfficiency(Swarm* swarm, int ID, int size);
};
void Tester::testTreeBalance(Swarm* swarm) {
    //this function tests the trees balance and prints out the results.
    cout << "\n\n\n------------Testing tree Balance------------\n" << endl;
    swarm->testBalance();
    cout << "\n-----------End test tree Balance--------------\n\n\n" << endl;
}

void Tester::testRemoveDead(Swarm* swarm) {
    //this function tests the Remove Dead function from swarm, currently remove dead is not properly working.
    cout << "\n";
    cout << "-------Test Remove Dead-------" << endl;
    cout << "\nDump before removal: \n" << endl;
    swarm->listRobots();
    swarm->removeDead();
    cout << "\nDump after removal: \n" << endl;
    swarm->listRobots();
}
void Tester::testDuplicates(Swarm* swarm, int ID) {
    //this function is for small scale presenting, its used to show that a node with the same ID cannot be inserted
    cout << "\n";
    cout << "-------Test Duplicates-------" << endl;
    cout << "Inserting node with ID " << ID << endl;
    cout << "\nDump before: \n" << endl;
    swarm->listRobots();
    Robot robot(ID);
    swarm->insert(robot);
    cout << "\nDump after: \n" << endl;
    swarm->listRobots();
    cout << "----------END TEST---------" << endl;
}
void Tester::testCheckDead(Swarm *swarm) {
    swarm->testCheckDead();
}

void Tester::testEfficiency(Swarm* swarm, int ID, int size) {
    //this function tests the efficiency of both removing and inserting, it first removes a node and prints the time it takes
    //It then takes that same node's id and reinserts a new node with that ID and times it again, then prints it out.

    clock_t start, stop;
    double T = 0.0;
    start = clock();
    swarm->remove(ID);
    stop = clock();
    T = stop-start;
    cout << "\n\n------------Testing Efficiency-----------" << endl;
    cout << "Swarm size - " << size << endl;
    cout << "removing robot with id " << ID << endl;
    cout << "Removal took " << T << " clock ticks (" << T/CLOCKS_PER_SEC << " seconds)!" << endl;
    if(swarm->findBot(ID)) {
        cout << "Robot still in swarm!" << endl;
    } else {
        cout << "Robot removed successfully" << endl;
    }
    cout << "Reinserting robot with id " << ID << endl;
    start = clock();
    swarm->insert(ID);
    stop = clock();
    T = stop - start;
    cout << "Inserting took " << T << " clock ticks (" << T/CLOCKS_PER_SEC << " seconds)!" << endl;
    if(swarm->findBot(ID)) {
        cout << "Robot still in swarm!" << endl;
    } else {
        cout << "Robot removed successfully" << endl;
    }
    cout << "----------End testing efficiency------------" << endl;
}
int main(){
    int FirstID = 0;
    Tester tester;
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    {
        Swarm team;
        int teamSize = 20;
        int tempID = 0;
        int ID = 0;
        for(int i=0;i<teamSize;i++){
            ID = idGen.getRandNum();
            if (i == teamSize / 2) tempID = ID;//we store this ID for later use
            Robot robot(ID,static_cast<ROBOTTYPE>(typeGen.getRandNum()));
            team.insert(robot);
        }
        cout << "\nDump after inserting " << teamSize << " nodes:\n\n";//test for inserting nodes ^
        team.dumpTree();
        cout << "\n\nList of Robots after inserting " << teamSize << " nodes:\n";
        team.listRobots();
        cout << endl;
        cout << "Find bot with ID " << tempID << endl;
        if(team.findBot(tempID)) {
            cout << "Found!" << endl;
        } else {
            cout << "Not found!" << endl;
        }

        tester.testTreeBalance(&team);//BALANCE TEST AFTER INSERTING

        tester.testDuplicates(&team, tempID);//Test for duplicate ID enteries

        team.remove(tempID);//Test for removing a node
        cout << "\nDump after removing the node with ID: " << tempID << "\n\n";
        team.dumpTree();
        cout << "\n\nList of Robots after removing the node with ID: " << tempID << "\n";
        team.listRobots();
        cout << endl;
        cout << "Find bot with ID " << tempID << endl;
        if(team.findBot(tempID)) {
            cout << "Found!" << endl;
        } else {
            cout << "Not found!" << endl;
        }

        tester.testTreeBalance(&team);//BALANCE TEST AFTER REMOVAL

        //Inserting dead nodes to test removeDead()
        for(int i=0;i<teamSize/2;i++){
            ID = idGen.getRandNum();
            Robot robot(ID,static_cast<ROBOTTYPE>(typeGen.getRandNum()));
            robot.setState(DEAD);
            team.insert(robot);
        }
        cout << "________Does not work______ (REMOVEDEAD FUNCTION)" << endl;
        tester.testRemoveDead(&team);
        tester.testCheckDead(&team);
        team.dumpTree();
        tester.testCheckDead(&team);

        cout << "\n\n NOTE: remove dead function has issues where it sometimes removes alive robots and sets some alive to dead" << endl;



        //Efficiency testing

        //Size 10000
        Swarm team1;
        teamSize = 100000;
        tempID = 0;
        FirstID = 0;
        ID = 0;
        for(int i=0;i<teamSize;i++){
            ID = idGen.getRandNum();
            if (i == teamSize / 2) tempID = ID;//we store this ID for later use
            Robot robot(ID,static_cast<ROBOTTYPE>(typeGen.getRandNum()));
            team.insert(robot);
            if (i == 0) {
                FirstID = ID;
            }
        }
        tester.testEfficiency(&team1, FirstID, teamSize);
        tester.testEfficiency(&team1, ID, teamSize);
        tester.testTreeBalance(&team1);
        //Size 20000
        Swarm team2;
        teamSize = 200000;
        tempID = 0;
        FirstID = 0;
        ID = 0;
        for(int i=0;i<teamSize;i++){
            ID = idGen.getRandNum();
            if (i == teamSize / 2) tempID = ID;//we store this ID for later use
            Robot robot(ID,static_cast<ROBOTTYPE>(typeGen.getRandNum()));
            team.insert(robot);
            if (i == 0) {
                FirstID = ID;
            }
        }
        tester.testEfficiency(&team2, FirstID, teamSize);
        tester.testEfficiency(&team2, ID, teamSize);
        tester.testTreeBalance(&team2);
        //Size 40000
        Swarm team3;
        teamSize = 400000;
        tempID = 0;
        FirstID = 0;
        ID = 0;
        for(int i=0;i<teamSize;i++){
            ID = idGen.getRandNum();
            if (i == teamSize / 2) tempID = ID;//we store this ID for later use
            Robot robot(ID,static_cast<ROBOTTYPE>(typeGen.getRandNum()));
            team.insert(robot);
            if (i == 0) {
                FirstID = ID;
            }
        }
        tester.testEfficiency(&team3, FirstID, teamSize);
        tester.testEfficiency(&team3, ID, teamSize);
        tester.testTreeBalance(&team3);


    }
    return 0;
}

