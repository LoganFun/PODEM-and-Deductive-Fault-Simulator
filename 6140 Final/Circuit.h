#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <random>

#define D 2
#define D_bar 3
#define xx 4

// Define logic gate types
enum GateType {
    INV,
    BUF,
    AND,
    OR,
    NOR,
    NAND
};

// Define a structure to represent a logic gate
class LogicGate {
public:
    GateType type;            // Gate type
    std::vector<int> inputs;  // Input node IDs
    int output;               // Output node ID
    bool finish;
    bool ready; 
    bool fault_Ready; //Ready to fault sim 
    bool fault_Finish;
    bool inversion;

    LogicGate() : finish(false), ready(false) {}
};

// Define a Circuit Graph
class CircuitGraph 
{
public:
    std::vector<LogicGate> gates;
    std::vector<std::pair<int, int>> connections; 
    std::vector<int> inputNodes;
    std::vector<int> outputNodes;
};



#endif