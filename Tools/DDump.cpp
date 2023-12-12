#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"

#include "EventDump.h"
#include "Object/DEvent.h"

int main(int argc, char **argv) {
    std::string file_name = "dp_simu.root";
    std::string tree_name = "Dark_Photon";
    long long skip_number = -1;
    long long event_number = -1;

    for(int i = 1; i < argc - 1; i++)
    {
        if ((TString) argv[1] == (TString) "-h") {
            EventDump dump;
            return -1;
        }

        if     ((TString) argv[i] == (TString) "-f")
            file_name = argv[i + 1];
        else if((TString) argv[i] == (TString) "-t")
            tree_name = argv[i + 1];
        else if((TString) argv[i] == (TString) "-j")
        {
            TString temp(argv[i + 1]);
            if(temp.IsDec()) skip_number = temp.Atoll();
        }
        else if((TString) argv[i] == (TString) "-e")
        {
            TString temp(argv[i + 1]);
            if(temp.IsDec()) event_number = temp.Atoll();
        }
    }

    EventDump dump(file_name, tree_name);
    dump.Dump(skip_number, event_number);

    return 0;
}
