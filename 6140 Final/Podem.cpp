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

std::vector<int> inverseMatrix2 = 
{
    1,
    0,
    D_bar,
    D,
    xx
};

std::vector<int> transMatrix =
{
  D,
  D_bar  
};

bool podem(CircuitGraph& circuit,std::vector<int> outputGates,std::vector<int>& nodeValues, std::vector<bool>& nodeValid,
            std::vector<int>& D_F,int nodefault, int stuck_value, std::vector<int> inputGates)
{

    // If error at OUTPUT 
    // Return true
    bool end = output_check(outputGates, nodeValues);
    if (end == true)
    {
        //std::cout << "Success!" <<std::endl;
        return true;
    }

    //std::cout<<"2 node value 72: " << nodeValues[72]  << std::endl;
    //std::cout<<"2 node value 179: " << nodeValues[179]  << std::endl;

    // If Check possible or not
    // Return false
    bool possible = possible_check(circuit,D_F, nodeValues, nodefault, stuck_value,nodeValid);
    if (possible == false)
    {  
        std::cout << "Impossible. Thank you for test." <<std::endl;
        return false;
    }

    //std::cout<<"3 node value 4 and 20: " << nodeValues[4] << nodeValues[20] << std::endl;
    //std::cout<<"3 node value 72: " << nodeValues[72]  << std::endl;
    //std::cout<<"3 node value 179: " << nodeValues[179]  << std::endl;
    // Objective Func
    // Find the Target
    /* the result of Objective*/
    int target = Objective(circuit,nodeValues, nodefault, stuck_value, nodeValid);
    
    //std::cout<<"target: " << target << std::endl;
    
    
    //std::cout<<"4 node value 4 and 20: " << nodeValues[4] << nodeValues[20] << std::endl;   
    //std::cout<<"4 node value 72: " << nodeValues[72]  << std::endl;
    //std::cout<<"4 node value 179: " << nodeValues[179]  << std::endl;
    // Backtrace Func
    // Directly Set the Input value
    // Output node should be input node
    /*The result of Backtrce*/
    int input_set = Backtrace(circuit,nodeValues, nodefault, stuck_value, target, inputGates);
    
    //std::cout<<"5 node value 4 and 20: " << nodeValues[4] << nodeValues[20] << std::endl;
    //std::cout<<"5 node value 72: " << nodeValues[72]  << std::endl;
    //std::cout<<"5 node value 179: " << nodeValues[179]  << std::endl;
    //std::cout<<"input_set: " << input_set << std::endl;

    // Imply Func
    // Use the Backtrace Result 
    Imply(circuit,nodeValues,input_set, nodeValid, nodefault, stuck_value);

    //std::cout<<"6 node value 14: " << nodeValues[14] << std::endl;
    //std::cout<<"6 node value 16: " << nodeValues[16] << std::endl;
    //std::cout<<"6 node value 17: " << nodeValues[17] << std::endl;
    //std::cout<<"6 node value 72: " << nodeValues[72]  << std::endl;
    //std::cout<<"6 node value 179: " << nodeValues[179]  << std::endl;

    // Check podem 
    // if true then true
    // back up
    CircuitGraph circuit_new = circuit;
    std::vector<int> nodeValues_new = nodeValues;
    std::vector<bool> nodeValid_new = nodeValid;
    std::vector<int> D_F_new = D_F;

    if (podem(circuit, outputGates, nodeValues, nodeValid, D_F, nodefault, stuck_value,inputGates))
    {
        return true;
    }
    // if false then another value 
    // recover
    else
    {
        circuit = circuit_new;
        nodeValues = nodeValues_new;
        nodeValid = nodeValid_new;
        D_F = D_F_new;
    }

    //std::cout<<"7 node value 2 and 10: " << nodeValues[2] << nodeValues[10] << std::endl;

    // if false then another value
    int label = input_set / 10;
    int stuck_value_2 = input_set %10;
    stuck_value_2 = 1-stuck_value_2;
    input_set = concatenateIntegers(label, stuck_value_2);

    // Imply again
    Imply(circuit,nodeValues,input_set, nodeValid, nodefault, stuck_value);

    circuit_new = circuit;
    nodeValues_new = nodeValues;
    nodeValid_new = nodeValid;
    D_F_new = D_F;
    
    if (podem(circuit, outputGates, nodeValues, nodeValid, D_F, nodefault, stuck_value,inputGates))
    {
        return true;
    }
    else
    {
        circuit = circuit_new;
        nodeValues = nodeValues_new;
        nodeValid = nodeValid_new;
        D_F = D_F_new;
        input_set = concatenateIntegers(label, xx);
    }

    Imply(circuit,nodeValues,input_set, nodeValid, nodefault, stuck_value);
    
    return false;

}

