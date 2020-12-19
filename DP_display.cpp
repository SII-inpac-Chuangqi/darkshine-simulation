//
// Created by Zhang Yulei on 12/18/20.
//

#include "EventDisplay.h"

#include "TFile.h"

#include <iostream>

namespace {
    void PrintUsage() {
        std::cerr << "Usage: " << std::endl;
        std::cerr << "  DAna [ -b ] [ -f geometry.root]" << std::endl;
        std::cerr << " -- [-b] : only print out detector geometry information" << std::endl;
        std::cerr << " -- [-f] : read the geometry from input root file" << std::endl;
        std::cerr << std::endl;
    }
}

int main(int argc, char **argv) {

    if (argc > 3 || argc < 2) {
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
        else
            return -1;
    }

    auto EvtDisplay = new EventDisplay();
    EvtDisplay->readGeo(file_in);

    if (batch_mode) {
        EvtDisplay->inspectMainRegion();
        return 0;
    }
    else{
        EvtDisplay->Initialize();

        EvtDisplay->drawDetector();

        EvtDisplay->Open();

    }

    return 1;
};