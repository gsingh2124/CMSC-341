// CMSC 341 - Fall 2021 - Project 3
/*****************************************
** File:    mytest.cpp
** Project: CMSC 341 Project 3, Fall 2021
** Author:  Gurjinder Singh
** Date:    11/11/2021
** E-mail:  gsingh10@umbc.edu
***********************************************/
#include "mqueue.h"
#include <cstdlib>
#include <ctime>
using namespace std;
// Priority functions compute an integer priority for an order.  Internal
// computations may be floating point, but must return an integer.
int priorityFn1(const Order &order);
int priorityFn2(const Order &order);

int main() {
    int randPriority;
    clock_t start,stop;
    double T = 0.0;
  try{

    MQueue queue1(priorityFn1);
    MQueue queue2(priorityFn1);


    // Create some test orders and insert them into the queue
    //Order(string customer, int priority, int material, int quantity, int arrival)
    Order order1("Ford",1,1,1,20);
    Order order2("Honda",5,1,2,21);
    Order order3("Toyota",2,0,0,22);
    Order order4("GM",4,3,2,23);

    queue1.insertOrder(order3);
    queue1.insertOrder(order1);
    queue1.insertOrder(order2);
    queue1.insertOrder(order4);


    std::cout << "\nqueue1 dump after inserting 4 orders (Toyota, Honda, Ford, GM):" << std::endl;
    queue1.dump();

    Order order5("Ferrari",6,3,2,1);
    Order order6("Hyundai",3,0,0,10);

    queue2.insertOrder(order5);
    queue2.insertOrder(order6);

    std::cout << "\nqueue2 dump after inserting 2 students (Ferrari, Hyundai):" << std::endl;
    queue2.dump();

    queue1.mergeWithQueue(queue2);

    // Print the queue, change priority function, print again, change
    // again, print again.  Changing the priority function should cause
    // the heap to be rebuilt.

    std::cout << std::endl << "Contents of queue1 after merging with queue2:\n";
    queue1.printOrderQueue();
    std::cout << "\nDump queue1 after merging with queue2:\n";
    queue1.dump();

    queue1.setPriorityFn(priorityFn2);

    std::cout << std::endl << "Contents of queue1 after changing the priority function to priorityFn2:\n";
    queue1.printOrderQueue();
    std::cout << "\nDump queue1 after changing the priority function to priorityFn2:\n";
    queue1.dump();
    
    queue1.setPriorityFn(priorityFn1);

    std::cout << std::endl << "Contents of queue1 after changing the priority function back to priorityFn1:\n";
    queue1.printOrderQueue();
    std::cout << "\nDump queue1 after changing the priority function back to priorityFn1:\n";
    queue1.dump();

    // Remove and print orders in priority order

    std::cout << "\nFinal queue1 in priority order:\n";
    while (queue1.numOrders() > 0) {
      Order order = queue1.getNextOrder();
      // this will be printed in the ascending order (sorted)
      std::cout << "[" << priorityFn1(order) << "] " << order << std::endl;


      cout << "---Implemented tests---" << endl;
      cout << "Testing efficency of insertion (100,000 items)" << endl;
      cout << "Testing with random priorities, using priofn2" << endl;

      MQueue queue3(priorityFn2);
      start = clock();
    for (int i = 100000; i > 0; i--) {
        randPriority = (rand() % 58) + 1;
        Order order("Test",randPriority,0,0,0);
        queue3.insertOrder(order);
    }
    stop = clock();
    T = stop - start;
    cout << "Insertion of 100,000 items took " << T << "clock ticks (" << T/CLOCKS_PER_SEC <<")" << endl;

    cout << endl << endl << endl;


    cout << "Testing efficency of insertion (200,000 items)" << endl;
    cout << "Testing with random priorities, using priofn2" << endl;

    MQueue queue4(priorityFn2);
    start = clock();
    for (int i = 200000; i > 0; i--) {
        randPriority = (rand() % 58) + 1;
        Order order("Test",randPriority,0,0,0);
        queue4.insertOrder(order);
    }
    stop = clock();
    T = stop - start;
    cout << "Insertion of 200,000 items took " << T << "clock ticks (" << T/CLOCKS_PER_SEC <<")" << endl;

        cout << endl << endl << endl;

    cout << "Testing efficency of insertion (400,000 items)" << endl;
    cout << "Testing with random priorities, using priofn2" << endl;

    MQueue queue5(priorityFn2);
    start = clock();
    for (int i = 400000; i > 0; i--) {
        randPriority = (rand() % 58) + 1;
        Order order("Test",randPriority,0,0,0);
        queue5.insertOrder(order);
    }
    stop = clock();
    T = stop - start;
    cout << "Insertion of 400,000 items took " << T << "clock ticks (" << T/CLOCKS_PER_SEC <<")" << endl;

        cout << endl << endl << endl;

        cout << "Testing efficency of insertion (800,000 items)" << endl;
        cout << "Testing with random priorities, using priofn2" << endl;

        MQueue queue6(priorityFn2);
        start = clock();
        for (int i = 800000; i > 0; i--) {
            randPriority = (rand() % 58) + 1;
            Order order("Test",randPriority,0,0,0);
            queue6.insertOrder(order);
        }
        stop = clock();
        T = stop - start;
        cout << "Insertion of 800,000 items took " << T << "clock ticks (" << T/CLOCKS_PER_SEC <<")" << endl;

        cout << endl << endl << endl;

        cout << "Testing Efficency of  merge operation (100,000 items)" << endl;

        start = clock();
        MQueue queue7(priorityFn2);
        for (int i = 100000; i > 0; i--) {
            randPriority = (rand() % 58) + 1;
            Order order("Test",randPriority,0,0,0);
            queue7.insertOrder(order);
        }
        MQueue queue8(priorityFn2);
        for (int i = 100000; i > 0; i--) {
            randPriority = (rand() % 58) + 1;
            Order order("Test",randPriority,0,0,0);
            queue8.insertOrder(order);
        }
        stop = clock();
        T = stop-start;
        cout << "merging of 2x 100,000 items took " << T << "clock ticks (" << T/CLOCKS_PER_SEC <<")" << endl;

        cout << endl << endl << endl;

        cout << "Testing Efficency of  merge operation (200,000 items)" << endl;

        start = clock();
        MQueue queue9(priorityFn2);
        for (int i = 200000; i > 0; i--) {
            randPriority = (rand() % 58) + 1;
            Order order("Test",randPriority,0,0,0);
            queue9.insertOrder(order);
        }
        MQueue queue10(priorityFn2);
        for (int i = 200000; i > 0; i--) {
            randPriority = (rand() % 58) + 1;
            Order order("Test",randPriority,0,0,0);
            queue10.insertOrder(order);
        }
        stop = clock();
        T = stop-start;
        cout << "merging of 2x 200,000 items took " << T << "clock ticks (" << T/CLOCKS_PER_SEC <<")" << endl;

        cout << endl << endl << endl;

        cout << "Testing Efficency of  merge operation (400,000 items)" << endl;

        start = clock();
        MQueue queue11(priorityFn2);
        for (int i = 400000; i > 0; i--) {
            randPriority = (rand() % 58) + 1;
            Order order("Test",randPriority,0,0,0);
            queue11.insertOrder(order);
        }
        MQueue queue12(priorityFn2);
        for (int i = 400000; i > 0; i--) {
            randPriority = (rand() % 58) + 1;
            Order order("Test",randPriority,0,0,0);
            queue12.insertOrder(order);
        }
        stop = clock();
        T = stop-start;
        cout << "merging of 2x 400,000 items took " << T << "clock ticks (" << T/CLOCKS_PER_SEC <<")" << endl;

        cout << endl << endl << endl;

        cout << "Testing copy constructor" << endl;
        cout << "Dump of heap to copy" << endl;

        MQueue testcopy(priorityFn2);

        for (int i = 15; i > 0; i--) {
            randPriority = (rand() % 58) + 1;
            Order order("Test",randPriority,0,0,0);
            testcopy.insertOrder(order);
        }
        testcopy.printOrderQueue();
        cout << "Dump of copy" << endl;
        MQueue newcopy(testcopy);
        newcopy.printOrderQueue();
  }
  }
    catch(std::out_of_range &e){
      cout << e.what() << endl;
  }
  return 0;
}

int priorityFn1(const Order & order) {
  //priority value is determined based on some criteria
  //priority value falls in the range [1-58]
  //the smaller value means the higher priority
  int priority = order.getMaterialAvailability() + order.getQuantity() + order.getArrivalTime();
  return priority;
}

int priorityFn2(const Order & order) {
  //priority is determined by a production manager
  //priority value falls in the range [1-58]
  //the smaller value means the higher priority
  return order.getPMPriority();
}