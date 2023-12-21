#ifndef PODEMSIM_H
#define PODEMSIM_H

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
#include "Podem.h"

#define D 2
#define D_bar 3
#define xx 4

extern std::vector<std::vector<int>> andMatrix;

extern std::vector<std::vector<int>> orMatrix;

extern std::vector<int> inverseMatrix;

bool is_Ready_Podem(std::vector<int>& nodeValues, std::vector<bool>& nodeValid, LogicGate& gate) ;

int Podem_sim_AND (const std::vector<int>& nodeValues,std::vector<bool>& nodeValid, const LogicGate& gate, int fault_here);

int Podem_sim_NAND (const std::vector<int>& nodeValues,std::vector<bool>& nodeValid, const LogicGate& gate, int fault_here);

int Podem_sim_OR (const std::vector<int>& nodeValues,std::vector<bool>& nodeValid, const LogicGate& gate, int fault_here);

int Podem_sim_NOR (const std::vector<int>& nodeValues,std::vector<bool>& nodeValid, const LogicGate& gate, int fault_here);

int Podem_sim_INV (const std::vector<int>& nodeValues,std::vector<bool>& nodeValid, const LogicGate& gate, int fault_here);

int Podem_sim_BUF (const std::vector<int>& nodeValues,std::vector<bool>& nodeValid, const LogicGate& gate, int fault_here);


#endif