//
// Created by Zhang Yulei on 12/18/20.
//

#include "EventDisplay.h"

#include "TFile.h"

#include <iostream>

namespace {
    void PrintUsage() {
        std::cerr << "Usage: " << std::endl;
        std::cerr << "  DAna [ -b ] [ -f geometry.root] [ -h ]" << std::endl;
        std::cerr << " -- [-b] : only print out detector geometry information" << std::endl;
        std::cerr << " -- [-f] : read the geometry from input root file" << std::endl;
        std::cerr << " -- [-h] : show this help usage" << std::endl;
        std::cerr << std::endl;
    }
}

int main(int argc, char **argv) {

    if (argc > 3) {
        PrintUsage();
        return 1;
    }

    bool batch_mode = false;
    auto file_in = TString("dp_out.root");

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-f")
            file_in = TString(argv[i+1]);
        else if (std::string(argv[i]) == "-b")
            batch_mode = true;
        else if (std::string(argv[i]) == "-h")
            PrintUsage();
        else
            return -1;
    }

    auto EvtDisplay = new EventDisplay();
    EvtDisplay->readFile(file_in);
    EvtDisplay->readGeo();

    EvtDisplay->inspectMainRegion();
    if (batch_mode)
        return 0;

    else{
        EvtDisplay->Initialize();
        EvtDisplay->bookSlot();
        EvtDisplay->readEvt();

        //EvtDisplay->drawEvent(1);
        EvtDisplay->drawEvent(0);
        //EvtDisplay->drawDetector();

        EvtDisplay->Open(EvtDisplay);

    }

    return 1;
};