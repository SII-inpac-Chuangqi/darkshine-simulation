//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DSIMU_EVENTREADERDIS_H
#define DSIMU_EVENTREADERDIS_H

#include "TROOT.h"
#include <TChain.h>
#include <TFile.h>
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

#include "Object/DEvent.h"
#include "Event/AnaEvent.h"

using namespace std;

class EventReader_D : public TNamed {
public:
    EventReader_D() : evt(nullptr) {};

    ~EventReader_D() override = default;

    void Convert();

    bool ReadNextEntry() const;
    bool ReadEntry(int i) const;
    Long64_t GetEntries();

    Int_t ReadFile(const std::string &filename);
    Int_t ReadTree(const std::string &treename, TFile* f);

    TFile* getFile() const {return f;}

    const shared_ptr<AnaEvent> &getEvt() const {
        return evt;
    }

    void setEvt(const shared_ptr<AnaEvent> &Evt) {
        EventReader_D::evt = Evt;
    }

    Long64_t getEntries() const {
        return Entries;
    }

    int getVerbose() const {
        return Verbose;
    }

    void setVerbose(int verbose) {
        Verbose = verbose;
    }

    int getRunNumber() const {
        return runNumber;
    }

    void setRunNumber(int runnumber) {
        runNumber = runnumber;
    }

    int getEventNumber() const {
        return eventNumber;
    }

    void setEventNumber(int eventnumber) {
        eventNumber = eventnumber;
    }

    int getSkipNumber() const {
        return skipNumber;
    }

    void setSkipNumber(int skipnumber) {
        skipNumber = skipnumber;
    }


private:
    //DEvent *evt{nullptr};
    shared_ptr<AnaEvent> evt;
    Long64_t Entries{0};

    int runNumber{0};
    int eventNumber{-1};
    int skipNumber{0};
    int eventProcessedNumber{0};

    // Verbosity
    int Verbose{0};

    TFile *f{nullptr};
    shared_ptr<TTreeReader> treeReader;
    shared_ptr<TTreeReaderValue<DEvent> > EvtPtr;

    // Declaration of leaf types
    Int_t RunNumber{0};
    Int_t EventNumber{0};
    Double_t Rndm[4] = {0,0,0,0};

    ClassDefOverride(EventReader_D,0);
};

#endif //DSIMU_EVENTREADERDIS_H
