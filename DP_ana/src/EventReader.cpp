//
// Created by Zhang Yulei on 9/19/20.
//

#include "Event/EventReader.h"

#include <iostream>
#include <iomanip>

Int_t EventReader::ReadFile(const std::string &filename) {
    f = new TFile(TString(filename));
    if (!f->IsOpen()) {
        std::cerr << "[READFILE ERROR] ==> File: " + filename + " does not exist." << std::endl;
        return -1;
    }

    ReadTree("Dark_Photon", f);

    return 1;
}

void EventReader::RegisterOutput() {

    /*
     * Register some variables in output
     */
    EvtWrt->RegisterIntVariable("RunNumber", &RunNumber, "RunNumber/I");
    EvtWrt->RegisterIntVariable("EventNumber", &eventProcessedNumber, "EventNumber/I");
    EvtWrt->RegisterDoubleVariable("Rndm", Rndm, "Rndm[4]/D");
    EvtWrt->RegisterDoubleVariable("PNEnergy_Target", &PNEnergy_Target, "PNEnergy_Target/D");
    EvtWrt->RegisterDoubleVariable("PNEnergy_ECAL", &PNEnergy_ECAL, "PNEnergy_ECAL/D");
    EvtWrt->RegisterDoubleVariable("ECAL_Leak", &ECAL_Leak, "ECAL_Leak/D");


    // Initialized Processed Number
    eventProcessedNumber = 0;
}


void EventReader::Convert() {
    /*
     *
     */
    evt->LinkChildren();
    RunNumber = evt->getRunId();
    EventNumber = evt->getEventId();
    for (int i = 0; i < 4; ++i) Rndm[i] = evt->getRndm()[i];

    PNEnergy_Target = evt->getPnEnergyTarget();
    PNEnergy_ECAL = evt->getPnEnergyEcal();
    ECAL_Leak = evt->getEleakEcal();

    eventProcessedNumber++;
    if (Verbose > 1) {
        cout << "======================================================================" << endl;
        std::cout << "[ READ EVENT ] : Convert (Verbosity 2)" << std::endl;
        std::cout << setw(0) << "These collections will be generated from this event" << std::endl;
        std::cout << left;
        auto list = evt->ListAllCollections();
        for (const auto &l : *list) {
            std::cout << setw(5) << " " << "- " << l << std::endl;
        }
        list->clear();
        list->shrink_to_fit();
        delete list;
    }
}

/*                     */
/* From ROOT MakeClass */
/*                     */

bool EventReader::ReadEntry(int i) {
    if (!input_tree) return false;
    Long64_t ientry = input_tree->LoadTree(i);
    if (ientry < 0) return false;
    input_tree->GetEntry(i);
    return true;
}

Int_t EventReader::ReadTree(const string &treename, TFile *tfile) {

    input_tree = tfile->Get<TTree>(treename.data());
    Entries = input_tree->GetEntries();
    input_tree->SetMakeClass(1);

    input_tree->SetBranchAddress("DEvent", &evt, &b_DEvent);

    if (Verbose > -1) {
        cout << "======================================================================" << endl;
        std::cout << "[ READ FILE ] : (Verbosity 0)" << std::endl;
        std::cout << left;
        std::cout << std::setw(5) << " " << std::setw(30);
        std::cout << "==> Input File: " << std::setw(30) << tfile->GetName() << std::endl;
        std::cout << std::setw(5) << " " << std::setw(30);
        std::cout << "==> Input Tree: " << std::setw(30) << treename << std::endl;

        std::cout << std::setw(5) << " " << std::setw(30);
        std::cout << "==> Total Event(s): " << std::setw(30) << Entries << std::endl;

        std::cout << std::setw(5) << " " << std::setw(30);
        std::cout << "==> Skip Event(s): " << std::setw(30) << skipNumber << std::endl;

        int Evt = 0;
        if (eventNumber == -1)
            Evt = (Entries >= skipNumber) ? static_cast<int>(Entries) - skipNumber : 0;
        else if (Entries >= skipNumber)
            Evt = (Entries >= skipNumber + eventNumber) ? eventNumber : static_cast<int>(Entries) - skipNumber;
        else
            Evt = 0;

        std::cout << std::setw(5) << " " << std::setw(30);
        std::cout << "==> Process Event(s): " << std::setw(30) << Evt << std::endl;
    }

    return 0;
}

void EventReader::ReadGeometry(const std::string &filename) {
    data_file = new TFile(TString(filename));
    if (!data_file->IsOpen()) {
        std::cerr << "[READFILE ERROR] ==> File: " + filename + " does not exist." << std::endl;
        return;
    }

    std::cout << "[ READ Geometry ] ==> reading geometry from file: " << data_file->GetName() << std::endl;
    gGeoManager = (TGeoManager *) data_file->Get("DetGeoManager");
    if (!gGeoManager) {
        std::cerr << "[ READ Geometry ] ==> No Geometry in the file..." << std::endl;
        return;
    }
}

Long64_t EventReader::GetEntries() const {
    if (input_tree)
        return input_tree->GetEntries();
    else {
        cerr<<"==> Error when reading input tree..."<<endl;
        exit(EXIT_FAILURE);
    }
}

EventReader::~EventReader() {
    delete f;
}





