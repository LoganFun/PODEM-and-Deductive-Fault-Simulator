#ifndef PODEM_H
#define PODEM_H

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
#include "In_out.h"
#include "PodemSim.h"

#define D 2
#define D_bar 3
#define xx 4

extern std::vector<int> transMatrix;

extern std::vector<int> inverseMatrix2;

bool podem(CircuitGraph& circuit,std::vector<int> outputGates,std::vector<int>& nodeValues, std::vector<bool>& nodeValid,
            std::vector<int>& D_F,int nodefault, int stuck_value, std::vector<int> inputGates
            );

int Objective(CircuitGraph& circuit,std::vector<int>& nodeValues, int nodefault, int stuck_value, std::vector<bool>& nodeValid);

int Backtrace(CircuitGraph& circuit,std::vector<int>& nodeValues, int nodefault, int stuck_value, int target, std::vector<int> inputGates);

void Imply(CircuitGraph& circuit, std::vector<int>& nodeValues, int input_set, std::vector<bool>& nodeValid,int nodefault, int stuck_value);

bool output_check(std::vector<int> outputGates,std::vector<int> nodeValues);

bool possible_check(CircuitGraph& circuit, std::vector<int>& D_F,std::vector<int>& nodeValues, int nodefault, int stuck_value,std::vector<bool>& nodeValid);

std::vector<int> D_F_Search(CircuitGraph& circuit, std::vector<int>& nodeValues);

bool D_F_Check(std::vector<int>& nodeValues, LogicGate& gate);

int concatenateIntegers(int a, int b);

int check_signle(CircuitGraph& circuit,std::vector<int>& nodeValues,std::vector<bool>& nodeValid,int nodefault, int stuck_value );

#endif