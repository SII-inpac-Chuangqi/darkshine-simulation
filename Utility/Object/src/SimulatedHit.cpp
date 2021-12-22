//
// Created by Zhang Yulei on 9/18/20.
//

#include "Object/SimulatedHit.h"
#include "G4SystemOfUnits.hh"

#include <cassert>

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
    PhotonNumber = rhs.PhotonNumber;
    MCPContribution = rhs.MCPContribution;
    SimHits_Edep = rhs.SimHits_Edep;
    CaloHits = rhs.CaloHits;
    return *this;
}


SimulatedHit::~SimulatedHit() {
    SimHits_Edep.clear();
    SimHits_Edep.shrink_to_fit();

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
           PhotonNumber == rhs.PhotonNumber &&
           //PContribution_TrackID == rhs.PContribution_TrackID &&
           MCPContribution == rhs.MCPContribution &&
           CaloHits == rhs.CaloHits;
}

bool SimulatedHit::operator!=(const SimulatedHit &rhs) const {
    return !(rhs == *this);
}

// Add the 3 particles with the most energy deposition contributed to this hit
void SimulatedHit::addParticleContribution(const McParticle &mcp, double Edep, bool record_all) {
    auto pdg = mcp.getPdg();
    bool target_mcp = (mcp.getId() == 1
                       || abs(pdg) == 13   // Muon
                       || abs(pdg) == 14   // muon neutrino
                       || abs(pdg) == 12   // electron neutrino
                       || (abs(pdg) >= 100 && abs(pdg) <= 10000) // inclusive hadrons
    );

    // merge two same particle contribution
    for (unsigned i = 0; i < SimHits_Edep.size(); ++i) {
        if (MCPContribution.at(i).getId() == mcp.getId()) {
            SimHits_Edep.at(i) += Edep;
            return;
        }
    }

    // update MCP Contribution with the top 5 deposited energy particles
    // unless they are targeted particle
    if (MCPContribution.size() >= 5 && !record_all && !target_mcp) {
        assert(SimHits_Edep.size() == MCPContribution.size());
        for (unsigned i = 0; i < SimHits_Edep.size(); ++i) {
            if (SimHits_Edep.at(i) < Edep
                && !(MCPContribution.at(i).getId() == 1
                     || abs(MCPContribution.at(i).getPdg()) == 13   // Muon
                     || abs(MCPContribution.at(i).getPdg()) == 14   // muon neutrino
                     || abs(MCPContribution.at(i).getPdg()) == 12   // electron neutrino
                     || (abs(MCPContribution.at(i).getPdg()) >= 100 &&
                         abs(MCPContribution.at(i).getPdg()) <= 10000) // inclusive hadrons
            )) {
                MCPContribution.at(i) = McParticle(mcp);
                SimHits_Edep.at(i) = Edep;
                break;
            }
        }
    } else {
        MCPContribution.emplace_back(mcp);
        SimHits_Edep.push_back(Edep);
        assert(SimHits_Edep.size() == MCPContribution.size());
    }
}
