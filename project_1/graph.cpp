//UMBC - CSEE - CMSC 341 - Fall 2021 - Proj1
/*****************************************
** File:    graph.cpp
** Project: CMSC 341 Project 1, Fall 2021
** Author:  Gurjinder Singh
** Date:    10/5/2021
** E-mail:  gsingh10@umbc.edu
***********************************************/
#include "graph.h"
Graph::Graph(){//default constructor
    m_dataFile = "";
    m_head = nullptr;
}

Graph::Graph(string dataFile){//alternate constructor with data file passed through
    m_dataFile = dataFile;
    m_head = nullptr;
    loadData();
}

Graph::~Graph(){//graph destructor
    Node * current = m_head;
    while (current != nullptr) {
        current->setValue(0);
        current->setVisited(false);
        current->setNorth(nullptr);
        current->setEast(nullptr);
        current->setSouth(nullptr);
        current->setWest(nullptr);

        current = current->m_next;
    }
    current = nullptr;//safety check? can never be too sure lol
    delete current;

    //Delete m_next pointers
    Node * current2 = m_head;
    while (current2 != nullptr) {
        Node * next = current2->m_next;
        current2 = nullptr;
        delete current2;
        current2 = next;
    }
    current2 = nullptr;//same safety check here
    delete current2;
    clearGraph();
}

void Graph::loadData(){//provided function
    int numNodes;
    int node, n, e, s, w; 
    ifstream dataFile;
    dataFile.open(m_dataFile);
    if (dataFile.is_open()) {
        dataFile >> numNodes;
        m_numNodes = numNodes;
        for (int i = 0; i < numNodes; i++) {
            dataFile >> node >> n >> e >> s >> w;
            insert(node, n, e, s, w);
        }
    }
    else
        //the following statement provides the reason if a file doesn't open
        //please note: not all platforms are providing the same message
        cerr << "Error: " << strerror(errno) << endl;
}