int Objective(CircuitGraph& circuit,std::vector<int>& nodeValues, int nodefault, int stuck_value, std::vector<bool>& nodeValid)
{
    // Initialize the return value
    int result = -1;
    int node; 
    int control_value;
    int non_control_value;

    // Need Activate the target fault 
    // if xx in the line
    // return the node and inverse value
    if (nodeValues[nodefault]==xx)
    {   
        int value = 1 - stuck_value;
        result = concatenateIntegers(nodefault,value);
        return result;
    }

    // If already activate
    if (nodeValues[nodefault]==D || nodeValues[nodefault]==D_bar)
    {
        // Find a gate in D-Frontier
        std::vector<int> D_F = D_F_Search(circuit, nodeValues);

        // std::cout<<"    D_F[0]: " << D_F[0] << std::endl;
        // std::cout<<"    circuit.gates[D_F[0]].output: " << circuit.gates[D_F[0]].output << std::endl;
        // std::cout<<"    circuit.gates[D_F[0]].inputs[0]: " << circuit.gates[D_F[0]].inputs[0] << std::endl;
        // std::cout<<"    circuit.gates[D_F[0]].inputs[1]: " << circuit.gates[D_F[0]].inputs[1] << std::endl;
        // std::cout<<"    nodeValues[circuit.gates[D_F[0]].output] " << nodeValues[circuit.gates[D_F[0]].output] << std::endl;
        // std::cout<<"    nodeValues[circuit.gates[D_F[0]].inputs[0]] " << nodeValues[circuit.gates[D_F[0]].inputs[0]] << std::endl;
        // std::cout<<"    nodeValues[circuit.gates[D_F[0]].inputs[1]] " << nodeValues[circuit.gates[D_F[0]].inputs[1]] << std::endl;
        // std::cout<<"    nodeValid[circuit.gates[D_F[0]].output] " << nodeValid[circuit.gates[D_F[0]].output] << std::endl;
        // std::cout<<"    nodeValid[circuit.gates[D_F[0]].inputs[0]] " << nodeValid[circuit.gates[D_F[0]].inputs[0]] << std::endl;
        // std::cout<<"    nodeValid[circuit.gates[D_F[0]].inputs[1]] " << nodeValid[circuit.gates[D_F[0]].inputs[1]] << std::endl;
        
        
        // For D_F[0], which is always there
        // Select one of the input, which is x
        // Input 1 is D or D_bar
        if (nodeValues[circuit.gates[D_F[0]].inputs[0]] ==D || nodeValues[circuit.gates[D_F[0]].inputs[0]] == D_bar)
        {
            node = circuit.gates[D_F[0]].inputs[1];
            if (circuit.gates[D_F[0]].type == AND || circuit.gates[D_F[0]].type == NAND)
            {
                control_value = 0; 
            }
            else if (circuit.gates[D_F[0]].type == OR || circuit.gates[D_F[0]].type == NOR)
            {
                control_value = 1; 
            }
            else
            {
                std::cout<<"D Frontier Wrong: Wrong Gate Type + Not two inputs"<<std::endl;
            }
        }
        // Input 2 is D or D_bar
        else if (nodeValues[circuit.gates[D_F[0]].inputs[1]] == D || nodeValues[circuit.gates[D_F[0]].inputs[1]] == D_bar)
        {
            node = circuit.gates[D_F[0]].inputs[0];
            if (circuit.gates[D_F[0]].type == AND || circuit.gates[D_F[0]].type == NAND)
            {
                control_value = 0; 
            }
            else if (circuit.gates[D_F[0]].type == OR || circuit.gates[D_F[0]].type == NOR)
            {
                control_value = 1; 
            }
            else
            {
                std::cout<<"D Frontier Wrong: Wrong Gate Type + Not two inputs"<<std::endl;
            }
        }
        else
        {
            std::cout<<"D Frontier Wrong: Input does not have D or D bar"<<std::endl;
        }

        // return the node and not controllong value 
        non_control_value = 1 - control_value;
        result = concatenateIntegers(node, non_control_value);
        return result;
    }
    
    if (result == -1)
    {
        std::cout<<"D Frontier Wrong: No D Frontier"<<std::endl;
    }

    return result;
}

