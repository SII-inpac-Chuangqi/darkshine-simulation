//
// Created by Zhang Yulei on 9/19/20.
//

#include "Event/EventReader.h"

#include <iostream>
#include <iomanip>

Int_t EventReader::ReadFile(const std::string &filename) {
    auto tree = new TTree;
    f = new TFile(TString(filename));
    if (!f) {
        std::cerr << "[READFILE ERROR] ==> File: " + filename + " does not exist." << std::endl;
        return -1;
    }

    treeReader = shared_ptr<TTreeReader>(new TTreeReader("Dark_Photon",f));
    Entries = treeReader->GetEntries();

    EvtPtr = shared_ptr<TTreeReaderValue<DEvent> >( new TTreeReaderValue<DEvent>(*treeReader,"DEvent") ) ;

    if (Verbose > -1) {
        cout << "======================================================================" << endl;
        std::cout << "[ READ FILE ] : (Verbosity 0)" << std::endl;
        std::cout << left;
        std::cout << std::setw(5) << " " << std::setw(30);
        std::cout << "==> Input File: " << std::setw(30) << filename << std::endl;

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

    return 1;
}

void EventReader::RegisterOutput() {

    /*
     * Register some variables in output
     */
    EvtWrt->RegisterIntVariable("RunNumber", &RunNumber, "RunNumber/I");
    EvtWrt->RegisterIntVariable("EventNumber", &eventProcessedNumber, "EventNumber/I");
    EvtWrt->RegisterDoubleVariable("Rndm", Rndm, "Rndm[4]/D");

    // Initialized Processed Number
    eventProcessedNumber = 0;
}


void EventReader::Convert() {
    /*
     *
     */

    // Initialization
    evt->Initialization(nALL);

    evt->ConvertTreeValuePtr(EvtPtr);
    evt->LinkChildren();

    RunNumber = evt->getRunId();
    EventNumber = evt->getEventId();
    for (int i = 0; i < 4; ++i) Rndm[i] = evt->getRndm()[i];

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

bool EventReader::ReadNextEntry() const {
// Read contents of entry.
    if (!treeReader) return false;
    return treeReader->Next();
}



