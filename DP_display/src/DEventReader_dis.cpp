//
// Created by Zhang Yulei on 9/19/20.
//

#include "DEventReader_dis.h"

#include <iostream>
#include <iomanip>

Int_t EventReader_D::ReadFile(const std::string &filename) {
    f = new TFile(TString(filename));
    if (!f) {
        std::cerr << "[READFILE ERROR] ==> File: " + filename + " does not exist." << std::endl;
        return -1;
    }

    ReadTree("Dark_Photon", f);

    return 1;
}

void EventReader_D::Convert() {
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

bool EventReader_D::ReadNextEntry() const {
// Read contents of entry.
    if (!treeReader) return false;
    return treeReader->Next();
}

bool EventReader_D::ReadEntry(int i) const {
    if (!treeReader) return false;
    if (treeReader->SetEntry(i) != TTreeReader::kEntryValid) return false;
    if ( i >= treeReader->GetEntries() ) return false;

    return true;
}

Long64_t EventReader_D::GetEntries() {
    return treeReader->GetEntries();
}

Int_t EventReader_D::ReadTree(const string &treename, TFile* tfile) {

    treeReader = shared_ptr<TTreeReader>(new TTreeReader(treename.data(),tfile));
    Entries = treeReader->GetEntries();

    EvtPtr =  shared_ptr<TTreeReaderValue<DEvent>>( new TTreeReaderValue<DEvent>(*treeReader,"DEvent") );

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





