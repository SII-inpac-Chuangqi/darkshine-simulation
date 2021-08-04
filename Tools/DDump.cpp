#include <iostream>
#include <string>

#include "TFile.h"
#include "TTree.h"

#include "EventDump.h"
#include "Object/DEvent.h"

int main(int argc, char **argv)
{
    if((TString)argv[1] == (TString)"-h")
    {
        EventDump dump;
        return -1;
    }

    if((argc < 3 && argc > 0) || argc > 5)
    {
        std::cout << "Error Wrong arguments, try DDump -h" << std::endl;
        return -1;
    }

    std::string file_name = "dp_out.root";
    std::string tree_name = "Dark_Photon";
    long long skip_number = -1;
    long long event_number = -1;

    if(argc >= 3)
    {
        file_name = argv[1];
        tree_name = argv[2];
    }
    if(argc > 3)
        skip_number = std::stoll(argv[3]);
    if(argc > 4)
        event_number = std::stoll(argv[4]);

    EventDump dump(file_name, tree_name);
    dump.Dump(skip_number, event_number);

    return 0;
}