int Backtrace(CircuitGraph& circuit,std::vector<int>& nodeValues, int nodefault, int stuck_value, int target, std::vector<int> inputGates)
{   
    int node, value_K, value;
    // Find the node and value
    node = target / 10;    
    value_K = target % 10;
    value = value_K;
    // Initialize
    bool is_gateout = 0;
    int gate_index = -1; 
    bool inversion;
    int node_new;
    bool Finish = 0;
    int return_value;

    // Check input node is gate output of not
    for (int index = 0; index < circuit.gates.size(); ++index)
    {
        LogicGate& gate = circuit.gates[index];
        if (gate.output == node)
        {
            is_gateout = 1;
            gate_index = index;
            break;
        }
    }

    // Still a gate output 
    while (is_gateout == 1)
    {

        // if find the gate 
        if (is_gateout == 1)
        {   
            // Check gate check is correct or not
            if (gate_index < 0)
            {
                std::cout<<"Backtrace Error: Gate Index is negative, not exist"<<std::endl;
            }
            // IF gate is natural num
            else
            {
                // check the inversity
                inversion = circuit.gates[gate_index].inversion;

                //Select one of the inputs with x 
                if (nodeValues[circuit.gates[gate_index].inputs[0]]==xx)
                {
                    value = value ^ inversion;
                    node = circuit.gates[gate_index].inputs[0];                
                }
                else if (nodeValues[circuit.gates[gate_index].inputs[1]]==xx)
                {
                    value = value ^ inversion;
                    node = circuit.gates[gate_index].inputs[1];    
                }
                else
                {
                    std::cout<<"Gate Index:"<< gate_index<<std::endl;
                    std::cout<<"circuit.gates[gate_index].output :"<< circuit.gates[gate_index].output<<std::endl;
                    std::cout<<"circuit.gates[gate_index].inputs[1]:"<< circuit.gates[gate_index].inputs[1] <<std::endl;
                    std::cout<<"circuit.gates[gate_index].inputs[0]:"<< circuit.gates[gate_index].inputs[0] <<std::endl; 
                    std::cout<<"nodeValues[circuit.gates[gate_index].output] :"<< nodeValues[circuit.gates[gate_index].output]<<std::endl;                   
                    std::cout<<"nodeValues[circuit.gates[gate_index].inputs[1]]:"<< nodeValues[circuit.gates[gate_index].inputs[1]] <<std::endl;
                    std::cout<<"nodeValues[circuit.gates[gate_index].inputs[0]]:"<< nodeValues[circuit.gates[gate_index].inputs[0]] <<std::endl;
                    std::cout<<"Backtrace Error: Gate inputs are not xx + D-F problem."<<std::endl;
                }
            }    
        }

        //Rest the output signal
        is_gateout = 0; 
        
        // Figure out it is a gate output or not
        for (int index = 0; index < circuit.gates.size(); ++index)
        {
            LogicGate& gate = circuit.gates[index];
            if (gate.output == node)
            {
                is_gateout = 1;
                gate_index = index;
                break;
            }
        }        
    }

    if (is_gateout == 0)
    {
        // Check if it is a Primary Input 
        for (int k = 0; k < inputGates.size(); ++k) 
        {
            //std::cout << inputGates[k] << std::endl;
            if (inputGates[k]==node)
            {
                Finish = 1;
            }
        }

        if (Finish ==0)
        {
            std::cout<<"inputGates[1]:" << inputGates[1] <<std::endl;
            std::cout<<"Node:" << node <<std::endl;
            std::cout<<"Backtrace Error: Not a gate output also not a input."<<std::endl;
        }
        
    }
    else
    {
        std::cout<<"Backtrace Error: Still gate output."<<std::endl;
    }
    
    if (Finish)
    {
        return_value = concatenateIntegers(node, value);
    }
    else
    {
        std::cout<<"Backtrace Error: Not Finish."<<std::endl;
    }

    return return_value;
}

