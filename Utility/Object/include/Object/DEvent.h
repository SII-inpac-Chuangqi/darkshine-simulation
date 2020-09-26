//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef DSIMU_DEVENT_H
#define DSIMU_DEVENT_H

#include <cstddef>
#include <cstdlib>
#include <map>
#include <iostream>
#include <memory>

#include "Object/CalorimeterHit.h"
#include "Object/McParticle.h"
#include "Object/ReconstructedParticle.h"
#include "Object/SimulatedHit.h"
#include "DStep.h"

#include "TObject.h"

enum CleanType {
    nALL, nVector
};

class DEvent : public TObject {
public:
    // Constructor
    DEvent() { Initialization(nALL); };

    ~DEvent() override {
        Initialization(nALL);
    }

    // Operators

    // Get Methods
    int getRunId() const {
        return RunID;
    }

    int getEventId() const {
        return EventID;
    }

    int getVerbose() const {
        return Verbose;
    }

    const double *getRndm() const {
        return Rndm;
    }

    const DStepMapO &getStepCollection_Old() const {
        return StepCollection;
    }

    const MCParticleMapO &getMcParticleCollection_Old() const {
        return MCParticleCollection;
    }

    const RecParticleMapO &getRecParticleCollection_Old() const {
        return RecParticleCollection;
    }

    const SimulatedHitMapO &getSimulatedHitCollection_Old() const {
        return SimulatedHitCollection;
    }

    const CalorimeterHitMapO &getCalorimeterHitCollection_Old() const {
        return CalorimeterHitCollection;
    }

    // Set Methods
    void setRunId(int runId) {
        RunID = runId;
    }

    void setEventId(int eventId) {
        EventID = eventId;
    }

    void setVerbose(int verbose) {
        Verbose = verbose;
    }

    void setRndm(const double *rndm) {
        for (int i = 0; i < 4; ++i)
            Rndm[i] = rndm[i];
    }

    virtual // Initialization
    void Initialization(CleanType);

    // Register Collections
    DStepVec *RegisterStepCollection(const std::string &);

    MCParticleVec *RegisterMCParticleCollection(const std::string &);

    RecParticleVec *RegisterRecParticleCollection(const std::string &);

    SimulatedHitVec *RegisterSimulatedHitCollection(const std::string &);

    CalorimeterHitVec *RegisterCalorimeterHitCollection(const std::string &);

    // Delete Collections
    void DeleteCollection(const std::string &);

    // List all Registered Collections for template T
    template<class T>
    std::vector<std::string> *ListCollections(const T &);

    virtual std::vector<std::string> *ListAllCollections();

    /*
     * Miscellaneous (truth)
     */
    double getPnEnergyTarget() const {
        return PNEnergy_Target;
    }

    void setPnEnergyTarget(double pnEnergyTarget) {
        PNEnergy_Target = pnEnergyTarget;
    }

    double getPnEnergyEcal() const {
        return PNEnergy_ECAL;
    }

    void setPnEnergyEcal(double pnEnergyEcal) {
        PNEnergy_ECAL = pnEnergyEcal;
    }

    double getEleakEcal() const {
        return Eleak_ECAL;
    }

    void setEleakEcal(double eleakEcal) {
        Eleak_ECAL = eleakEcal;
    }

    McParticle* SearchID(MCParticleVec* mv, int ID) {
        for (auto itr : *mv) {
            if (itr->getId() == ID) return itr;
        }
        return nullptr;
    }
protected:
    // run number
    int RunID{0};

    // event number
    int EventID{0};

    /*
     * Miscellaneous (truth)
     */
    // Random Number Seeds
    double Rndm[4]{};
    // Photon-Nuclear reaction Energy in Target/ECAL region
    double PNEnergy_Target{0.};
    double PNEnergy_ECAL{0.};
    // Energy leakage in ECAL holder
    double Eleak_ECAL{0.};

    /*
     * Miscellaneous (Reconstructed)
     */
    double TotalRecEnergy{0.};
    double ECALRecEnergy{0.};
    double HCALRecEnergy{0.};

    // Verbosity
    int Verbose{0};

    // Step Collection
    DStepMapO StepCollection;

    // Event Physics Quantities
    MCParticleMapO MCParticleCollection;
    RecParticleMapO RecParticleCollection;
    SimulatedHitMapO SimulatedHitCollection;
    CalorimeterHitMapO CalorimeterHitCollection;

ClassDef(DEvent, 1);
};

// Some inline
template<class T>
std::vector<std::string> *DEvent::ListCollections(const T &in) {
    auto tmp = new std::vector<std::string>;
    for (auto itr : in) tmp->emplace_back(itr.first);

    return tmp;
}

#endif //DSIMU_DEVENT_H

