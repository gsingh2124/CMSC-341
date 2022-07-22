//UMBC - CSEE - CMSC 341 - Fall 2021 - Proj2
#include "swarm.h"
Swarm::Swarm(){
    Robot* robot = new Robot();
    m_root = robot;
}

Swarm::~Swarm(){
    clear();
    delete m_root;
}

void Swarm::insert(const Robot& robot){
    if (m_root->getID() == 0) {//Id can never be 0 (MINID/MAXID) so this can be used to fill the root
        m_root->setID(robot.getID());
        m_root->setState(robot.getState());
        m_root->setType(robot.getType());
        m_root->setHeight(robot.getHeight());
        m_root->setLeft(robot.getLeft());
        m_root->setRight(robot.getRight());
    } else {
        m_root = insert(robot, m_root);
    }
}

Robot* Swarm::insert(const Robot& robot, Robot*& aBot) {//recursive insert function
    if (aBot == nullptr) {//assigns the empty node with all the values of aBot
        Robot* aRobot = new Robot();
        aRobot->setID(robot.getID());
        aRobot->setState(robot.getState());
        aRobot->setType(robot.getType());
        aRobot->setHeight(robot.getHeight());
        aRobot->setLeft(robot.getLeft());
        aRobot->setRight(robot.getRight());
        return aRobot;
    } else if (aBot->getID() > robot.getID()) {//inserts to the left
        aBot->m_left = insert(robot, aBot->m_left);
        updateHeight(aBot);
        return rebalance(aBot);
    } else if (aBot->getID() < robot.getID()) {//inserts to the right
        aBot->m_right = insert(robot, aBot->m_right);
        updateHeight(aBot);
        return rebalance(aBot);
    } else {
        return aBot;
    }
}

void Swarm::clear(){
    clearSwarm(m_root);
}
void Swarm::clearSwarm(Robot* aBot) {//deletes every robot in swarm
    if (aBot == nullptr) {
        return;
    } else {
        clearSwarm(aBot->m_left);
        //remove(m_root, aBot->getID());
        clearSwarm(aBot->m_right);
        aBot = nullptr;
        delete aBot;
    }
}
void Swarm::remove(int id){
    remove(m_root, id);
}

Robot* Swarm::remove(Robot* aBot, int val) {
    Robot* temp;//temp variable for storing min value and aBot
    if (aBot == nullptr) {
        return nullptr;//cannot remove something that doesnt exist
    } else if (val < aBot->getID()){
        aBot->m_left = remove(aBot->m_left, val);
    } else if (val > aBot->getID()) {
        aBot->m_right = remove(aBot->m_right, val);
    } else if (aBot->m_left && aBot->m_right) {
        temp = findMin(aBot->m_right);//finds right side lowest node,
        aBot->m_id = temp->getID();
        aBot->m_state = temp->getState();//assign all values over
        aBot->m_type = temp->getType();
        aBot->m_left = temp->getLeft();
        aBot->m_height = temp->getHeight();
        aBot->m_right = remove(aBot->m_right, aBot->m_id);
    } else {//when there is zero or one child
        temp = aBot;
        if(aBot->m_left == nullptr) {
            aBot = aBot->m_right;
        } else if (aBot->m_right == nullptr) {
            aBot = aBot->m_left;
        }
        delete temp;
    }
    updateHeight(aBot);
    aBot = rebalance(aBot);
    return aBot;
}

Robot* Swarm::findMin(Robot* aBot) {
    if (aBot == nullptr || aBot->m_left == nullptr) {
        return aBot;
    } else {
        return findMin(aBot->m_left);//only need to traverse left since tree is balanced
    }
}
Robot* Swarm::findMax(Robot* aBot) {
    if(aBot == nullptr || aBot->m_right == nullptr) {
        return aBot;
    } else {
        return findMax(aBot->m_right);//only need to traverse right since tree is balanced
    }
}
void Swarm::updateHeight(Robot *aBot){//Function updates the height of a node
    if (aBot == nullptr) {
        return;
    }
    int leftHeight = (aBot->getLeft() == nullptr ? -1 : aBot->getLeft()->getHeight());//sets the left height
    int rightHeight = (aBot->getRight() == nullptr ? -1 : aBot->getRight()->getHeight());//sets the right height
    aBot->setHeight(1+(leftHeight > rightHeight ? leftHeight : rightHeight));//assigns new height to robot
}

int Swarm::checkImbalance(Robot* aBot){//function subtracts the left and right height of a node to return its balance
    if (aBot == nullptr) {
        return -1;
    } else {
        int leftHeight = -1;
        int rightHeight = -1;
        if (aBot->m_left != nullptr) {
            leftHeight = aBot->m_left->getHeight();//set left height
        }
        if (aBot->m_right != nullptr) {
            rightHeight = aBot->m_right->getHeight();//set right height
        }
        return (leftHeight - rightHeight);
    }
}

