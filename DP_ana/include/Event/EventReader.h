//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DSIMU_EVENTREADER_H
#define DSIMU_EVENTREADER_H

#include "TROOT.h"
#include <TChain.h>
#include <TFile.h>
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TGeoManager.h"

#include "Event/AnaEvent.h"
#include "EventStoreAndWriter.h"

using namespace std;

class EventReader {
public:
    EventReader() : evt(nullptr) {};

    ~EventReader();

    void RegisterOutput();

    void Convert();

    bool ReadEntry(int i);

    Long64_t GetEntries() const;

    Int_t ReadFile(const std::string &filename);

    Int_t ReadTree(const std::string &treename, TFile *tfile);

    void ReadGeometry(const std::string &filename);

    AnaEvent *getEvt() const {
        return evt;
    }

    void setEvt(AnaEvent *in) {
        evt = in;
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

    const shared_ptr<EventStoreAndWriter> &getEvtWrt() const {
        return EvtWrt;
    }

    void setEvtWrt(const shared_ptr<EventStoreAndWriter> &evtWrt) {
        EvtWrt = evtWrt;
    }

    TFile *getDataFile() const {
        return data_file;
    }

private:
    TFile* data_file;

    AnaEvent *evt{nullptr};
    TBranch *b_DEvent;   //!

    Long64_t Entries{0};
    shared_ptr<EventStoreAndWriter> EvtWrt;

    int runNumber{0};
    int eventNumber{-1};
    int skipNumber{0};
    int eventProcessedNumber{0};

    double ECAL_Leak{0.};
    double PNEnergy_Target{0.};
    double PNEnergy_ECAL{0.};

    // Verbosity
    int Verbose{0};

    TFile *f{nullptr};
    TTree *input_tree{nullptr};

    // Declaration of leaf types
    Int_t RunNumber{0};
    Int_t EventNumber{0};
    Double_t Rndm[4] = {0, 0, 0, 0};

};

#endif //DSIMU_EVENTREADER_H
