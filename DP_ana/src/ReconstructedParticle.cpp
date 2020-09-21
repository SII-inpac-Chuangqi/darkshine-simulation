//
// Created by Zhang Yulei on 9/18/20.
//

#include "Object/ReconstructedParticle.h"

ReconstructedParticle::ReconstructedParticle() {

}

ReconstructedParticle::ReconstructedParticle(const ReconstructedParticle &rhs) : DParticle(rhs) {
    *this = rhs;
}

ReconstructedParticle::~ReconstructedParticle() {}

bool ReconstructedParticle::operator==(const ReconstructedParticle &rhs) const {
    return static_cast<const DParticle &>(*this) == static_cast<const DParticle &>(rhs) &&
           Parents == rhs.Parents &&
           Children == rhs.Children &&
           MCParticles == rhs.MCParticles &&
           CaloHits == rhs.CaloHits;
}

bool ReconstructedParticle::operator!=(const ReconstructedParticle &rhs) const {
    return !(rhs == *this);
}

ReconstructedParticle &ReconstructedParticle::operator=(const ReconstructedParticle &rhs) {
    if (&rhs == this) { return *this; }
    DParticle::operator=(rhs);
    MCParticles = rhs.MCParticles;
    CaloHits = rhs.CaloHits;
    Parents = rhs.Parents;
    Children = rhs.Children;
    return *this;
}

const MCParticleVec &ReconstructedParticle::getMcParticles() const {
    return MCParticles;
}

const CalorimeterHitVec &ReconstructedParticle::getCaloHits() const {
    return CaloHits;
}

const RecParticleVec &ReconstructedParticle::getParents() const {
    return Parents;
}

const RecParticleVec &ReconstructedParticle::getChildren() const {
    return Children;
}

void ReconstructedParticle::setMcParticles(const MCParticleVec &mcParticles) {
    MCParticles = mcParticles;
}

void ReconstructedParticle::setCaloHits(const CalorimeterHitVec &caloHits) {
    CaloHits = caloHits;
}

void ReconstructedParticle::setParents(const RecParticleVec &parents) {
    Parents = parents;
}

void ReconstructedParticle::setChildren(const RecParticleVec &children) {
    Children = children;
}