Robot* Swarm::rebalance(Robot* aBot){
    if ((checkImbalance(aBot) < -1) && (checkImbalance(aBot->m_right) <= 0)) {
        return leftRotation(aBot);//Case when balance of node is < -1 and the right side of the node is <= 0, left rotate
    } else if ((checkImbalance(aBot) > 1) && (checkImbalance(aBot->m_left) >= 0)) {
        return rightRotation(aBot);//Case when balance of node is > 1 and the left side of the node is <= 0, right rotate
    } else if ((checkImbalance(aBot) < -1) && (checkImbalance(aBot->m_right) >= 0)) {
        aBot->m_right = rightRotation(aBot->m_right);//case to RL, nodes balance is < -1 and right node is >= 0
        return leftRotation(aBot);
    } else if ((checkImbalance(aBot) > 1) && (checkImbalance(aBot->m_left) <= 0)) {
        aBot->m_left = leftRotation(aBot->m_left);//case to LR, nodes balance is > 1 and right node is <= 0
        return rightRotation(aBot);
    } else {
        return aBot;
    }
}
Robot* Swarm::leftRotation(Robot* aBot) {
    Robot* pivot = aBot;//Node to rotate around
    Robot* next = pivot->m_right;//the node in question to rotate
    pivot->m_right = next->m_left;//swap around child robots
    next->m_left = pivot;
    updateHeight(pivot);//update height of both nodes
    updateHeight(next);
    return next;
}

Robot* Swarm::rightRotation(Robot* aBot) {
    Robot* pivot = aBot;//Node to rotate around
    Robot* next = pivot->m_left;//the node in question to rotate
    pivot->m_left = next->m_right;//swap around child robots
    next->m_right = pivot;
    updateHeight(pivot);//update height of both nodes
    updateHeight(next);
    return next;
}

void Swarm::listRobots() const {
    printBots(m_root);
}
void Swarm::printBots(Robot* aBot) const{//Function formats bots to a view of their actual values to print out
    if (aBot == nullptr) {
        return;
    } else {
        printBots(aBot->m_left);
        cout << aBot->getID() << ":" << aBot->getStateStr() << ":" << aBot->getTypeStr() << endl;
        printBots(aBot->m_right);
    }
}

bool Swarm::setState(int id, STATE state){
    return (setState(id, state, m_root));
}

Robot* Swarm::setState(int id, STATE state, Robot* aBot) {//sets state of robot
    if (aBot != nullptr){
        setState(id, state, aBot->m_left);
        if (aBot->getID() == id) {
            aBot->setState(state);
            return aBot;
        }
        setState(id, state, aBot->m_right);
    }
    return aBot;
}

void Swarm::removeDead(){//This function first gets the number of dead nodes to create an array of ID's of dead robots.
    int numDead = getNumDeadRobots(m_root);//The function then calls removeDead size times to remove all dead nodes
    int tempsize = numDead;
    int array[numDead];
    int* deadRobots;
    deadRobots = getDead(m_root, array, &tempsize);
    for (int i = 0; i < numDead; i++) {
        removeDead(m_root, array, numDead);
    }
    for(int i = 0; i < tempsize; i++) {
        deadRobots[i] = 0;
    }
}

Robot* Swarm::removeDead(Robot* aBot, int* array, int size) {//this function removes every dead node from an array of ID's
    for (int i = 0; i < size; i++) {
        remove(array[i]);
    }
    return aBot;
}

bool Swarm::findBot(int id) const {//function call to recursive function of findbot
    if(findBot(m_root, id)) {
        return true;
    } else {
        return false;
    }
}

bool Swarm::findBot(Robot *aBot, int id) const {
    if (aBot == nullptr) {//if empty, not found
        return false;
    }
    if (aBot != nullptr) {
        if (aBot->getID() == id) {//base case if id's match
            return true;
        }
    }
    bool output = findBot(aBot->m_left, id);
    bool output2 = findBot(aBot->m_right, id);
    if (output) {//recursive basecase for left hand traversal
        return true;
    }
    if (output || output2) {//if either is true, then node is found
        return true;
    } else {
        return false;
    }
}

void Swarm::dumpTree() const {
    dump(m_root);
}

void Swarm::dump(Robot* aBot) const{
    if (aBot != nullptr){
        cout << "(";
        dump(aBot->m_left);//first visit the left child
        cout << aBot->m_id << ":" << aBot->m_height;//second visit the node itself
        dump(aBot->m_right);//third visit the right child
        cout << ")";
    }
}

void Swarm::testCheckDead() {
    testCheckDead(m_root);
}

void Swarm::testCheckDead(Robot* aBot) {//develop function used to help easily identy which robots are dead
    if (aBot != nullptr){
        testCheckDead(aBot->m_left);//first visit the left child
        if (aBot->getStateStr() == "DEAD") {
            cout << aBot->getID() << " is Dead and still within the swarm." << endl;
        }
        testCheckDead(aBot->m_right);//third visit the right child
    }
}

int Swarm::getNumDeadRobots(Robot *aBot) {//Returns the number of dead robots within the tree
    if (aBot == nullptr) {
        return 0;
    } else {
        if (aBot->getStateStr() == "DEAD") {
            return 1 + getNumDeadRobots(aBot->m_left) + getNumDeadRobots(aBot->m_right);
        } else {
            return 0 + getNumDeadRobots(aBot->m_left) + getNumDeadRobots(aBot->m_right);
        }
    }
}
int* Swarm::getDead(Robot* aBot, int* array, int* size) {//returns an array of intergers that contains the ID's of all dead robots
    if (aBot != nullptr){
        getDead(aBot->m_left, array, size);//first visit the left child
        if (aBot->getState() == DEAD) {
            array[*size-1] = aBot->getID();
            *size = *size - 1;
        }
        getDead(aBot->m_right, array, size);//third visit the right child
    }
    return array;
}

void Swarm::testBalance() {
    bool val = checkImbalance(m_root);
    if (val) {
        cout << "Tree is balanced" << endl;
    } else {
        cout << "Tree is not balanced" << endl;
    }
}