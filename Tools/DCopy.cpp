#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "TFile.h"
#include "TTree.h"

#include "Object/DEvent.h"
#include "Utility/parser.h"

std::vector<int> GetEventIds(const std::string &txt_name)
{
    std::ifstream file(txt_name);
    if (!file) {}

    std::vector<int> numbers;

    std::string line;
    std::string token;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        
        while (std::getline(ss, token, ','))
            numbers.push_back(std::stoi(token));
    }
    
    file.close();

    return numbers; 
}

int main(int argc, char **argv) {
    std::string file_name = "dp_simu.root";
    std::string tree_name = "Dark_Photon";
    std::string txt_name = "events.txt";

    arg_parser::Parser parser;
    parser.Add("f,file", file_name, "dp_simu.root", "name of the file to dump");
    parser.Add("t,tree", tree_name, "Dark_Photon", "name of the tree storing DEvents");
    parser.Add("x,txt", txt_name, "events.txt", "name of the txt file containing event ids");

    parser.Parse(argc, argv);

    auto events = GetEventIds(txt_name);
   
//    for (int event : events)
//        std::cout << event << " ";
//    std::cout << std::endl;

    return 0;
}
