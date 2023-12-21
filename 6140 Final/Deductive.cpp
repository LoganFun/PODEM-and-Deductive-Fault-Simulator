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

bool isFaultReady(std::vector<bool>& nodeFaultValid, LogicGate& gate)
{
    if (gate.inputs.size() == 2)
    {
        if (nodeFaultValid[gate.inputs[0]] && nodeFaultValid[gate.inputs[1]])
            return true;
    }

    if (gate.inputs.size() == 1)
    {
        
        if (nodeFaultValid[gate.inputs[0]] )
            return true;
    }
    return false;
}


std::vector<int> intersection_new
    (std::vector<int> vec1, std::vector<int> vec2) 

{   
    std::vector<int> result;
    std::set_intersection(vec1.begin(), vec1.end(),
                          vec2.begin(), vec2.end(),
                          std::back_inserter(result));

    return result;
}

std::vector<int> difference_new
    (std::vector<int>& vec1, std::vector<int>& vec2) 

{   
    std::vector<int> result;
    std::set_difference (vec1.begin(), vec1.end(),
                          vec2.begin(), vec2.end(),
                          std::back_inserter(result));

    return result;
}

std::vector<int> union_sets_new
    (std::vector<int>& vec1, std::vector<int>& vec2) 

{   
    std::vector<int> result;
    std::set_union (vec1.begin(), vec1.end(),
                          vec2.begin(), vec2.end(),
                          std::back_inserter(result));

    return result;
}

void simulateAND_F(
    //std::vector<std::vector<std::vector<int>>> nodeFaults,
    std::vector<std::vector<int>>& nodeFaults_new,
    const LogicGate& gate,const std::vector<int>& nodeValues)
{
    std::vector<int> result;
    //Divide into four categories
    if (nodeValues[gate.inputs[0]] == 0 && nodeValues[gate.inputs[1]]==0)
    {
        result= intersection_new(nodeFaults_new[gate.inputs[0]],nodeFaults_new[gate.inputs[1]]);
        nodeFaults_new[gate.output] = union_sets_new(result,nodeFaults_new[gate.output]);
    }
    else if (nodeValues[gate.inputs[0]] == 1 && nodeValues[gate.inputs[1]]==0)
    {
        result=     difference_new(nodeFaults_new[gate.inputs[1]],nodeFaults_new[gate.inputs[0]]);
        nodeFaults_new[gate.output] = union_sets_new(result,nodeFaults_new[gate.output]);
    }
    else if(nodeValues[gate.inputs[0]] == 0 && nodeValues[gate.inputs[1]]==1)
    {
        result=    difference_new(nodeFaults_new[gate.inputs[0]],nodeFaults_new[gate.inputs[1]]);
        nodeFaults_new[gate.output] = union_sets_new(result,nodeFaults_new[gate.output]);
    }
    else
    {
        result=        union_sets_new(nodeFaults_new[gate.inputs[1]],nodeFaults_new[gate.inputs[0]]);
        nodeFaults_new[gate.output] = union_sets_new(result,nodeFaults_new[gate.output]);
    }
}

void simulateNAND_F(
    //std::vector<std::vector<std::vector<int>>> nodeFaults,
    std::vector<std::vector<int>>& nodeFaults_new,
    const LogicGate& gate,const std::vector<int>& nodeValues)
{
    std::vector<int> result;
    //Divide into four categories
    if (nodeValues[gate.inputs[0]] == 0 && nodeValues[gate.inputs[1]]==0)
    {
        result= intersection_new(nodeFaults_new[gate.inputs[0]],nodeFaults_new[gate.inputs[1]]);
        nodeFaults_new[gate.output] = union_sets_new(result,nodeFaults_new[gate.output]);
    }
    else if (nodeValues[gate.inputs[0]] == 1 && nodeValues[gate.inputs[1]]==0)
    {
        result=     difference_new(nodeFaults_new[gate.inputs[1]],nodeFaults_new[gate.inputs[0]]);
        nodeFaults_new[gate.output] = union_sets_new(result,nodeFaults_new[gate.output]);
    }
    else if(nodeValues[gate.inputs[0]] == 0 && nodeValues[gate.inputs[1]]==1)
    {
        result=    difference_new(nodeFaults_new[gate.inputs[0]],nodeFaults_new[gate.inputs[1]]);
        nodeFaults_new[gate.output] = union_sets_new(result,nodeFaults_new[gate.output]);
    }
    else
    {
        result=        union_sets_new(nodeFaults_new[gate.inputs[1]],nodeFaults_new[gate.inputs[0]]);
        nodeFaults_new[gate.output] = union_sets_new(result,nodeFaults_new[gate.output]);
    }
}