void Imply(CircuitGraph& circuit, std::vector<int>& nodeValues, int input_set,std::vector<bool>& nodeValid, int nodefault, int stuck_value)
{   
    //std::cout<<"        Imply 0 node value 27: " << nodeValues[27] << std::endl;
    /* Whether the circuit is change during the loop*/
    bool is_change = 0;
    int node = input_set / 10;
    int value = input_set % 10;
    bool fault_here;
    //std::cout<<"    Imply 1 node: " << node  << std::endl;

    // Initialize the input one
    //If fault at input
    if (node == nodefault)
    {   
        //std::cout<<"Node Fault Position Setting"<<std::endl;
        // std::cout<<"    Imply 1 node value 72: " << nodeValues[72]  << std::endl;
        // std::cout<<"    Imply 1 node value 179: " << nodeValues[179]  << std::endl;       
        // std::cout<<"    Imply 1 stuck_value: " << stuck_value  << std::endl;       

        if (stuck_value == (1-value))
        {
            if (stuck_value==1)
            {
                //std::cout<<"Input Valid Test D bar"<<std::endl;
                //std::cout<<"node: "<<node<<std::endl;
                //std::cout<<"nodeValid[node]: "<<nodeValid[node]<<std::endl;
                nodeValues[node] = D_bar;
                nodeValid[node] = true;
                //std::cout<<"nodeValid[node]: "<<nodeValid[node]<<std::endl;
            }
            else if (stuck_value==0)
            {
                //std::cout<<"Input Valid Test D"<<std::endl;
                nodeValues[node] = D;
                nodeValid[node] = true;                
            }
            else
            {
                std::cout<<"Imply Wrong: Wrong Primary Value Set not 1 or 0"<<std::endl;
            }
        }
        else
        {
            std::cout<<"Imply Wrong: Initialize Fault at input"<<std::endl;
        }
    }
    // For normal cases
    else
    {
        if (value == 1 || value == 0 )
        {
            nodeValid[node] = true;
            nodeValues[node] = value;
        }
        else
        {
            std::cout<<"Imply Wrong: In normal case, Wrong Primary Value Set not 1 or 0"<<std::endl;
        }
    }

    //std::cout<<"         Imply 1 node value 27: " << nodeValues[27] << std::endl;

    // Begin Simulate
    do
    {
        // Initialize to 0 
        is_change = 0;

        // Begin Loop
        for ( LogicGate& gate : circuit.gates)
        {
            fault_here = 0;
            if (gate.output == nodefault)
            {
                fault_here = 1;
            }

            if (gate.finish == false)
            {
                switch (gate.type)
                {
                case AND:
                    if (is_Ready_Podem(nodeValues, nodeValid, gate))
                    {
                        nodeValid[gate.output] = 1;
                        nodeValues[gate.output] = Podem_sim_AND(nodeValues,nodeValid, gate, fault_here);
                        gate.finish = true;
                        is_change = true;
                    }
                    break;
                case NAND:                   
                    if (is_Ready_Podem(nodeValues, nodeValid, gate))
                    {
                        // if (gate.output == 53)
                        // {
                        //     std::cout<<"    Get into NAND gate "<<std::endl;
                        //     std::cout << "    nodeValid[gate.inputs[0]]" << nodeValid[gate.inputs[0]] <<std::endl;
                        //     std::cout << "    nodeValid[gate.inputs[0]]" << nodeValid[gate.inputs[1]] <<std::endl;
                        //     std::cout << "    nodeValues[gate.inputs[0]]" << nodeValues[gate.inputs[0]] <<std::endl;
                        //     std::cout << "    nodeValues[gate.inputs[0]]" << nodeValues[gate.inputs[1]] <<std::endl;
                        // }
                        nodeValid[gate.output] = 1;
                        nodeValues[gate.output] = Podem_sim_NAND(nodeValues,nodeValid, gate, fault_here);
                        if (gate.output == 53)
                        {
                            // std::cout<<"    After NAND Sim "<<std::endl;
                            // std::cout << "    nodeValid[gate.output]" << nodeValid[gate.output] <<std::endl;
                            // std::cout << "    nodeValues[gate.output]" << nodeValues[gate.output] <<std::endl;
                        }
                        gate.finish = true;
                        is_change = true;
                    }
                    break;
                case OR:
                    if (is_Ready_Podem(nodeValues, nodeValid, gate))
                    {
                        // if (gate.output == 179)
                        // {
                        //     std::cout<<"    Get into OR gate "<<std::endl;
                        //     std::cout << "    nodeValid[gate.inputs[0]]" << nodeValid[gate.inputs[0]] <<std::endl;
                        //     std::cout << "    nodeValid[gate.inputs[1]]" << nodeValid[gate.inputs[1]] <<std::endl;
                        //     std::cout << "    nodeValues[gate.inputs[0]]" << nodeValues[gate.inputs[0]] <<std::endl;
                        //     std::cout << "    nodeValues[gate.inputs[1]]" << nodeValues[gate.inputs[1]] <<std::endl;
                        //     std::cout << "    nodeValid[gate.output]" << nodeValid[gate.output] <<std::endl;
                        //     std::cout << "    nodeValues[gate.output]" << nodeValues[gate.output] <<std::endl;
                        //     std::cout << "    fault_here" << fault_here <<std::endl;
                            
                        // }
                        nodeValid[gate.output] = 1;

                        // if (gate.output == 179)
                        //     std::cout<<"    Get into OR gate "<<std::endl;
                
                        nodeValues[gate.output] = Podem_sim_OR(nodeValues,nodeValid, gate, fault_here);

                        gate.finish = true;
                        is_change = true;

                        // if (gate.output == 179)
                        // {
                        //     std::cout<<"   After OR gate "<<std::endl;
                        //     std::cout << "    nodeValid[gate.output]" << nodeValid[gate.output] <<std::endl;
                        //     std::cout << "    nodeValues[gate.output]" << nodeValues[gate.output] <<std::endl;
                        // }

                    }
                    break;
                case NOR:
                    if (is_Ready_Podem(nodeValues, nodeValid, gate))
                    {
                        // if (gate.output == 27)
                        // {
                        //     std::cout<<"        Get into NAND gate "<<std::endl;
                        //     std::cout << "      nodeValid[gate.inputs[0]]" << nodeValid[gate.inputs[0]] <<std::endl;
                        //     std::cout << "      nodeValid[gate.inputs[0]]" << nodeValid[gate.inputs[1]] <<std::endl;
                        //     std::cout << "      nodeValues[gate.inputs[0]]" << nodeValues[gate.inputs[0]] <<std::endl;
                        //     std::cout << "      nodeValues[gate.inputs[0]]" << nodeValues[gate.inputs[1]] <<std::endl;
                        // }
                        nodeValid[gate.output] = 1;
                        nodeValues[gate.output] = Podem_sim_NOR(nodeValues,nodeValid, gate, fault_here);
                        gate.finish = true;
                        is_change = true;
                        // if (gate.output == 27)
                        // {
                        //     std::cout<<"        After NAND gate "<<std::endl;
                        //     std::cout << "      nodeValid[gate.inputs[0]]" << nodeValid[gate.inputs[0]] <<std::endl;
                        //     std::cout << "      nodeValid[gate.inputs[0]]" << nodeValid[gate.inputs[1]] <<std::endl;
                        //     std::cout << "      nodeValues[gate.inputs[0]]" << nodeValues[gate.inputs[0]] <<std::endl;
                        //     std::cout << "      nodeValues[gate.inputs[0]]" << nodeValues[gate.inputs[1]] <<std::endl;
                        // }
                    }
                    break;
                case INV:
                    if (is_Ready_Podem(nodeValues, nodeValid, gate))
                    {
                        nodeValid[gate.output] = 1;
                        nodeValues[gate.output] = Podem_sim_INV(nodeValues,nodeValid, gate, fault_here);
                        gate.finish = true;
                        is_change = true;
                    }
                    break;
                case BUF:
                    if (is_Ready_Podem(nodeValues, nodeValid, gate))
                    {
                        nodeValid[gate.output] = 1;
                        nodeValues[gate.output] = Podem_sim_BUF(nodeValues,nodeValid, gate, fault_here);
                        gate.finish = true;
                        is_change = true;
                    }
                    break;
                }
            } 
        }     
    } while (is_change == 1);

//    std::cout<<"         Imply 2 node value 27: " << nodeValues[27] << std::endl;

}

