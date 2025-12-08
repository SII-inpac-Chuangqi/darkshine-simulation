#include <iostream>

#include "TFile.h"
#include "TTree.h"

#include "Object/DEvent.h"
#include "Utility/parser.h"

int main(int argc, char **argv) {
    std::string file_name = "dp_simu.root";
    std::string tree_name = "Dark_Photon";

    arg_parser::Parser parser;
    parser.Add("f,file", file_name, "dp_simu.root", "name of the file to dump");
    parser.Add("t,tree", tree_name, "Dark_Photon",  "name of the tree storing DEvents");

    parser.Parse(argc, argv);

    return 0;
}
