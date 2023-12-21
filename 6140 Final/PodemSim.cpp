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

#define ZERO = 0;
#define ONE = 1;
#define D 2
#define D_bar 3
#define xx 4

std::vector<std::vector<int>> andMatrix = 
{
    {0, 0, 0, 0, 0},
    {0, 1, D, D_bar, xx},
    {0, D, D, 0, xx},
    {0, D_bar, 0, D_bar, xx},
    {0, xx, xx, xx, xx}
};

std::vector<std::vector<int>> orMatrix = 
{
    {0, 1, D, D_bar, xx},
    {1, 1, 1, 1, 1},
    {D, 1, D, 1, xx},
    {D_bar, 1, 1, D_bar, xx},
    {xx, 1, xx, xx, xx}
};

std::vector<int> inverseMatrix = 
{
    1,
    0,
    D_bar,
    D,
    xx
};

bool is_Ready_Podem(std::vector<int>& nodeValues, std::vector<bool>& nodeValid, LogicGate& gate) 
{
    // if (gate.output == 14)
    // {
    //     std::cout<<"    circuit.gates[6].output: " << gate.output << std::endl;
    //     std::cout<<"    circuit.gates[6].inputs[0]: " << gate.inputs[0] << std::endl;
    //     std::cout<<"    circuit.gates[6].inputs[1]: " << gate.inputs[1] << std::endl;
    //     std::cout<<"    nodeValues[circuit.gates[6].output] " << nodeValues[gate.output] << std::endl;
    //     std::cout<<"    nodeValues[circuit.gates[6].inputs[0]] " << nodeValues[gate.inputs[0]] << std::endl;
    //     std::cout<<"    nodeValues[circuit.gates[6].inputs[1]] " << nodeValues[gate.inputs[1]] << std::endl;
    //     std::cout<<"    nodeValid[circuit.gates[6].inputs[1]] " << nodeValid[gate.inputs[1]] << std::endl;
    //     std::cout<<"    nodeValid[circuit.gates[6].inputs[1]] " << nodeValid[gate.inputs[1]] << std::endl;
    // }
    
    switch (gate.type)
    {
        case AND:
            if (nodeValues[gate.inputs[0]]==0||nodeValues[gate.inputs[1]]==0)
            {
                return true;
            }

            if (nodeValid[gate.inputs[0]]==1 && nodeValid[gate.inputs[1]]==1)
            {
                return true;
            }           
            break;
        case NAND:
            if (nodeValues[gate.inputs[0]]==0||nodeValues[gate.inputs[1]]==0)
            {
                return true;
            }

            if (nodeValid[gate.inputs[0]]==1 && nodeValid[gate.inputs[1]]==1)
            {
                // if (gate.output == 14)
                // {
                //     std::cout<<"    Test Test 2"  << std::endl;
                //     std::cout<<"    nodeValues[gate.output] " << nodeValues[gate.output] << std::endl;
                //     std::cout<<"    nodeValues[gate.inputs[0]] " << nodeValues[gate.inputs[0]] << std::endl;
                //     std::cout<<"    nodeValues[gate.inputs[1]] " << nodeValues[gate.inputs[1]] << std::endl;
                //     std::cout<<"    nodeValid[gate.inputs[1]] " << nodeValid[gate.inputs[0]] << std::endl;
                //     std::cout<<"    nodeValid[gate.inputs[1]] " << nodeValid[gate.inputs[1]] << std::endl;  
                // }
                return true;
            }   
            break;
        case OR:
            if (nodeValues[gate.inputs[0]]==1||nodeValues[gate.inputs[1]]==1)
            {
                return true;
            }

            if (nodeValid[gate.inputs[0]]==1 && nodeValid[gate.inputs[1]]==1)
            {
                return true;
            }   
            break;
        case NOR:
            if (nodeValues[gate.inputs[0]]==1||nodeValues[gate.inputs[1]]==1)
            {
                return true;
            }

            if (nodeValid[gate.inputs[0]]==1 && nodeValid[gate.inputs[1]]==1)
            {
                return true;
            }
            break;
        case INV:
            if (nodeValid[gate.inputs[0]]==1)
            {
                return true;
            }
            break;
        case BUF:
            if (nodeValid[gate.inputs[0]]==1)
            {
                return true;
            }
            break;
    }

    return false;    
}