void simulateOR_F(
    //std::vector<std::vector<std::vector<int>>> nodeFaults,
    std::vector<std::vector<int>>& nodeFaults_new,
    const LogicGate& gate,const std::vector<int>& nodeValues)
{
    std::vector<int> result;
    //Divide into four categories
    if (nodeValues[gate.inputs[0]] == 0 && nodeValues[gate.inputs[1]]==0)
    {
        result= union_sets_new(nodeFaults_new[gate.inputs[0]],nodeFaults_new[gate.inputs[1]]);
        nodeFaults_new[gate.output] = union_sets_new(result,nodeFaults_new[gate.output]);
    }
    else if (nodeValues[gate.inputs[0]] == 1 && nodeValues[gate.inputs[1]]==0)
    {
        result=     difference_new(nodeFaults_new[gate.inputs[0]],nodeFaults_new[gate.inputs[1]]);
        nodeFaults_new[gate.output] = union_sets_new(result,nodeFaults_new[gate.output]);
    }
    else if(nodeValues[gate.inputs[0]] == 0 && nodeValues[gate.inputs[1]]==1)
    {

        result=    difference_new(nodeFaults_new[gate.inputs[1]],nodeFaults_new[gate.inputs[0]]);
        nodeFaults_new[gate.output] = union_sets_new(result,nodeFaults_new[gate.output]);
    }
    else
    {
        result=    intersection_new(nodeFaults_new[gate.inputs[1]],nodeFaults_new[gate.inputs[0]]);
        nodeFaults_new[gate.output] = union_sets_new(result,nodeFaults_new[gate.output]);
    }
}

void simulateNOR_F(
    //std::vector<std::vector<std::vector<int>>> nodeFaults,
    std::vector<std::vector<int>>& nodeFaults_new,
    const LogicGate& gate,const std::vector<int>& nodeValues)
{
    std::vector<int> result;
    //Divide into four categories
    if (nodeValues[gate.inputs[0]] == 0 && nodeValues[gate.inputs[1]]==0)
    {
        result= union_sets_new(nodeFaults_new[gate.inputs[0]],nodeFaults_new[gate.inputs[1]]);
        nodeFaults_new[gate.output] = union_sets_new(result,nodeFaults_new[gate.output]);
    }
    else if (nodeValues[gate.inputs[0]] == 1 && nodeValues[gate.inputs[1]]==0)
    {
        result=     difference_new(nodeFaults_new[gate.inputs[0]],nodeFaults_new[gate.inputs[1]]);
        nodeFaults_new[gate.output] = union_sets_new(result,nodeFaults_new[gate.output]);
    }
    else if(nodeValues[gate.inputs[0]] == 0 && nodeValues[gate.inputs[1]]==1)
    {

        result=    difference_new(nodeFaults_new[gate.inputs[1]],nodeFaults_new[gate.inputs[0]]);
        nodeFaults_new[gate.output] = union_sets_new(result,nodeFaults_new[gate.output]);
    }
    else
    {
        result=    intersection_new(nodeFaults_new[gate.inputs[1]],nodeFaults_new[gate.inputs[0]]);
        nodeFaults_new[gate.output] = union_sets_new(result,nodeFaults_new[gate.output]);
    }
}

void simulateINV_F(
    //std::vector<std::vector<std::vector<int>>> nodeFaults,
    std::vector<std::vector<int>>& nodeFaults_new,
    const LogicGate& gate,const std::vector<int>& nodeValues
    )
{
    nodeFaults_new[gate.output] = union_sets_new(nodeFaults_new[gate.inputs[0]],nodeFaults_new[gate.output]);
}  

void simulateBUF_F(
    //std::vector<std::vector<std::vector<int>>> nodeFaults,
    std::vector<std::vector<int>>& nodeFaults_new,
    const LogicGate& gate,const std::vector<int>& nodeValues
    )
{
    nodeFaults_new[gate.output] = union_sets_new(nodeFaults_new[gate.inputs[0]],nodeFaults_new[gate.output]);
}   