bool output_check(std::vector<int> outputGates,std::vector<int> nodeValues)
{
    // check output node
    // whether have D or D bar
    for (int index : outputGates)
    {
        if (index < nodeValues.size()) 
        {
            //std::cout << "Value at node index " << index << " is " << nodeValues[index] << std::endl;
            if (nodeValues[index] == D || nodeValues[index] == D_bar)
            {
                // std::cout << "Output Node: "<< index << std::endl;
                // std::cout << "Value at node index 27 "  << " is " << nodeValues[27] << std::endl;
                // std::cout << "Value at node index 16 "  << " is " << nodeValues[16] << std::endl;
                // std::cout << "Value at node index 53 "  << " is " << nodeValues[53] << std::endl;
                // std::cout << "Value at node index 179 "  << " is " << nodeValues[179] << std::endl;    
                // std::cout << "Value at node index 183 "  << " is " << nodeValues[183] << std::endl;  
                // std::cout << "Value at node index 3 "  << " is " << nodeValues[3] << std::endl;    
                // std::cout << "Value at node index 72 "  << " is " << nodeValues[72] << std::endl;                    
                return true;
            }
        } 
        else 
        {
            std::cout << "Index " << index << " is out of range for nodeValues." << std::endl;
        }
    }

    // std::cout << "Value at node index 27 "  << " is " << nodeValues[27] << std::endl;
    // std::cout << "Value at node index 16 "  << " is " << nodeValues[16] << std::endl;
    // std::cout << "Value at node index 53 "  << " is " << nodeValues[53] << std::endl;
    

    return false;
}

