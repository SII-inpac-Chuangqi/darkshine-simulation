//
// Created by Zhang Yulei on 9/18/20.
//

#include "Object/CalorimeterHit.h"

CalorimeterHit::CalorimeterHit() {

}

CalorimeterHit::~CalorimeterHit() {}

CalorimeterHit::CalorimeterHit(const CalorimeterHit &rhs) : DHit(rhs) {
    *this = rhs;
}

bool CalorimeterHit::operator==(const CalorimeterHit &rhs) const {
    return static_cast<const DHit &>(*this) == static_cast<const DHit &>(rhs) &&
           PContribution == rhs.PContribution &&
           SimHits == rhs.SimHits;
}

bool CalorimeterHit::operator!=(const CalorimeterHit &rhs) const {
    return !(rhs == *this);
}

CalorimeterHit &CalorimeterHit::operator=(const CalorimeterHit &rhs) {
    if (&rhs == this) { return *this; }
    DHit::operator=(rhs);
    PContribution = rhs.PContribution;
    SimHits = rhs.SimHits;
    return *this;
}

const RecParticleVec &CalorimeterHit::getPContribution() const {
    return PContribution;
}

const SimulatedHitVec &CalorimeterHit::getSimHits() const {
    return SimHits;
}

void CalorimeterHit::setPContribution(const RecParticleVec &pContribution) {
    PContribution = pContribution;
}

void CalorimeterHit::setSimHits(const SimulatedHitVec &simHits) {
    SimHits = simHits;
}