void fault_sim_A(CircuitGraph& circuit, std::vector<bool>& nodeFaultValid, 
    //std::vector<std::vector<std::vector<int>>> nodeFaults,
    std::vector<std::vector<int>>& nodeFaults_new,
    std::vector<int>& nodeFaults_all,
    std::vector<int>& outputGates, std::vector<int> &nodeValues,
    std::vector<int>& inputPattern, std::vector<int> &inputGates,
    int maxnode)
{
    //Initial End Signal
    bool allFinish =false;

    //Fault is here 
    for (int i = 1; i <= maxnode; ++i)
    {
        if (nodeValues[i]==1)
        {
            std::string combinedString = std::to_string(i) + std::to_string(0);
            int combinedNumber = std::stoi(combinedString);
            nodeFaults_new[i].push_back(combinedNumber);
        }
        else
        {
            std::string combinedString = std::to_string(i) + std::to_string(1);
            int combinedNumber = std::stoi(combinedString);
            nodeFaults_new[i].push_back(combinedNumber);
        }

        std::string combinedString0 = std::to_string(i) + std::to_string(0);
        std::string combinedString1 = std::to_string(i) + std::to_string(1);
        nodeFaults_all.push_back(std::stoi(combinedString0));
        nodeFaults_all.push_back(std::stoi(combinedString1));          
    }

    //Input Valid Signal
    //std::cout << "InputPattern Size: " << inputPattern.size() << std::endl;
    
    for (long unsigned int i = 0; i < inputPattern.size(); ++i)
    {
        //Pick the gate
        int inputNodeIndex = inputGates[i];
        //Set the faults
        nodeFaultValid[inputNodeIndex] = true;
    }
        
    //Initialize Signals
    for (LogicGate& gate : circuit.gates)
    {
        gate.fault_Finish = false;
        gate.fault_Ready = false;
    }

    //Start Simulation 
    while (!allFinish)
    {
        //Initially False !!!!!!
        allFinish = true;
        for (LogicGate& gate : circuit.gates)
        {
            //std::cout<<"Gate Output: "<< gate.output<<std::endl;
            //No Finish
            if (gate.fault_Finish==false)
            {
                switch (gate.type)
                {
                case AND:
                    if (isFaultReady(nodeFaultValid,gate))
                    {
                        nodeFaultValid[gate.output]=true;
                        //nodeFaults[gate.output] = 
                        simulateAND_F(nodeFaults_new, gate, nodeValues);
                        gate.fault_Finish = true;
                        //std::cout<<"AND Finish:"<<gate.type<<std::endl;
                    }
                    break;
                case NAND:
                    if (isFaultReady(nodeFaultValid,gate))
                    {
                        nodeFaultValid[gate.output]=true;
                        //nodeFaults[gate.output] = 
                        simulateNAND_F(nodeFaults_new, gate, nodeValues);
                        gate.fault_Finish = true;
                        //std::cout<<"NAND Finish:"<<gate.type<<std::endl;
                    }
                    break;                
                case OR:
                    if (isFaultReady(nodeFaultValid,gate))
                    {
                        nodeFaultValid[gate.output]=true;
                        //nodeFaults[gate.output] = 
                        simulateOR_F(nodeFaults_new, gate, nodeValues);
                        gate.fault_Finish = true;
                        //std::cout<<"OR Finish:"<<gate.type<<std::endl;
                    }
                    break; 
                case NOR:
                    if (isFaultReady(nodeFaultValid,gate))
                    {
                        nodeFaultValid[gate.output]=true;
                        //nodeFaults[gate.output] = 
                        simulateNOR_F(nodeFaults_new, gate, nodeValues);
                        gate.fault_Finish = true;
                        //std::cout<<"NOR Finish:"<<gate.type<<std::endl;
                    }
                    break; 
                case INV:
                    if (isFaultReady(nodeFaultValid,gate))
                    {
                        nodeFaultValid[gate.output]=true;
                        //nodeFaults[gate.output] = 
                        simulateINV_F(nodeFaults_new, gate, nodeValues);
                        gate.fault_Finish = true;
                        //std::cout<<"INV Finish:"<<std::endl;
                    }
                    break;
                case BUF:
                    if (isFaultReady(nodeFaultValid,gate))
                    {
                        
                        nodeFaultValid[gate.output]=true;
                        //nodeFaults[gate.output] = 
                        //std::cout<<"test0"<<std::endl;
                        simulateBUF_F(nodeFaults_new, gate, nodeValues);
                        //std::cout<<"test1"<<std::endl;
                        gate.fault_Finish = true;
                        //std::cout<<"BUFFinish:"<<gate.type<<std::endl;
                    }
                    break;                    
                default:
                    std::cout<<"Wrong Gate type"<<std::endl;
                    break;
                }
            }
        }

        for (LogicGate& gate : circuit.gates)
        {
            if (gate.fault_Finish == false)
            {
                allFinish = false;
                break;
            }
        }        
    }
}

