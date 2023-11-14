//
// Created by Zhang Yulei on 12/18/20.
//

#include "DEventDisplay.h"
#include "DisData.h"

#include "TFile.h"

#include <iostream>

namespace {
    void PrintUsage() {
        std::cerr << "Usage: " << std::endl;
        std::cerr << "  DDis [ -b ] [ -f dp_simu.root] [ -g geometry.root] [ -t ] [ -h ]" << std::endl;
        std::cerr << " -- [-b] : only print out detector geometry information" << std::endl;
        std::cerr << " -- [-g] : read the geometry from input root file" << std::endl;
        std::cerr << " -- [-t] : use this option when reading truth tracker hit" << std::endl;
        std::cerr << " -- [-f] : read the event information from input root file" << std::endl;
        std::cerr << " -- [-c] : read the DAna config (optional)" << std::endl;
        std::cerr << " -- [-h] : show this help usage" << std::endl;
        std::cerr << std::endl;
    }
}

int main(int argc, char **argv) {

    if (argc < 1) {
        PrintUsage();
        return 1;
    }

    bool batch_mode = false;
    auto file_in = TString("dp_simu.root");
    auto conf_in = TString("");
    auto geo_file_in = TString("dp_simu.root");
    bool no_strip_mode = false;

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-f")
            file_in = TString(argv[i+1]);
        else if (std::string(argv[i]) == "-c")
            conf_in = TString(argv[i+1]);
        else if (std::string(argv[i]) == "-g")
            geo_file_in = TString(argv[i+1]);
        else if (std::string(argv[i]) == "-t")
            no_strip_mode = true;
        else if (std::string(argv[i]) == "-b")
            batch_mode = true;
        else if (std::string(argv[i]) == "-h") {
            PrintUsage();
            return 0;
        }
    }

    DisData::CreateInstance();
    if(geo_file_in==""){
        geo_file_in=file_in;
        std::cout<<"No geometry file provided: read from input"<<std::endl;
    }
        
    dDisData->SetGeoFile(geo_file_in);

    auto EvtDisplay = new DEventDisplay();
    EvtDisplay->setNoStripMode(no_strip_mode);
    EvtDisplay->readFile(file_in);
    EvtDisplay->readGeo(dDisData->GetGeoFile());

    EvtDisplay->inspectMainRegion();
    if (batch_mode)
        return 0;

    else{
        EvtDisplay->Initialize();
        //EvtDisplay->bookSlot();
        EvtDisplay->loadAnaConfig(conf_in);
        EvtDisplay->readEvt();

        // default Draw 0-th event
        EvtDisplay->drawEvent(0);

        EvtDisplay->Open(EvtDisplay);

    }

    return 1;
}
