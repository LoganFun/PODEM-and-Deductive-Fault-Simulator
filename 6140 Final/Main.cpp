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

int main() 
{  
    std::vector<int> nodeFaults_temp;

    // Deductive part 
    // or Final PODEM part
    char part_decision = input_part();

    if (part_decision == 'A' || part_decision == 'a')
    {    
        // Whether Test fault coverage or not
        // Check Option A or B or C
        char sim_mode = input_simmode();

        while (true)
        {
            //Input values
            std::string inputValue;
            std::string label;
            //std::vector<bool> inputPattern;
            std::vector<int> inputPattern;

            if (sim_mode == 'A' || sim_mode == 'a'|| 
                sim_mode == 'B' || sim_mode == 'b')
            {
                // Get the Input for Deductive Fault Simulation     
               input(inputValue,label, inputPattern);
            }
            else if (sim_mode == 'C' || sim_mode == 'c')
            {
                // Generate Random Input for Deductive Fault Coverage
                rand_input(inputValue,label, inputPattern);
            }
            else
            {
                std::cout << "Wrong Simulation Mode " <<std::endl;
            }

            //Circuit Generation
            std::vector<int> inputGates={};
            std::vector<int> outputGates={};
            std::vector<int> nodeValues(5000, 0);
            std::vector<bool> nodeValid(5000, false);

            //Fault Propagation
            std::vector<bool>nodeFaultValid(5000,false);

            // Initialize the Node Faults
            std::vector<std::vector<int>> nodeFaults_new;
            for(int j =0; j<5000; j++)
            {
                nodeFaults_new.push_back(std::vector<int>());
            }

            std::vector<int> nodeFaults_all;   

            //construct file path
            std::string filename = "/home/logan/Study/6140/files/" + label + ".txt";
            std::string faultfilename = "/home/logan/Study/6140/files/" + label + "fault.txt";

            // Declare necessary Gates
            // Construct the Circuit
            CircuitGraph circuit = parseCircuitFile(filename, inputGates, outputGates, nodeValues, nodeValid);
            
            // Simulate Gates 
            // Simulate all the node valid and values
            simulateCircuit(circuit, inputPattern, inputGates, nodeValues, nodeValid);
            // Get the Max node for following steps
            // To save space and improve speed
            int maxNode = maxnode(circuit);

            // Print the Circuit
            // Output the Circuit for Project 1
            //output_Original(nodeValues, outputGates);
            
            //A Sim
            if (sim_mode == 'A' || sim_mode == 'a')
            {
                fault_sim_A(circuit,nodeFaultValid, nodeFaults_new, nodeFaults_all, 
                    outputGates,nodeValues,inputPattern,inputGates, maxNode);
                // Output Fault List for Deductive Simulation
                output_Fault(nodeFaults_new,outputGates);

            }
            //B Sim
            else if(sim_mode == 'B' || sim_mode == 'b')
            {
                fault_sim_B(circuit, nodeFaultValid, nodeFaults_new, nodeFaults_all, 
                    outputGates,nodeValues,inputPattern,inputGates,faultfilename);  

                //Output Fault List for Deductive Simulation
                output_Fault(nodeFaults_new,outputGates);          
            }
            // C Sim
            else if(sim_mode == 'C' || sim_mode == 'c')
            {
                fault_sim_A(circuit,nodeFaultValid, nodeFaults_new, nodeFaults_all, 
                    outputGates,nodeValues,inputPattern,inputGates, maxNode);
                // Output Fault List for Deductive Simulation
                output_Fault(nodeFaults_new,outputGates);             
                // Output Fault Coverage for Deductive Simulation
                output_FaultCov(nodeFaults_new, nodeFaults_all,nodeFaults_temp,outputGates);
            }
            else
            {
                std::cout << "Wrong Simulation Mode " <<std::endl;
            }

        }

        
    }
    // PODEM
    else if (part_decision == 'B' || part_decision == 'b')
    {
        // Select the File at the beginning
        // Pass the Label to file Address
        std::string label;
        std::cout << "Please input the label you want (For example:s27):";
        std::cin >> label;
        
        //std::cout << "label:"<<label;
        //construct file path
        std::string filename = "/home/logan/Study/6140/files/" + label + ".txt";
        std::string faultfilename = "/home/logan/Study/6140/files/" + label + "_podem_fault.txt";

        // A Loop to input all the fault into to simulator
        std::ifstream file(faultfilename); 
        std::string line;     

        // Check if the file is open
        if (file.is_open()) 
        {
            // Skip the first line
            std::getline(file, line);

            // Read file line by line
            while (std::getline(file, line)) 
            {
                int firstValue, secondValue;

                // Using a stringstream to separate the values
                std::stringstream ss(line);

                // Read the two values from the line
                ss >> firstValue >> secondValue;

                // Output the values
                std::cout << "node has fault: " << firstValue << ", stuck_value : " << secondValue << std::endl;

                /* The node that has fault*/
                int nodefault = firstValue;
                /*The s-a fault value at node*/
                int stuck_value = secondValue;
                
                //Circuit Generation
                /* input node ID */
                std::vector<int> inputGates={};
                std::vector<int> outputGates={};
                std::vector<int> nodeValues(5000, xx);
                //std::cout<<"node value 2 and 5: " << nodeValues[2] << nodeValues[5] << std::endl;
                std::vector<bool> nodeValid(5000, false);

                //Fault Propagation
                std::vector<bool>nodeFaultValid(5000,false);

                // Initialize the Node Faults
                std::vector<std::vector<int>> nodeFaults_new;
                for(int j =0; j<5000; j++)
                {
                    nodeFaults_new.push_back(std::vector<int>());
                }

                // Store D-Frontier
                std::vector<int> D_F;

                //Construct the Circuit
                
                CircuitGraph circuit = parseCircuitFile(filename, inputGates, outputGates, nodeValues, nodeValid);
                
                //std::cout<<"node value 2 and 10: " << nodeValues[2] << nodeValues[5] << std::endl;
                
                //Initialize some Signal
                // Initial Signal Gate End Signal
                for (LogicGate& gate : circuit.gates)
                {
                    gate.finish = false;
                }

                // Begin to use podem
                bool podem_result = podem(circuit, outputGates, nodeValues, nodeValid, D_F, nodefault, stuck_value, inputGates);
                //std::cout << "Disapper Test "<<std::endl;
                //std::cout<<"node value 2 and 10: " << nodeValues[2] << nodeValues[5] << std::endl;

                // Output the vector
                // At the input position
                std::vector<std::string> input_trans = {"0", "1", "1","0","X"};
                

                if (podem_result)
                {
                    std::cout<<"    PODEM Result: ";
                    for (auto node : inputGates)
                    {
                        std::cout<<input_trans[nodeValues[node]];
                    }
                    std::cout<<std::endl;

                    // std::cout << "In the Main Value at node index 27 "  << " is " << nodeValues[27] << std::endl;
                    // std::cout << "In the Main Value at node index 183 "  << " is " << nodeValues[183] << std::endl;

                }
                else
                {
                    std::cout<<"PODEM Fail"<<std::endl;
                }     
            }
            file.close(); // Close the file
        } 
        else 
        {
            std::cout << "Unable to open file";
        }               
    }
    else
    {
        std::cout<<"Wrong Input Mode" <<std::endl;
    }
    return 0;
}
