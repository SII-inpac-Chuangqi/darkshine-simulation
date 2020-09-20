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

class DEvent
{
public:
    // Constructor
    DEvent() { Initialization(); };
    virtual ~DEvent() = default;

    // Operators

    // Get Methods
    int getRunId() const {
        return RunID;
    }

    int getEventId() const {
        return EventID;
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

    // Initialization
    void Initialization();

    // Register Collections
    DStepVec* RegisterStepCollection(const std::string& );
    MCParticleVec* RegisterMCParticleCollection(const std::string& );
    RecParticleVec* RegisterRecParticleCollection(const std::string& );
    SimulatedHitVec* RegisterSimulatedHitCollection(const std::string& );
    CalorimeterHitVec* RegisterCalorimeterHitCollection(const std::string& );

    // Delete Collections
    void DeleteCollection(const std::string&);

    // List all Registered Collections for template T
    template<class T> std::vector<std::string>* ListCollections(const T&);
    std::vector<std::string>* ListAllCollections();


protected:
    // run number
    int RunID{0};

    // event number
    int EventID{0};

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
std::vector<std::string>* DEvent::ListCollections(const T& in) {
    auto tmp = new std::vector<std::string>;
    for (auto itr : in) tmp->push_back(itr.first);

    return tmp;
}

#endif //DSIMU_DEVENT_H

