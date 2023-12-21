#include "Circuit.h"
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

// Function to simulate an AND gate
int simulateAND(const std::vector<int>& nodeValues, const LogicGate& gate) {
    return nodeValues[gate.inputs[0]] && nodeValues[gate.inputs[1]];
}

// Function to simulate an OR gate
int simulateOR(const std::vector<int>& nodeValues, const LogicGate& gate) {
    return nodeValues[gate.inputs[0]] || nodeValues[gate.inputs[1]];
}

// Function to simulate a NOR gate
int simulateNOR(const std::vector<int>& nodeValues, const LogicGate& gate) {
    return !(nodeValues[gate.inputs[0]] || nodeValues[gate.inputs[1]]);
}

// Function to simulate a NAND gate
int simulateNAND(const std::vector<int>& nodeValues, const LogicGate& gate) {
    return !(nodeValues[gate.inputs[0]] && nodeValues[gate.inputs[1]]);
}

// Function to simulate a NOT gate
int simulateINV(const std::vector<int>& nodeValues, const LogicGate& gate) {
    return !nodeValues[gate.inputs[0]];
}

// Function to simulate a BUF gate
int simulateBUF(const std::vector<int>& nodeValues, const LogicGate& gate) {
    return nodeValues[gate.inputs[0]];
}

// Function to check if a gate is ready to be simulated
bool isReady(std::vector<bool> nodeValid, LogicGate& gate) 
{
    for (int input : gate.inputs) {
        if (!nodeValid[input]) {
            return false;
        }
    }
    return true;
}

CircuitGraph parseCircuitFile(const std::string& filePath, std::vector<int>& inputGates,
    std::vector<int>& outputGates,std::vector<int> nodeValues, std::vector<bool> nodeValid)
{
    CircuitGraph circuit;
    std::string line;
    
    // Read the file
    std::ifstream circuitFile(filePath);
    
    if (!circuitFile.is_open()) 
    {
        std::cerr << "Failed to open circuit description file." << std::endl;
    }
    else
    {
        //std::cerr << "Sucessfully open circuit description file." << std::endl;
    }

    while (std::getline(circuitFile, line))
    {
        std::istringstream iss(line);
        std::string gateTypeStr;
        std::string token;

        int takeNode;
        int outputNode;
        int inputNode1, inputNode2;
        iss >> token;
        //Input Gates and Output Gates
        if (token == "INPUT")
        {
            //  Input Node Info
            while (iss >> takeNode && takeNode != -1)
            {
                nodeValid[takeNode] = true;
                // inputGate Add
                inputGates.push_back(takeNode);
            }
        }
        else if (token == "OUTPUT")
        {
            // Output Node Info
            while (iss >> takeNode && takeNode != -1)
            {
                //printf("takenodes:%d\n",takeNode);
                outputGates.push_back(takeNode);
                //printf("size:%d\n",sizeof(takeNode));
                //printf("size:%d\n",sizeof(outputGates));
            }
        }

        // Reset iss to the initial position
        iss.clear();
        iss.seekg(0); 

        // Parse the line to extract gate information and connections
        if (iss >> gateTypeStr) 
        {
            LogicGate gate;
            // Convert gate type string to enum
            if (gateTypeStr == "INV") {
                gate.type = INV;
                gate.inversion=1;
            }
            else if (gateTypeStr == "BUF") {
                gate.type = BUF;
                gate.inversion=0;
            }
            else if (gateTypeStr == "AND") {
                gate.type = AND;
                gate.inversion=0;
            }
            else if (gateTypeStr == "OR") {
                gate.type = OR;
                gate.inversion=0;
            }
            else if (gateTypeStr == "NOR") {
                gate.type = NOR;
                gate.inversion=1;
            }
            else if (gateTypeStr == "NAND") {
                gate.type = NAND;
                gate.inversion=1;
            }

            if (gateTypeStr != "OUTPUT" && gateTypeStr != "INPUT")
            {
                // Set gate inputs and output
                iss >> inputNode1;
                gate.inputs.push_back(inputNode1);
                if (gate.type != INV && gate.type != BUF) {
                    iss >> inputNode2 >> outputNode;
                    gate.inputs.push_back(inputNode2);
                    gate.output = outputNode;
                }
                else
                {
                    iss >> outputNode;
                    gate.output = outputNode;
                }
            
            // Add the gate to the list
            circuit.gates.push_back(gate);
            }
        }
    }
    circuitFile.close();

    return circuit;
}