void fault_sim_B(CircuitGraph& circuit, std::vector<bool>& nodeFaultValid, 
    std::vector<std::vector<int>>& nodeFaults_new,
    std::vector<int>& nodeFaults_all,
    std::vector<int>& outputGates, std::vector<int> &nodeValues,
    std::vector<int>& inputPattern, std::vector<int> &inputGates,
    std::string faultfilename)
{
    //Initial End Signal
    bool allFinish =false;

    // Inject the necessary faults
    read_B_fault(faultfilename, nodeFaults_new, nodeFaults_all, nodeValues);
    
    //Input Valid Signal
    //std::cout << "InputPattern Size: " << inputPattern.size() << std::endl;
    for (long unsigned int i = 0; i < inputPattern.size(); ++i)
    {
        //Pick the gate
        int inputNodeIndex = inputGates[i];
        //Set the faults
        nodeFaultValid[inputNodeIndex] = true;
    }
        
    //Initialize Signals
    for (LogicGate& gate : circuit.gates)
    {
        gate.fault_Finish = false;
        gate.fault_Ready = false;
    }

    //Start Simulation 
    while (!allFinish)
    {
        //Initially False !!!!!!
        allFinish = true;
        for (LogicGate& gate : circuit.gates)
        {
            //std::cout<<"Gate Output: "<< gate.output<<std::endl;
            //No Finish
            if (gate.fault_Finish==false)
            {
                switch (gate.type)
                {
                case AND:
                    if (isFaultReady(nodeFaultValid,gate))
                    {
                        nodeFaultValid[gate.output]=true;
                        //nodeFaults[gate.output] = 
                        simulateAND_F(nodeFaults_new, gate, nodeValues);
                        gate.fault_Finish = true;
                        //std::cout<<"AND Finish:"<<gate.type<<std::endl;
                    }
                    break;
                case NAND:
                    if (isFaultReady(nodeFaultValid,gate))
                    {
                        nodeFaultValid[gate.output]=true;
                        //nodeFaults[gate.output] = 
                        simulateNAND_F(nodeFaults_new, gate, nodeValues);
                        gate.fault_Finish = true;
                        //std::cout<<"NAND Finish:"<<gate.type<<std::endl;
                    }
                    break;                
                case OR:
                    if (isFaultReady(nodeFaultValid,gate))
                    {
                        nodeFaultValid[gate.output]=true;
                        //nodeFaults[gate.output] = 
                        simulateOR_F(nodeFaults_new, gate, nodeValues);
                        gate.fault_Finish = true;
                        //std::cout<<"OR Finish:"<<gate.type<<std::endl;
                    }
                    break; 
                case NOR:
                    if (isFaultReady(nodeFaultValid,gate))
                    {
                        nodeFaultValid[gate.output]=true;
                        //nodeFaults[gate.output] = 
                        simulateNOR_F(nodeFaults_new, gate, nodeValues);
                        gate.fault_Finish = true;
                        //std::cout<<"NOR Finish:"<<gate.type<<std::endl;
                    }
                    break; 
                case INV:
                    if (isFaultReady(nodeFaultValid,gate))
                    {
                        nodeFaultValid[gate.output]=true;
                        //nodeFaults[gate.output] = 
                        simulateINV_F(nodeFaults_new, gate, nodeValues);
                        gate.fault_Finish = true;
                        //std::cout<<"INV Finish:"<<std::endl;
                    }
                    break;
                case BUF:
                    if (isFaultReady(nodeFaultValid,gate))
                    {
                        
                        nodeFaultValid[gate.output]=true;
                        //nodeFaults[gate.output] = 
                        //std::cout<<"test0"<<std::endl;
                        simulateBUF_F(nodeFaults_new, gate, nodeValues);
                        //std::cout<<"test1"<<std::endl;
                        gate.fault_Finish = true;
                        //std::cout<<"BUFFinish:"<<gate.type<<std::endl;
                    }
                    break;                    
                default:
                    std::cout<<"Wrong Gate type"<<std::endl;
                    break;
                }
            }
        }

        for (LogicGate& gate : circuit.gates)
        {
            if (gate.fault_Finish == false)
            {
                allFinish = false;
                break;
            }
        }        
    }    
}        