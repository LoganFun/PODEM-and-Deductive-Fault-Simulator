#ifndef DEDUCTIVE_H
#define DEDUCTIVE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <random>
#include "Circuit.h"
#include "Gatesim.h"
#include "In_out.h"

#define D 2
#define D_bar 3
#define xx 4

// Function prototypes
bool isFaultReady(std::vector<bool>& nodeFaultValid, LogicGate& gate);
std::vector<int> intersection_new(std::vector<int> vec1, std::vector<int> vec2);
std::vector<int> difference_new(std::vector<int>& vec1, std::vector<int>& vec2);
std::vector<int> union_sets_new(std::vector<int>& vec1, std::vector<int>& vec2);

void simulateAND_F(std::vector<std::vector<int>>& nodeFaults_new, const LogicGate& gate, const std::vector<bool>& nodeValues);
void simulateNAND_F(std::vector<std::vector<int>>& nodeFaults_new, const LogicGate& gate, const std::vector<bool>& nodeValues);
void simulateOR_F(std::vector<std::vector<int>>& nodeFaults_new, const LogicGate& gate, const std::vector<bool>& nodeValues);
void simulateNOR_F(std::vector<std::vector<int>>& nodeFaults_new, const LogicGate& gate, const std::vector<bool>& nodeValues);
void simulateINV_F(std::vector<std::vector<int>>& nodeFaults_new, const LogicGate& gate, const std::vector<bool>& nodeValues);
void simulateBUF_F(std::vector<std::vector<int>>& nodeFaults_new, const LogicGate& gate, const std::vector<bool>& nodeValues);

void fault_sim_A(CircuitGraph& circuit, std::vector<bool>& nodeFaultValid, 
    //std::vector<std::vector<std::vector<int>>> nodeFaults,
    std::vector<std::vector<int>>& nodeFaults_new,
    std::vector<int>& nodeFaults_all,
    std::vector<int>& outputGates, std::vector<int> &nodeValues,
    std::vector<int>& inputPattern, std::vector<int> &inputGates,
    int maxnode);

void fault_sim_B(CircuitGraph& circuit, std::vector<bool>& nodeFaultValid, 
    //std::vector<std::vector<std::vector<int>>> nodeFaults,
    std::vector<std::vector<int>>& nodeFaults_new,
    std::vector<int>& nodeFaults_all,
    std::vector<int>& outputGates, std::vector<int> &nodeValues,
    std::vector<int>& inputPattern, std::vector<int> &inputGates,
    std::string faultfilename);
   
#endif
