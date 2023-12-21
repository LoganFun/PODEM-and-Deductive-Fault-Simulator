#ifndef IN_OUT_H
#define IN_OUT_H

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
#include "Deductive.h"

#define D 2
#define D_bar 3
#define xx 4

int inputNum();
void rand_input(std::string& inputValue, std::string& label, std::vector<int>& inputPattern);
void input(std::string& inputValue, std::string& label, std::vector<int>& inputPattern);
void output_Original(std::vector<bool>& nodeValues, std::vector<int> outputGates);
char input_simmode();
void output_Fault(std::vector<std::vector<int>> nodeFaults_new, std::vector<int> outputGates);
void output_FaultCov(std::vector<std::vector<int>>& nodeFaults_new, std::vector<int>& nodeFaults_all, std::vector<int>& nodeFaults_temp, std::vector<int> outputGates);
int maxnode(CircuitGraph circuit);
void read_B_fault(std::string faultfilename,std::vector<std::vector<int>>& nodeFaults_new,std::vector<int>& nodeFaults_all,std::vector<int> &nodeValues);
char input_part();


#endif