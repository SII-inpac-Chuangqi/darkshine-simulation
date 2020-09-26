//
// Created by Zhang Yulei on 9/24/20.
//

#ifndef DSIMU_SECONDARYMAXEFINDER_H
#define DSIMU_SECONDARYMAXEFINDER_H

#include "TString.h"
#include "Core/AnaProcessor.h"

class SecondaryMaxEFinder {
public:

    explicit SecondaryMaxEFinder(const shared_ptr<EventStoreAndWriter> &evtWrt) : EvtWrt(evtWrt) {}

// Get Methods
    int getVerbose() const {
        return Verbose;
    }

    double getSecondaryMaxE() const {
        return Secondary_MaxE;
    }

    const TString &getSecondaryMaxEPvName() const {
        return Secondary_MaxE_PVName;
    }

    const TString &getSecondaryMaxEProcess() const {
        return Secondary_MaxE_Process;
    }

    const shared_ptr<EventStoreAndWriter> &getEvtWrt() const {
        return EvtWrt;
    }

    // Set Methods
    void setVerbose(int verbose) {
        SecondaryMaxEFinder::Verbose = verbose;
    }

    void setSecondaryMaxE(double secondaryMaxE) {
        Secondary_MaxE = secondaryMaxE;
    }

    void setSecondaryMaxEPvName(const TString &secondaryMaxEPvName) {
        Secondary_MaxE_PVName = secondaryMaxEPvName;
    }

    void setSecondaryMaxEProcess(const TString &secondaryMaxEProcess) {
        Secondary_MaxE_Process = secondaryMaxEProcess;
    }

    void setEvtWrt(const shared_ptr<EventStoreAndWriter> &evtWrt) {
        EvtWrt = evtWrt;
    }

    // Process
    void RegisterParameters();

private:

    shared_ptr<EventStoreAndWriter> EvtWrt;

    int Verbose{0};

    double Secondary_MaxE;
    TString Secondary_MaxE_PVName;
    TString Secondary_MaxE_Process;

};


#endif //DSIMU_SECONDARYMAXEFINDER_H
