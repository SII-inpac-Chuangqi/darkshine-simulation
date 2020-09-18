//
// Created by Zhang Yulei on 9/18/20.
//

#include "Object/SimulatedHit.h"

SimulatedHit::SimulatedHit() {

}

SimulatedHit::~SimulatedHit() {}

SimulatedHit::SimulatedHit(const SimulatedHit &rhs) : DHit(rhs) {
    *this = rhs;
}

bool SimulatedHit::operator==(const SimulatedHit &rhs) const {
    return static_cast<const DHit &>(*this) == static_cast<const DHit &>(rhs) &&
           EdepEm == rhs.EdepEm &&
           EdepHad == rhs.EdepHad &&
           PContribution == rhs.PContribution &&
           CaloHits == rhs.CaloHits;
}

bool SimulatedHit::operator!=(const SimulatedHit &rhs) const {
    return !(rhs == *this);
}

SimulatedHit &SimulatedHit::operator=(const SimulatedHit &rhs) {
    if (&rhs == this) { return *this; }
    DHit::operator=(rhs);
    EdepEm = rhs.EdepEm;
    EdepHad = rhs.EdepHad;
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

const MCParticleVec &SimulatedHit::getPContribution() const {
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


