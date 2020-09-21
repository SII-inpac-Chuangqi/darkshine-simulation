//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef DSIMU_DEVENT_H
#define DSIMU_DEVENT_H

#include <cstddef>
#include<cstdlib>
#include <map>
#include <iostream>

#include "Object/CalorimeterHit.h"
#include "Object/McParticle.h"
#include "Object/ReconstructedParticle.h"
#include "Object/SimulatedHit.h"
#include "Utility/DStep.h"

class DEvent {
public:
    // Constructor
    DEvent() { Initialization(); };

    virtual ~DEvent() {
        Initialization();
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

    const MCParticleMap &getMCParticleCollection() const {
        return MCParticleCollection;
    }

    const RecParticleMap &getRecParticleCollection() const {
        return RecParticleCollection;
    }

    const SimulatedHitMap &getSimulatedHitCollection() const {
        return SimulatedHitCollection;
    }

    const CalorimeterHitMap &getCalorimeterHitCollection() const {
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

    // Initialization
    void Initialization();

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

    std::vector<std::string> *ListAllCollections();


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
    DStepMap StepCollection;

    // Event Physics Quantities
    MCParticleMap MCParticleCollection;
    RecParticleMap RecParticleCollection;
    SimulatedHitMap SimulatedHitCollection;
    CalorimeterHitMap CalorimeterHitCollection;
};

// Some inline
template<class T>
std::vector<std::string> *DEvent::ListCollections(const T &in) {
    auto tmp = new std::vector<std::string>;
    for (auto itr : in) tmp->emplace_back(itr.first);

    return tmp;
}

#endif //DSIMU_DEVENT_H