int Podem_sim_AND (const std::vector<int>& nodeValues,std::vector<bool>& nodeValid, const LogicGate& gate, int fault_here)
{
    // Whether there is the fault 
    // For normal Cases
    if (!fault_here)
    {
        if (nodeValues[gate.inputs[0]]==0||nodeValues[gate.inputs[1]]==0)
        {
            return 0;
        }
        else if (nodeValid[gate.inputs[0]]==1 && nodeValid[gate.inputs[1]]==1)
        {
            return andMatrix[nodeValues[gate.inputs[0]]][nodeValues[gate.inputs[1]]];
        }  
        else
        {
            std::cout << "Sim Wrong at AND: Input Situation wrong. " << std::endl;
        }
    }
    // For Special Fault at here cases
    else
    {

        if (nodeValues[gate.inputs[0]]==0||nodeValues[gate.inputs[1]]==0)
        {
            return D_bar;
        }
        else if (nodeValid[gate.inputs[0]]==1 && nodeValid[gate.inputs[1]]==1)
        {
            int return_value;
            return_value = andMatrix[nodeValues[gate.inputs[0]]][nodeValues[gate.inputs[1]]];
            if (return_value==0)
            {
                return D_bar;
            }
            else if(return_value==1)
            {
                return D;
            }
            else
            {
                std::cout << "Sim Wrong at AND: Fault at injection node . " << std::endl;
            }
        }  
        else
        {
            std::cout << "Sim Wrong at AND: Input Situation wrong. " << std::endl;
        }        
    }  
    std::cout << "Sim Wrong " << std::endl;
    return 0;
}

int Podem_sim_NAND (const std::vector<int>& nodeValues,std::vector<bool>& nodeValid, const LogicGate& gate, int fault_here)
{
    // Whether there is the fault 
    // For normal Cases
    //std::cout<< " Get into the NAND Simulation " << std::endl;

    if (!fault_here)
    {
        //std::cout<< " Get into the NAND Simulation without fault here" << std::endl;
        if (nodeValues[gate.inputs[0]]==0||nodeValues[gate.inputs[1]]==0)
        {
            //std::cout<< " Simulate NAND with a controllong value" << std::endl;
            return 1;
        }
        else if (nodeValid[gate.inputs[0]]==1 && nodeValid[gate.inputs[1]]==1)
        {
            // std::cout<< " Simulate NAND with two valid input" << std::endl;
            // std::cout<< " nodeValues[gate.inputs[0]]:" << nodeValues[gate.inputs[0]] << std::endl;
            // std::cout<< " nodeValues[gate.inputs[1]]:" << nodeValues[gate.inputs[1]] << std::endl;
            int result =andMatrix[nodeValues[gate.inputs[0]]][nodeValues[gate.inputs[1]]];
            // std::cout<< " And result: "<< result << std::endl;
            // std::cout<< " inverseMatrix[result]: "<< inverseMatrix[result] << std::endl;
            return inverseMatrix[result];
        }  
        else
        {
            std::cout << "Sim Wrong at NAND: Input Situation wrong. " << std::endl;
        }
    }
    // For Special Fault at here cases
    else
    {
        //std::cout<< " Get into the NAND Simulation with fault here" << std::endl;

        if (nodeValues[gate.inputs[0]]==0||nodeValues[gate.inputs[1]]==0)
        {
            return D;
        }
        else if (nodeValid[gate.inputs[0]]==1 && nodeValid[gate.inputs[1]]==1)
        {
            int return_value;
            return_value = andMatrix[nodeValues[gate.inputs[0]]][nodeValues[gate.inputs[1]]];
            return_value = inverseMatrix[return_value];
            if (return_value==0)
            {
                return D_bar;
            }
            else if(return_value==1)
            {
                return D;
            }
            else
            {
                std::cout << "Sim Wrong at NAND: Fault at injection node . " << std::endl;
            }
        }  
        else
        {
            std::cout << "Sim Wrong at NAND: Input Situation wrong. " << std::endl;
        }        
    }
        std::cout << "Sim Wrong " << std::endl;
    return 0;  
}