void Graph::insert(int node, int n, int e, int s, int w){
    Node * temp = findNode(node);//Assign temp to a node, nullptr if not found
    if (empty()) {//if the graph is empty, then assign temp to the m_head
        temp = new Node(node);
        m_head = temp;
        if (n == -1) {//assignment for -1 val
            temp->setNorth(nullptr);
        } else {
            if (temp->getValue() != n) {//check to see if setting pointer to self
                Node * newNode = new Node(n);
                insertAtEnd(newNode);
                temp->setNorth(newNode);
            }
        }//algorithm is the same for the remaining 3 cardinals
        if (e == -1) {
            temp->setEast(nullptr);
        } else {
            if (temp->getValue() != e) {
                Node * newNode = new Node(e);
                insertAtEnd(newNode);
                temp->setEast(newNode);
            }
        }
        if (s == -1) {
            temp->setSouth(nullptr);
        } else {
            if (temp->getValue() != s) {
                Node * newNode = new Node(s);
                insertAtEnd(newNode);
                temp->setSouth(newNode);
            }
        }
        if (w == -1) {
            temp->setWest(nullptr);
        } else {
            if (temp->getValue() != w) {
                Node * newNode = new Node(w);
                insertAtEnd(newNode);
                temp->setWest(newNode);
            }
        }
    } else {//if not empty
        if (temp == nullptr) {//if node is nullptr
            temp = new Node(node);//assign node with basic constructor
            //North
            if (n == -1) {//if the value is -1
                temp->setNorth(nullptr);
                //set the temp node to nullprt and set its cardinal value
            } else {// if the value is not -1
                Node * newNode = findNode(n);//set a new node to find the value
                if (newNode == nullptr) {//if the value is not found
                    if (temp->getValue() != n) {
                        newNode = new Node(n);//create a new node with the value
                        insertAtEnd(newNode);//add node to list, helps with verifying if node exists or not
                        temp->setNorth(findNode(newNode->getValue()));//assign the cardinal variable with the new node
                        //assignment is done with find node to ensure node is in the list
                        //this algorithm is present throughout this whole function.
                    } else {
                        cout << "Error, unable to assign node to itself" << endl;
                    }
                } else {
                    if (temp->getValue() != newNode->getValue()) {
                        temp->setNorth(newNode);//if the node exists, point to it
                    } else {
                        cout << "Error, unable to assign node to itself" << endl;
                    }
                }
            }//This algorithm is used for the remaining 3 cardinals
            //East
            if (e == -1) {
                temp->setEast(nullptr);
            } else {// if the value is not -1
                Node * newNode = findNode(e);
                if (newNode == nullptr) {
                    if (temp->getValue() != e) {
                        newNode = new Node(e);
                        insertAtEnd(newNode);
                        temp->setEast(findNode(newNode->getValue()));
                    } else {
                        cout << "Error, unable to assign node to itself" << endl;
                    }
                } else {
                    if (temp->getValue() != newNode->getValue()) {
                        temp->setEast(newNode);//if the node exists, point to it
                    } else {
                        cout << "Error, unable to assign node to itself" << endl;
                    }
                }
            }
            //South
            if (s == -1) {
                temp->setSouth(nullptr);
            } else {
                Node * newNode = findNode(s);
                if (newNode == nullptr) {
                    if (temp->getValue() != s) {
                        newNode = new Node(s);
                        insertAtEnd(newNode);
                        temp->setSouth(findNode(newNode->getValue()));
                    } else {
                        cout << "Error, unable to assign node to itself" << endl;
                    }
                } else {
                    if (temp->getValue() != newNode->getValue()) {
                        temp->setSouth(newNode);//if the node exists, point to it
                    } else {
                        cout << "Error, unable to assign node to itself" << endl;
                    }
                }
            }
            //West
            if (w == -1) {
                temp->setWest(nullptr);
            } else {
                Node * newNode = findNode(w);
                if (newNode == nullptr) {
                    if (temp->getValue() != w) {
                        newNode = new Node(w);
                        insertAtEnd(newNode);
                        temp->setWest(findNode(newNode->getValue()));
                    } else {
                        cout << "Error, unable to assign node to itself" << endl;
                    }
                } else {
                    if (temp->getValue() != newNode->getValue()) {
                        temp->setWest(newNode);//if the node exists, point to it
                    } else {
                        cout << "Error, unable to assign node to itself" << endl;
                    }
                }
            }
            insertAtEnd(temp);
        } else {//for when node exists
            //North
            if (n == -1) {//if the value is -1
                temp->setNorth(nullptr);
            } else {
                Node * newNode = findNode(n);
                if (newNode == nullptr) {
                    if (temp->getValue() != n) {
                        newNode = new Node(n);
                        insertAtEnd(newNode);
                        temp->setNorth(findNode(newNode->getValue()));
                    } else {
                        cout << "Error, unable to assign node to itself" << endl;
                    }
                } else {
                    if (temp->getValue() != newNode->getValue()) {
                        temp->setNorth(newNode);//if the node exists, point to it
                    } else {
                        cout << "Error, unable to assign node to itself" << endl;
                    }
                }
            }//This algorithm is used for the remaining 3 cardinals
            //East
            if (e == -1) {
                temp->setEast(nullptr);
            } else {// if the value is not -1
                Node * newNode = findNode(e);
                if (newNode == nullptr) {
                    if (temp->getValue() != e) {
                        newNode = new Node(e);
                        insertAtEnd(newNode);
                        temp->setEast(findNode(newNode->getValue()));
                    } else {
                        cout << "Error, unable to assign node to itself" << endl;
                    }
                } else {
                    if (temp->getValue() != newNode->getValue()) {
                        temp->setEast(newNode);//if the node exists, point to it
                    } else {
                        cout << "Error, unable to assign node to itself" << endl;
                    }
                }
            }
            //South
            if (s == -1) {
                temp->setSouth(nullptr);
            } else {
                Node * newNode = findNode(s);
                if (newNode == nullptr) {
                    if (temp->getValue() != s) {
                        newNode = new Node(s);
                        insertAtEnd(newNode);
                        temp->setSouth(findNode(newNode->getValue()));
                    } else {
                        cout << "Error, unable to assign node to itself" << endl;
                    }
                } else {
                    if (temp->getValue() != newNode->getValue()) {
                        temp->setSouth(newNode);//if the node exists, point to it
                    } else {
                        cout << "Error, unable to assign node to itself" << endl;
                    }
                }
            }
            //West
            if (w == -1) {
                temp->setWest(nullptr);
            } else {
                Node * newNode = findNode(w);
                if (newNode == nullptr) {
                    if (temp->getValue() != w) {
                        newNode = new Node(w);
                        insertAtEnd(newNode);
                        temp->setWest(findNode(newNode->getValue()));
                    } else {
                        cout << "Error, unable to assign node to itself" << endl;
                    }
                } else {
                    if (temp->getValue() != newNode->getValue()) {
                        temp->setWest(newNode);//if the node exists, point to it
                    } else {
                        cout << "Error, unable to assign node to itself" << endl;
                    }
                }
            }
        }
    }
}

