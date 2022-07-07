//
// Created by Zhang Yulei on 9/24/20.
//

#ifndef DSIMU_SECONDARYMAXEFINDER_H
#define DSIMU_SECONDARYMAXEFINDER_H

#include "TString.h"
#include "Core/AnaProcessor.h"
#include "Object/McParticle.h"

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

    AnaEvent *getEvt() const {
        return Evt;
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

    void setEvt(AnaEvent *evt) {
        Evt = evt;
    }

    // Process
    void RegisterParameters();
    void RegisterParameters_visible();

    McParticle* FindSecondary(int PDG = 0, double Emin = 0., McParticle *mcp = nullptr);
    McParticle* FindDPDecay(McParticle *mcp);

private:

    AnaEvent* Evt{};
    shared_ptr<EventStoreAndWriter> EvtWrt;

    int Verbose{0};

    int Secondary_Found{0};
    int Secondary_PDG{0};
    double Secondary_MaxE{0.};
    double Secondary_MaxE_P[3];
    TString Secondary_MaxE_PVName;
    TString Secondary_MaxE_Process;
    // add for visible decay
    int DP_decay{0};
    double E_decay_1{0};
    double E_decay_2{0};
    double P_decay_1[3];
    double P_decay_2[3];
    int PDG_decay_1{0};
    int PDG_decay_2{0};

};


#endif //DSIMU_SECONDARYMAXEFINDER_H