bool possible_check(CircuitGraph& circuit, std::vector<int>& D_F,std::vector<int>& nodeValues, int nodefault, int stuck_value, std::vector<bool>& nodeValid)
{
    bool result = true;
    int value_cor = 1 - stuck_value;

    D_F = D_F_Search(circuit, nodeValues);
    
    // If it is not activate 
    if (nodeValues[nodefault] == xx)
    {
        return true;
    }
    else
    {
        for (size_t index = 0; index < circuit.gates.size(); ++index)
        {
            LogicGate& gate = circuit.gates[index];

            if (gate.output == nodefault
                && (nodeValues[gate.output]==D || nodeValues[gate.output]==D_bar))
            {
                // check the activate is correct or not
                switch (gate.type)
                {
                case OR:
                    /* code */
                    if (simulateOR(nodeValues,gate)!=value_cor && nodeValid[gate.inputs[0]] && nodeValid[gate.inputs[1]])
                    {
                        return false;
                    }
                    
                    break;
                case NOR:
                    /* code */
                    if (simulateNOR(nodeValues,gate)!=value_cor&& nodeValid[gate.inputs[0]] && nodeValid[gate.inputs[1]])
                    {
                        return false;
                    }
                    break;
                case AND:
                    /* code */
                    if (simulateAND(nodeValues,gate)!=value_cor&& nodeValid[gate.inputs[0]] && nodeValid[gate.inputs[1]])
                    {
                        return false;
                    }
                    break;
                case NAND:
                    /* code */
                    if (simulateNAND(nodeValues,gate)!=value_cor && nodeValid[gate.inputs[0]] && nodeValid[gate.inputs[1]])
                    {
                        return false;
                    }
                    break;
                case INV:
                    /* code */
                    if (simulateINV(nodeValues,gate)!=value_cor&& nodeValid[gate.inputs[0]] )
                    {
                        return false;
                    }
                    break;
                case BUF:
                    if (simulateBUF(nodeValues,gate)!=value_cor&& nodeValid[gate.inputs[0]] )
                    {
                        return false;
                    }
                    /* code */
                    break;                
                default:
                    break;
                }                
            }
        }
    }


    // IF D frontier is empty
    if (D_F.size() == 0)
    {
        std::cout << "No D Frontier so that it is impossible." <<std::endl;
        result =false;
    }

    // If it is not activate 
    if (nodeValues[nodefault]!=D && nodeValues[nodefault]!=D_bar && nodeValues[nodefault]!=xx)
    {
        std::cout << "Not activate so that it is impossible." <<std::endl;
        result =false;
    }

    return result;
}