void Graph::insertAtHead(Node * aNode){//just assigns all values of aNode to head.
    m_head->setNext(aNode->getNext());
    m_head->setValue(aNode->getValue());
    m_head->setWest(aNode->getWest());
    m_head->setSouth(aNode->getSouth());
    m_head->setEast(aNode->getEast());
    m_head->setNorth(aNode->getNorth());
    m_head->setVisited(aNode->getVisited());
}

Node * Graph::findNode(int nodeValue){
    Node * result = nullptr;//stores the found node
    Node * current = m_head;//initialized iterator
    while (current != nullptr) {//Loop until end
        if (current->m_value == nodeValue) {//if the current value == the expected value
            result = current;//assign result node to current node
        }
        current = current->m_next;
    }
    return result;//will return a nullptr or a node of value nodeValue
}

bool Graph::findPath(int start, int end){
    clearVisited();
    clearResult();
    if (!empty()) {
        Node * startNode = findNode(start);//finds start node
        Node * endNode = findNode(end);//finds end node
        if (startNode != nullptr) {//checks for making sure valid input
            if (endNode != nullptr) {//same^
                findPath(startNode, end);//starts pathfinding if nodes are valid nodes
            }
        }
    }
    return true;
}

bool Graph::findPath(Node* aNode, int end){
    aNode->setVisited(true);//set visited to true for every run
    m_path.push(aNode->getValue());//add the path to queue
    if (aNode->getValue() == end) {//base case
        return true;
    }
    if (aNode->getNorth() != nullptr) {//check cardinal to see if its an node
        if (!aNode->getNorth()->getVisited()) {//make sure current node isnt visited already
            if (findPath(aNode->getNorth(), end)) {//recursive call, since function returns a boolean, use it for
                                                   //setting the condition of when to quit.
                return true;
            }
        }
    }//the same algorithm is in place for the rest of the cardinals
    if (aNode->getEast() != nullptr) {
        if (!aNode->getEast()->getVisited()) {
            if (findPath(aNode->getEast(), end)) {
                return true;
            }
        }
    }
    if (aNode->getSouth() != nullptr) {
        if (!aNode->getSouth()->getVisited()) {
            if (findPath(aNode->getSouth(), end)) {
                return true;
            }
        }
    }
    if (aNode->getWest() != nullptr) {
        if (!aNode->getWest()->getVisited()) {
            if (findPath(aNode->getWest(), end)) {
                return true;
            }
        }
    }
    m_path.pop();//only ever met if path not found
    return false;//return false if no path found.
}

void Graph::dump(){//prints out the stack
    stack<int> outputStack = m_path;
    stack<int> reverseoutput;//did this way because output was backwards
    while(!outputStack.empty()) {
        reverseoutput.push(outputStack.top());
        outputStack.pop();
    }
    string out = "";
    while (!reverseoutput.empty()) {
        out = out + to_string(reverseoutput.top())+" => ";
        reverseoutput.pop();
    }
    out = out + "END";
    cout << out << endl;
}

