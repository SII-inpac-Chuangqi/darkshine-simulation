//
// Created by Zhang Yulei on 9/18/20.
//

#include "Object/SimulatedHit.h"

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
    //PContribution_TrackID = rhs.PContribution_TrackID;
    PContribution = rhs.PContribution;
    CaloHits = rhs.CaloHits;
    return *this;
}

double SimulatedHit::getEdepEm() const {
    return EdepEm;
}

double SimulatedHit::getEdepHad() const {
    return EdepHad;
}

const MCParticleVec& SimulatedHit::getPContribution() const {
    return PContribution;
}

const CalorimeterHitVec &SimulatedHit::getCaloHits() const {
    return CaloHits;
}

void SimulatedHit::setEdepEm(double edepEm) {
    EdepEm = edepEm;
}

void SimulatedHit::setEdepHad(double edepHad) {
    EdepHad = edepHad;
}

void SimulatedHit::setPContribution(const MCParticleVec &pContribution) {
    PContribution = pContribution;
}

void SimulatedHit::setCaloHits(const CalorimeterHitVec &caloHits) {
    CaloHits = caloHits;
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
           PContribution == rhs.PContribution &&
           CaloHits == rhs.CaloHits;
}

bool SimulatedHit::operator!=(const SimulatedHit &rhs) const {
    return !(rhs == *this);
}
