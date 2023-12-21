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


int maxnode(CircuitGraph circuit)
{
    int result=1;
    for (LogicGate& gate : circuit.gates)
    {
        //std::cout<<"gate type:"<<gate.type<<std::endl;
        if (gate.inputs.size()==2)
        {
            if (gate.inputs[0]>result)
            {
                result = gate.inputs[0];
            }
            if (gate.inputs[1]>result)
            {
                result = gate.inputs[1];
            }
            if (gate.output>result)
            {
                result = gate.output;
            }            
        }
        else
        {
            if (gate.inputs[0]>result)
            {
                result = gate.inputs[0];
            }
            if (gate.output>result)
            {
                result = gate.output;
            }  
        }
        //std::cout<<"Max node:"<<result<<std::endl;
    }

    return result;
}

int inputNum()
{
    int numInputs;
    std::cout << "Please input the number of inputs:  " << std::endl;
    std::cin >> numInputs;
    return numInputs;
}

void rand_input(std::string& inputValue, std::string& label,
    //std::vector<bool>& inputPattern)
    std::vector<int>& inputPattern)
{
    int num;  

    std::cout << "Please input the data and input size (for example: s27:7): " ;
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, inputValue); 
    size_t colonPosition = inputValue.find(':');

    if (colonPosition != std::string::npos) 
    {
        label = inputValue.substr(0, colonPosition);
        // Jump : 
        num = std::stoi(inputValue.substr(colonPosition + 1)); 
    } 
    else 
    {
        std::cerr << "Wrong input\n";
    }

    //random generation
    std::random_device rd;  // 随机数生成器
    std::mt19937 gen(rd()); // 以 rd() 作为种子的 Mersenne Twister 生成器
    //std::bernoulli_distribution dist(0.5); // 生成真假概率各为50%的分布
    std::uniform_int_distribution<> dist(0, 1);
    //std::vector<bool> result(num);
    std::vector<int> result(num);
    std::generate(result.begin(), result.end(), [&]() { return dist(gen); });
    
    inputPattern = result;

}

void input(std::string& inputValue, std::string& label,
    std::vector<int>& inputPattern)
{
    // Cout Guide
    std::cout << "Please input the data (for example: s27:1110101): ";
    
    // Follow a cin so that we need to clean the zone
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, inputValue); 

    //std::cout << "Input Value: " << inputValue << std::endl;

    size_t colonPosition = inputValue.find(':');
    if (colonPosition != std::string::npos) {
        label = inputValue.substr(0, colonPosition);
        // Jump : 
        std::string binaryStr = inputValue.substr(colonPosition + 1); 

        for (char c : binaryStr) {
            if (c == '0') 
            {
                inputPattern.push_back(0);  
            } 
            else if (c == '1')
            {
                inputPattern.push_back(1);  
            }
            else 
            {
                std::cerr << "Wrong: Nonbinary\n";
            }
        }
    } 
    else 
    {
        std::cerr << "Wrong input\n";
    }
}

void output_Original(std::vector<bool>& nodeValues, std::vector<int> outputGates)
{
    for (int outputID : outputGates) 
    {
        std::cout << "Output " << outputID << ": " << nodeValues[outputID] << std::endl;
    }
}

char input_simmode()
{
    char result;
    std::cout<<"Please input the fault sim model " << std::endl;
    std::cout<<"    Simulate all faults (A/a), " << std::endl;
    std::cout<<"    Simulate indicated faults (B/b), " << std::endl;
    std::cout<<"    Find fault coverage (C/c):  " ;
    std::cin>>result;
    std::cout<<std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return result;
}

void output_Fault(
    //std::vector<std::vector<std::vector<int>>> nodeFaults,
    std::vector<std::vector<int>> nodeFaults_new,
    std::vector<int> outputGates
)
{
    std::vector<int> detectedFaults;
    //Find the final set using union
    for(int outputID : outputGates)
    {
        detectedFaults = union_sets_new(detectedFaults, nodeFaults_new[outputID]);       
    }
    //output them
    for(const auto& subvec : detectedFaults)
    {
            std::cout << subvec/10 << "s-a-" << subvec%10<<std::endl;
    }

}

void output_FaultCov(
    std::vector<std::vector<int>>& nodeFaults_new,
    std::vector<int>& nodeFaults_all,
    std::vector<int>& nodeFaults_temp,
    std::vector<int> outputGates)
{

    for(int outputID : outputGates)
    {
        nodeFaults_temp = union_sets_new(nodeFaults_temp, nodeFaults_new[outputID]);       
    }    

    float coverage = (float)nodeFaults_temp.size()/(float)nodeFaults_all.size();
    std::cout << "temp size: "<<(float)nodeFaults_temp.size()<<std::endl;
    std::cout << "all size: "<<(float)nodeFaults_all.size()<<std::endl;
    std::cout<<"Fault Coverage: "<<coverage*100<<std::endl;

}

void read_B_fault(std::string faultfilename,
    std::vector<std::vector<int>>& nodeFaults_new,
    std::vector<int>& nodeFaults_all,std::vector<int> &nodeValues
    )
{   
    std::string line;
    std::string token;

    int takeNode;
    int stuckValue;

    // First Read
    std::ifstream faultfile(faultfilename);

        // Check Open or not
        if (!faultfile.is_open()) 
        {
            std::cerr << "Failed to open fault description file." << std::endl;
        }
        else
        {
            std::cerr << "Sucessfully open fault description file." << std::endl;
        }    
    
    // Skip the First Line
    std::getline(faultfile, line);

    // Following Lines 
    while (faultfile >> takeNode >> stuckValue)
    {
        std::cout << "Tested Fault List: " <<std::endl;
        std::cout << "Net number: " << takeNode << ", Stuck-at-value: " << stuckValue << std::endl;
    
        // Then Inject
        // Same to aviod some unneccsary faults
        if (nodeValues[takeNode] == 1 && stuckValue == 0)
        {
            std::string combinedString = std::to_string(takeNode) + std::to_string(0);
            int combinedNumber = std::stoi(combinedString);
            nodeFaults_new[takeNode].push_back(combinedNumber);
            
        }
        else if (nodeValues[takeNode] == 0 && stuckValue == 1)
        {
            std::string combinedString = std::to_string(takeNode) + std::to_string(1);
            int combinedNumber = std::stoi(combinedString);
            nodeFaults_new[takeNode].push_back(combinedNumber);
        }
        
        //For all fault coverage
        // Include a total set
        std::string combinedString2 = std::to_string(takeNode) + std::to_string(stuckValue);
        int combineNumber2 = std::stoi(combinedString2);
        nodeFaults_all.push_back(combineNumber2);   
    
    }
    
    faultfile.close();

    // // Check output
    // // 输出二维向量
    // for (const auto& row : nodeFaults_new) {
    //     for (int val : row) {
    //         std::cout << val << " ";
    //     }
    //     std::cout << std::endl;
    // }

}

char input_part()
{
    char result;
    std::cout<<"Please input the part you wanna test:"<<std::endl;
    std::cout<<"    Deductive (A/a) or PODEM (B/b): ";
    std::cin>>result;
    std::cout<<std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return result;    
}