// output Gate ID for D_F
std::vector<int> D_F_Search(CircuitGraph& circuit, std::vector<int>& nodeValues)
{
    std::vector<int> result;

    for (size_t index = 0; index < circuit.gates.size(); ++index)
    {
        LogicGate& gate = circuit.gates[index];

        // Ensure the gate is not Inv or Buf
        if (gate.type != INV && gate.type != BUF)
        {

            //For test
            if (gate.inputs[0]==18 || gate.inputs[1]==18)
            {
                // std::cout<<"In D_F Search" <<std::endl;
                // std::cout << "gate.inputs[0]" << gate.inputs[0]<<std::endl;
                // std::cout << "gate.inputs[1]" << gate.inputs[1]<<std::endl;
                // std::cout << "gate.output" << gate.output <<std::endl;
                // std::cout << "value gate.inputs[0]" << nodeValues[gate.inputs[0]]<<std::endl;
                // std::cout << "value gate.inputs[1]" << nodeValues[gate.inputs[1]]<<std::endl;
                // std::cout << "value gate.output" << nodeValues[gate.output] <<std::endl;
                // std::cout << "nodeValues[8]" << nodeValues[8] <<std::endl;
                // std::cout << "value gate.output" << nodeValues[gate.output] <<std::endl;
                
            }
        
            //Input D
            if (D_F_Check(nodeValues, gate))
            {
                result.push_back(index);
            }
        }
    }

    return result;
}

// Check the signal two input gate
// Whethter it is a D frontier
bool D_F_Check(std::vector<int>& nodeValues, LogicGate& gate)
{   
    // the output must be X
    if (nodeValues[gate.output] == xx)
    {
        // one of the input is D
        if (nodeValues[gate.inputs[0]]==D || nodeValues[gate.inputs[0]]==D_bar||
            nodeValues[gate.inputs[1]]==D || nodeValues[gate.inputs[1]]==D_bar)
        {           
            return true;
        }     
    }
    return false;
}

int concatenateIntegers(int a, int b) 
{
    std::ostringstream oss;
    oss << a << b;
    std::string combined = oss.str();

    // 将字符串转换为整数
    std::istringstream iss(combined);
    int result;
    iss >> result;

    return result;
}

int check_signle(CircuitGraph& circuit,std::vector<int>& nodeValues,std::vector<bool>& nodeValid,int nodefault, int stuck_value )
{
    // First it is not activate
    if (nodeValues[nodefault]==xx)
    {
        for (size_t index = 0; index < circuit.gates.size(); ++index)
        {
            if (circuit.gates[index].inputs[0] == nodefault)
            {
                if (circuit.gates[index].type == INV || circuit.gates[index].type == BUF )
                {
                    return index;
                }
            }
        }
    }  
    return -1;
}

