//
// Created by Zhang Yulei on 12/18/20.
//

#include <iostream>

#include "TFile.h"

#include "DEventDisplay.h"
#include "DisData.h"

#include "Utility/parser.h"

int main(int argc, char **argv) {

    bool batch_mode = false;
    auto file_in = TString("dp_simu.root");
    auto conf_in = TString("");
    auto geo_file_in = TString("dp_simu.root");
    bool no_strip_mode = false;

    arg_parser::Parser parser;
    parser.Add("f,input",  file_in, "read the event information from input root file"); 
    parser.Add("c,config", conf_in, "read the DAna config (optional)");
    parser.Add("g,geometry", geo_file_in, "read the geometry from input root file");
    parser.AddFlag("t", no_strip_mode, false, "read the geometry from input root file");
    parser.AddFlag("b", batch_mode, false, "only print out detector geometry information");
    parser.Parse(argc, argv);

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