void Graph::clearResult(){
    while (!m_path.empty()) {//while the stack is not empty
        m_path.pop();//remove elements
    }
}

void Graph::clearVisited(){
    Node * current = m_head;
    while (current != nullptr) {
        //loop linked list, all nodes are linked by the m_next pointer
        // in a linear fashion, but graph computation does not use m_next,
        // but the cardinal variables
        current->m_visited = false;
        current = current->m_next;//iterator
    }
}

void Graph::buildGraph(string file){//builds the graph but from a function ig
    clearGraph();
    m_dataFile = file;
    loadData();
}

void Graph::clearGraph(){//empties the current graph, basically a destructor ngl
    Node * current = m_head;
    while (current != nullptr) {//Erase all cardinals and vars
        current->setNorth(nullptr);
        current->setEast(nullptr);
        current->setSouth(nullptr);
        current->setWest(nullptr);
        current->setVisited(false);
        current->setValue(0);
        current = current->m_next;
    }

    Node * eraser = m_head;
    Node * next = nullptr;
    while (eraser != nullptr) {//deleting pointers
        next = eraser->m_next;
        delete eraser;
        eraser = next;
    }
    m_dataFile = "";
    current = nullptr;
    eraser = nullptr;
    next = nullptr;
    delete current;
    delete eraser;
    delete next;
}

const Graph & Graph::operator=(const Graph & rhs){
    this->m_dataFile = rhs.m_dataFile;
    if (this != &rhs) {
        try {
            m_numNodes = rhs.m_numNodes;
            Node * current = rhs.m_head;
            while (current != nullptr) {//loop inserting nodes, using insert function so proper pointers happen
                int value = current->getValue();//seperating all cardinal values because there are some nullptr's which dont work with ints
                int n;
                int e;
                int s;
                int w;
                if (current->getNorth() == nullptr) {//If nullptr, pass through -1, to be rebuilt by insert()
                    n = -1;
                } else {
                    n = current->getNorth()->getValue();//If not, get actual value to be rebuilt by insert()
                }//algorithm is the same for the remaining 3 cardinal variables
                if (current->getEast() == nullptr) {
                    e = -1;
                } else {
                    e = current->getEast()->getValue();
                }
                if (current->getSouth() == nullptr) {
                    s = -1;
                } else {
                    s = current->getSouth()->getValue();
                }
                if (current->getWest() == nullptr) {
                    w = -1;
                } else {
                    w = current->getWest()->getValue();
                }
                this->insert(value, n, e, s, w);
                current = current->m_next;
            }
            this->m_path = rhs.m_path;//copy over path, stacks copy nicely
        } catch (exception &e) {
            cout << "\nSomething that could go wrong went wrong";//never have i run into this but why not
        }
    }
    return *this;
}

Node * Graph::getLastNode(Node * head) {// inserts node in m_next var
    Node * current = m_head;
    while (current->m_next != nullptr) {//iterate to the end of the linked list
        current = current->m_next;
    }
    return current;
}
void Graph::insertAtEnd(Node *aNode) {//adds node to the end of the list
    Node * a = getLastNode(m_head);
    a->setNext(aNode);
}
int Graph::getGraphSize() {//returns the size of the graph
    int count = 0;
    Node * current = m_head;
    while(current != nullptr) {
        count++;
        current = current->m_next;
    }
    return count;
}
void Graph::testPrintGraph() {//test function for printing graph to see if the correct values are present
    Node * current = m_head;
    while (current != nullptr) {
        cout << current->getValue() << " " << current->getNorth() << " " << current->getEast() << " " << current->getSouth() << " " << current->getWest() << endl;
        current = current->m_next;
    }
}

bool Graph::empty() const {		// is the list empty
    return m_head == nullptr;
}
