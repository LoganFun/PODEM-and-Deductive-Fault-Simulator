#ifndef LOGICGATE_H
#define LOGICGATE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <random>
#include "Circuit.h" // Assuming the LogicGate class is defined in this header

#define D 2
#define D_bar 3
#define xx 4

// Function declarations
int simulateAND(const std::vector<int>& nodeValues, const LogicGate& gate);
int simulateOR(const std::vector<int>& nodeValues, const LogicGate& gate);
int simulateNOR(const std::vector<int>& nodeValues, const LogicGate& gate);
int simulateNAND(const std::vector<int>& nodeValues, const LogicGate& gate);
int simulateINV(const std::vector<int>& nodeValues, const LogicGate& gate);
int simulateBUF(const std::vector<int>& nodeValues, const LogicGate& gate);
bool isReady(std::vector<bool> nodeValid, LogicGate& gate);

CircuitGraph parseCircuitFile(const std::string& filePath, std::vector<int>& inputGates,
    std::vector<int>& outputGates,std::vector<int> nodeValues, std::vector<bool> nodeValid);

void simulateCircuit( CircuitGraph& circuit, std::vector<int>& inputPattern, 
    std::vector<int> &inputGates,std::vector<int> &nodeValues, 
    std::vector<bool>& nodeValid);

#endif // LOGICGATE_H