#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"

#include "EventDump.h"
#include "Object/DEvent.h"
#include "Utility/parser.h"

int main(int argc, char **argv) {
    std::string file_name = "dp_simu.root";
    std::string tree_name = "Dark_Photon";
    long skip_number = -1;
    long event_number = -1;

    arg_parser::Parser parser;
    parser.Add("f,file", file_name, "dp_simu.root", "name of the file to dump");
    parser.Add("t,tree", tree_name, "Dark_Photon",  "name of the tree storing DEvents");
    parser.Add("s,skip", skip_number, -1,  "optional: not set or < 0, dump all events; >= 0, skip events from 0 to skip number");
    parser.Add("e,event", event_number, -1, "optional: activated if skip number >= 0; not set or < 0, dump all events unskipped; >= 0, dump events from skip number to skip number + event number");
    parser.Parse(argc, argv);

    EventDump dump(file_name, tree_name);
    dump.Dump(skip_number, event_number);

    return 0;
}
