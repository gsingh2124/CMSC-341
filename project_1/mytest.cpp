//UMBC - CSEE - CMSC 341 - Fall 2021 - Proj1
/*****************************************
** File:    mytest.cpp
** Project: CMSC 341 Project 1, Fall 2021
** Author:  Gurjinder Singh
** Date:    10/5/2021
** E-mail:  gsingh10@umbc.edu
***********************************************/
#include "graph.h"

class Tester{ // Tester class to implement test functions
public://the names of the functions accurately describe them, along with their cout statements
    void testNormalOperation(Graph aGraph);
    void testAlternativeConstructor(Graph aGraph);
    void testBuildEmptyGraph(Graph aGraph);
    void testNonExistantPath(Graph aGraph);
    void testExistingPath(Graph aGraph);
    void testFindItself(Graph aGraph);
    void testFindNonExistingStart(Graph aGraph);
    void testFindNonExistingEnd(Graph aGraph);
    void testDeepCopy(Graph aGraph, Graph bGraph);
};
int main(){
    Tester test;
    Graph aGraph("testdata.txt");

    test.testNormalOperation(aGraph);//################

    Graph bGraph;
    bGraph.buildGraph("testdata.txt");

    test.testAlternativeConstructor(bGraph);//########

    Graph cGraph;

    test.testBuildEmptyGraph(cGraph);//###############

    Graph dGraph("testdata.txt");

    test.testNonExistantPath(dGraph);//##############

    Graph eGraph("testdata.txt");

    test.testExistingPath(eGraph);//################

    Graph fGraph("testdata.txt");

    test.testFindItself(fGraph);//#################

    Graph gGraph("testdata.txt");

    test.testFindNonExistingStart(gGraph);//######

    Graph hGraph("testdata.txt");

    test.testFindNonExistingEnd(hGraph);//#######

    Graph iGraph("testdata.txt");
    Graph jGraph;

    test.testDeepCopy(jGraph, iGraph);//#


    return 0;
}
void Tester::testNormalOperation(Graph aGraph) {
    cout << "\nTesting for normal use case\n" << endl;
    aGraph.findPath(0,5);
    cout << "Finding the path from node 0 to node 5: " << endl;
    aGraph.dump();

    aGraph.findPath(2,2);
    cout << "Finding the path from node 2 to itself: " << endl;
    aGraph.dump();
    aGraph.findPath(4,0);
    cout << "Finding the path from node 4 to node 0: " << endl;
    aGraph.dump();
}
void Tester::testAlternativeConstructor(Graph aGraph) {
    cout << "\nTest for Alternative Constructor\n" << endl;
    aGraph.findPath(0,5);
    cout << "Finding the path from node 0 to node 5: " << endl;
    aGraph.dump();

    aGraph.findPath(2,2);
    cout << "Finding the path from node 2 to itself: " << endl;
    aGraph.dump();
    aGraph.findPath(4,0);
    cout << "Finding the path from node 4 to node 0: " << endl;
    aGraph.dump();
}
void Tester::testBuildEmptyGraph(Graph aGraph) {
    cout << "\nTest for building an empty graph\n" << endl;
    aGraph.findPath(0,5);
    cout << "Finding the path from node 0 to node 5: " << endl;
    aGraph.dump();

    aGraph.findPath(2,2);
    cout << "Finding the path from node 2 to itself: " << endl;
    aGraph.dump();
    aGraph.findPath(4,0);
    cout << "Finding the path from node 4 to node 0: " << endl;
    aGraph.dump();
}
void Tester::testNonExistantPath(Graph aGraph) {
    cout << "\nTest for a path that does not exist\n" << endl;
    aGraph.findPath(13,0);
    cout << "Finding the path from node 13 to node 3: " << endl;
    aGraph.dump();

}
void Tester::testExistingPath(Graph aGraph) {
    cout << "\nTest for a path that does exist\n" << endl;
    aGraph.findPath(0,5);
    cout << "Finding the path from node 0 to node 5: " << endl;
    aGraph.dump();
}
void Tester::testFindItself(Graph aGraph) {
    cout << "\nTest for pathfinding the same node\n" << endl;
    aGraph.findPath(5,5);
    cout << "Finding the path from node 5 to node 5: " << endl;
    aGraph.dump();
}
void Tester::testFindNonExistingStart(Graph aGraph) {
    cout << "\nTest for finding path of a non existent start\n" << endl;
    aGraph.findPath(16,5);
    cout << "Finding the path from node 16 to node 5: " << endl;
    aGraph.dump();
}
void Tester::testFindNonExistingEnd(Graph aGraph) {
    cout << "\nTest for finding path of a non existent end\n" << endl;
    aGraph.findPath(5,16);
    cout << "Finding the path from node 5 to node 16: " << endl;
    aGraph.dump();
}
void Tester::testDeepCopy(Graph aGraph, Graph bGraph) {
    cout << "\nTest for the deep copy constructor\n" << endl;
    cout << "\nAssigning now..\n." << endl;
    aGraph = bGraph;
    cout << "\nrhs before clear\n" << endl;
    bGraph.testPrintGraph();
    cout << "\nrhs after clear\n" << endl;
    bGraph.clearGraph();
    cout << "\ncopy after assigning and clearing rhs\n" << endl;
    aGraph.testPrintGraph();

    cout << "\nThe output shows memory addresses of certain pointers, they are clearly different.\n" << endl;
}