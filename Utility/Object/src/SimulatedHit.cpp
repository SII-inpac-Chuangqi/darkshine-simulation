//
// Created by Zhang Yulei on 9/18/20.
//

#include "Object/SimulatedHit.h"

#include <cassert>

ClassImp(SimulatedHit);

SimulatedHit::SimulatedHit() = default;

SimulatedHit::SimulatedHit(const SimulatedHit &rhs) : DHit(rhs) {
    *this = rhs;
}

SimulatedHit &SimulatedHit::operator=(const SimulatedHit &rhs) {
    if (&rhs == this) { return *this; }
    DHit::operator=(rhs);
    ELeak_Wrapper = rhs.ELeak_Wrapper;
    EdepEm = rhs.EdepEm;
    EdepHad = rhs.EdepHad;
    MCPContribution = rhs.MCPContribution;
    SimHits_Edep = rhs.SimHits_Edep;
    CaloHits = rhs.CaloHits;
    return *this;
}


SimulatedHit::~SimulatedHit() {
    SimHits_Edep.clear();
    SimHits_Edep.shrink_to_fit();

    for (auto p: MCPContribution)
        delete p;
    MCPContribution.clear();
    MCPContribution.shrink_to_fit();

    CaloHits.clear();
    CaloHits.shrink_to_fit();
}


double SimulatedHit::getEdepEm() const {
    return EdepEm;
}

double SimulatedHit::getEdepHad() const {
    return EdepHad;
}

void SimulatedHit::setEdepEm(double edepEm) {
    EdepEm = edepEm;
}

void SimulatedHit::setEdepHad(double edepHad) {
    EdepHad = edepHad;
}

void SimulatedHit::setELeakWrapper(double eLeakWrapper) {
    ELeak_Wrapper = eLeakWrapper;
}

double SimulatedHit::getELeakWrapper() const {
    return ELeak_Wrapper;
}

bool SimulatedHit::operator==(const SimulatedHit &rhs) const {
    return static_cast<const DHit &>(*this) == static_cast<const DHit &>(rhs) &&
           ELeak_Wrapper == rhs.ELeak_Wrapper &&
           EdepEm == rhs.EdepEm &&
           EdepHad == rhs.EdepHad &&
           //PContribution_TrackID == rhs.PContribution_TrackID &&
           MCPContribution == rhs.MCPContribution &&
           CaloHits == rhs.CaloHits;
}

bool SimulatedHit::operator!=(const SimulatedHit &rhs) const {
    return !(rhs == *this);
}

// Add the 3 particles with the most energy deposition contributed to this hit
void SimulatedHit::addParticleContribution(McParticle *mcp, double Edep) {
    if (MCPContribution.size() >= 3) {
        assert(SimHits_Edep.size() == MCPContribution.size());
        for (unsigned i = 0; i < SimHits_Edep.size(); ++i) {
            if (SimHits_Edep.at(i) < Edep) {
                MCPContribution.at(i) = mcp;
                SimHits_Edep.at(i) = Edep;
                break;
            }
        }
    } else {
        MCPContribution.push_back(mcp);
        SimHits_Edep.push_back(Edep);
        assert(SimHits_Edep.size() == MCPContribution.size());
    }
}