void simulateCircuit( CircuitGraph& circuit, std::vector<int>& inputPattern, 
    std::vector<int> &inputGates,std::vector<int> &nodeValues, 
    std::vector<bool>& nodeValid)
{
    // End Signal False
    bool allFinish=false;

    // Initialize the input node
    // std::cout << inputPattern.size() << std::endl;
    if(inputPattern.size())
    {
        for (long unsigned int i = 0; i < inputPattern.size(); ++i)
        {
            //Pick the gate
            int inputNodeIndex = inputGates[i];        
            //Set the value and valid signal
            nodeValues[inputNodeIndex] = inputPattern[i];
            nodeValid[inputNodeIndex] = true;
        }
    }
    
    // Initial Signal Gate End Signal
    for (LogicGate& gate : circuit.gates)
    {
        gate.finish = false;
    }

    while (!allFinish)
    {
        allFinish = true;
        for ( LogicGate& gate : circuit.gates)
        {
            if (gate.finish == false)
            {
                //std::cout << "gate type: " << gate.type << std:: endl;
                switch (gate.type) {
                case AND:
                    if (isReady(nodeValid, gate))
                    {
                        nodeValid[gate.output] = 1;
                        nodeValues[gate.output] = simulateAND(nodeValues, gate);
                        gate.finish = true;
                        //std::cout << "Finish AND "  << std::endl;
                        if (gate.output == 9) std::cout << "Wrong " << std::endl;
                    }
                    break;
                case OR:
                    if (isReady(nodeValid, gate))
                    {
                        nodeValid[gate.output] = 1;
                        nodeValues[gate.output] = simulateOR(nodeValues, gate);
                        gate.finish = true;
                        //std::cout << "Finish OR " << std::endl;
                        if (gate.output == 9) std::cout << "Wrong " << std::endl;
                    }
                    break;
                case NOR:
                    if (isReady(nodeValid, gate))
                    {
                        nodeValid[gate.output] = 1;
                        nodeValues[gate.output] = simulateNOR(nodeValues, gate);
                        gate.finish = true;
                        //std::cout << "Finish NOR " << std::endl;
                    }
                    break;
                case NAND:
                    if (isReady(nodeValid, gate))
                    {
                        nodeValid[gate.output] = 1;
                        nodeValues[gate.output] = simulateNAND(nodeValues, gate);
                        gate.finish = true;
                        //std::cout << "Finish NAND " << std::endl;
                    }
                    break;
                case INV:
                    if (isReady(nodeValid, gate))
                    {
                        nodeValid[gate.output] = 1;
                        nodeValues[gate.output] = simulateINV(nodeValues, gate);
                        gate.finish = true;
                        //std::cout << "Finish INV " << std::endl;
                        if (gate.output == 9) std::cout << "Wrong INV " << gate.inputs[0] << std::endl;
                    }
                    break;
                case BUF:
                    if (isReady(nodeValid, gate))
                    {
                        nodeValid[gate.output] = 1;
                        nodeValues[gate.output] = simulateBUF(nodeValues, gate);
                        gate.finish = true;
                        //std::cout << "Finish BUF " << std::endl;
                    }
                    break;
                }
            }
        }
        
        // check whether all finish or not
        for (LogicGate& gate : circuit.gates)
        {
            if (gate.finish == false)
            {
                allFinish = false;
                break;
            }
        }
    }
}