int Podem_sim_OR (const std::vector<int>& nodeValues,std::vector<bool>& nodeValid, const LogicGate& gate, int fault_here)
{
    // Whether there is the fault 
    // For normal Cases
    if (!fault_here)
    {
        if (nodeValues[gate.inputs[0]]==1||nodeValues[gate.inputs[1]]==1)
        {
            return 1;
        }
        else if (nodeValid[gate.inputs[0]]==1 && nodeValid[gate.inputs[1]]==1)
        {
            return orMatrix[nodeValues[gate.inputs[0]]][nodeValues[gate.inputs[1]]];
        }  
        else
        {
            std::cout << "Sim Wrong at OR: Input Situation wrong. " << std::endl;
        }
    }
    // For Special Fault at here cases
    else
    {
        if (nodeValues[gate.inputs[0]]==1||nodeValues[gate.inputs[1]]==1)
        {
            return D;
        }
        else if (nodeValid[gate.inputs[0]]==1 && nodeValid[gate.inputs[1]]==1)
        {
            int return_value;
            return_value = orMatrix[nodeValues[gate.inputs[0]]][nodeValues[gate.inputs[1]]];
            if (return_value==0)
            {
                return D_bar;
            }
            else if(return_value==1)
            {
                return D;
            }
            else
            {
                std::cout << "Sim Wrong at OR: Fault at injection node . " << std::endl;
            }
        }  
        else
        {
            std::cout << "Sim Wrong at OR: Input Situation wrong. " << std::endl;
        }        
    }
    std::cout << "Sim Wrong " << std::endl;
    return 0; 
}

int Podem_sim_NOR (const std::vector<int>& nodeValues,std::vector<bool>& nodeValid, const LogicGate& gate, int fault_here)
{
    // Whether there is the fault 
    // For normal Cases
    if (!fault_here)
    {
        if (nodeValues[gate.inputs[0]]==1||nodeValues[gate.inputs[1]]==1)
        {
            return 0;
        }
        else if (nodeValid[gate.inputs[0]]==1 && nodeValid[gate.inputs[1]]==1)
        {
            int result = orMatrix[nodeValues[gate.inputs[0]]][nodeValues[gate.inputs[1]]];
            result = inverseMatrix[result];
            return result;
        }  
        else
        {
            std::cout << "Sim Wrong at NOR: Input Situation wrong. " << std::endl;
        }
    }
    // For Special Fault at here cases
    else
    {
        if (nodeValues[gate.inputs[0]]==1||nodeValues[gate.inputs[1]]==1)
        {
            return D_bar;
        }
        else if (nodeValid[gate.inputs[0]]==1 && nodeValid[gate.inputs[1]]==1)
        {
            int return_value;
            return_value = orMatrix[nodeValues[gate.inputs[0]]][nodeValues[gate.inputs[1]]];
            return_value = inverseMatrix[return_value];

            if (return_value==0)
            {
                return D_bar;
            }
            else if(return_value==1)
            {
                return D;
            }
            else
            {
                std::cout << "Sim Wrong at NOR: Fault at injection node . " << std::endl;
            }
        }  
        else
        {
            std::cout << "Sim Wrong at NOR: Input Situation wrong. " << std::endl;
        }        
    }
    std::cout << "Sim Wrong " << std::endl;
    return 0; 
}

int Podem_sim_INV (const std::vector<int>& nodeValues,std::vector<bool>& nodeValid, const LogicGate& gate, int fault_here)
{
    // Whether there is the fault 
    // For normal Cases
    if (!fault_here)
    {
        int result = nodeValues[gate.inputs[0]];
        result = inverseMatrix[result];
        return result;
    }
    else
    {
        int result = nodeValues[gate.inputs[0]];
        result = inverseMatrix[result];
        if (result==0)
        {
            return D_bar;
        }
        else if(result==1)
        {
            return D;
        }
        else
        {
            std::cout << "Sim Wrong at INV: Fault at injection node . " << std::endl;
            std::cout << "gate.finish: " << gate.finish<< std::endl;
            std::cout << "gate.output: " << gate.output<< std::endl;
        }
    }
    std::cout << "Sim Wrong " << std::endl;
    return 0;
}

int Podem_sim_BUF (const std::vector<int>& nodeValues,std::vector<bool>& nodeValid, const LogicGate& gate, int fault_here)
{
    // Whether there is the fault 
    // For normal Cases
    if (!fault_here)
    {
        int result = nodeValues[gate.inputs[0]];
        return result;
    }
    else
    {
        int result = nodeValues[gate.inputs[0]];
        if (result==0)
        {
            return D;
        }
        else if(result==1)
        {
            return D_bar;
        }
        else
        {
            std::cout << "Sim Wrong at BUF: Fault at injection node . " << std::endl;
        }
    }
    std::cout << "Sim Wrong " << std::endl;
    return 0;
}