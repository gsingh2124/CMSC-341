// CMSC 341 - Fall 2021 - Project 3 - MQueue Class 
/*****************************************
** File:    mqueue.cpp
** Project: CMSC 341 Project 3, Fall 2021
** Author:  Gurjinder Singh
** Date:    11/11/2021
** E-mail:  gsingh10@umbc.edu
***********************************************/
#include "mqueue.h"
using namespace std;
using std::domain_error;

MQueue::MQueue(prifn_t priFn)
{
  m_heap = nullptr;
  m_size = 0;
  m_priorFunc = priFn;
}

MQueue::~MQueue()//Does not work, causes segfaults at end
{
    //clear();
    //m_head = nullptr;
    //m_size = 0;
    //m_priorFunc = nullptr;
}

MQueue::MQueue(const MQueue& rhs)
{
    copyRecurs(rhs.m_heap);
}

void MQueue::copyRecurs(Node *root) {
    if (m_heap != nullptr) {
        copyRecurs(root->m_left);
        {
            Order *temp = new Order(root->getOrder().getCustomer(),//copys all values of order from current node to new node
                                    root->getOrder().getPMPriority(),
                                    root->getOrder().getMaterialAvailability(),
                                    root->getOrder().getQuantity(),
                                    root->getOrder().getArrivalTime());
            this->insertOrder(*temp);//inserts the new node
        }
        copyRecurs(root->m_right);
    }
}

MQueue& MQueue::operator=(const MQueue& rhs)
{
    MQueue *temp = assignmentRecurs(rhs.m_heap, *temp);

}

MQueue* MQueue::assignmentRecurs(Node* ptr, MQueue& queue) {//recursive implementation of assignment operator
    if (ptr != nullptr) {
        assignmentRecurs(ptr->m_left, queue);
        {
            queue.insertOrder(ptr->getOrder());
        }
        assignmentRecurs(ptr->m_right, queue);
    }
}

void MQueue::insertOrder(const Order& input) {//inserts a new order into the heap
    Node* insert = new Node(input);
    m_heap = merge(insert, m_heap);
    m_size++;
}

Order MQueue::getNextOrder() {//returns the next order, attempts to remove root node (does not remove root node)
    Order temp = m_heap->getOrder();
    Node* ptr = m_heap;
    //m_heap->m_left = nullptr;
    //m_heap->m_right = nullptr;
    //delete m_heap;
    m_size--;
    m_heap = merge(m_heap->m_left, m_heap->m_right);
    return temp;
}

void MQueue::mergeWithQueue(MQueue& rhs) {

    if (m_heap == rhs.m_heap) {//check for self assignment
        return;
    } else {
        m_heap = merge(m_heap, rhs.m_heap);
        m_size = m_size + rhs.m_size;
    }
}
Node* MQueue::merge(Node* q1, Node* q2) {
    prifn_t fn = getPriorityFn();//print fn to use for comparing priorities
    if (q1 == nullptr) {
        return q2;
    }
    if (q2 == nullptr) {
        return q1;
    }
    //if (q1->getOrder().getPMPriority() > q2->getOrder().getPMPriority()) {
    //    Node* temp = q1;
    //    q1 = q2;
    //    q2 = temp;
    //}
    if (fn(q1->getOrder()) > fn(q2->getOrder())) {
        Node* temp = q1;
        q1 = q2;
        q2 = temp;
    }
    Node* temp = q1->m_right;
    q1->m_right= q1->m_left;
    q1->m_left = temp;
    q1->m_left = merge(q2, q1->m_left);

    return q1;
}

void MQueue::clear() {
    while (m_heap != nullptr) {
        getNextOrder();
    }
    //clearRecurs(m_heap);
}

void MQueue::clearRecurs(Node *ptr) {//attempts to clear the heap recursively given the root node
    if (ptr != nullptr) {
        clearRecurs(ptr->m_left);
        clearRecurs(ptr->m_right);
        delete ptr;
        m_heap = merge(ptr->m_left, ptr->m_right);
        m_size--;
    }
}

int MQueue::numOrders() const
{
  return m_size;//returnt he size of the heap
}

void MQueue::printOrderQueue() const {//function calls recursive function for output
    if (m_heap == nullptr) {
        return;
    } else {
        printQueueInOrder(m_heap);
    }
}
void MQueue::printQueueInOrder(Node* root) const{
    prifn_t fn = getPriorityFn();//saved for use in printing
    if (root == nullptr) {
        return;
    } else {
        cout <<  "[" + to_string(fn(root->getOrder())) + "] " << root->getOrder() << endl;
        printQueueInOrder(root->m_left);
        printQueueInOrder(root->m_right);//recursively print the tree from left middle right
    }
}

prifn_t MQueue::getPriorityFn() const {
  return m_priorFunc;//just returns prio fn
}

void MQueue::setPriorityFn(prifn_t priFn) {
    int size = m_size;//saves the size as it changes
    MQueue *newHeap = new MQueue(priFn);
    for (int i = 0; i < size; i++) {
        newHeap->insertOrder(getNextOrder());//contonuously inserts orders
    }
    m_heap = newHeap->m_heap;//assigns new heap with new prifn
    m_priorFunc = priFn;
    m_size = size;
}

// overloaded insertion operator for Order class
ostream& operator<<(ostream& sout, const Order& order) {
  sout << "Order: " << order.getCustomer() << ", PM priority: " << order.getPMPriority()
       << ", quantity: " << order.getQuantityValue() 
       << ", material availability: " << order.getAvailabilityProbablity() << "%"
       << ", arrival time: " << order.getArrivalTime();
  return sout;
}

// overloaded insertion operator for Node class
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getOrder();
  return sout;
}

// for debugging
void MQueue::dump() const
{
  if (m_size == 0) {
    cout << "Empty skew heap.\n" ;
  } else {
    dump(m_heap);
    cout << endl;
  }
}

// for debugging
void MQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    cout << m_priorFunc(pos->m_order) << ":" << pos->m_order.getCustomer();
    dump(pos->m_right);
    cout << ")";
  }
}

