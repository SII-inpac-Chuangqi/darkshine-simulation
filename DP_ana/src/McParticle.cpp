//
// Created by Zhang Yulei on 9/18/20.
//

#include "Object/McParticle.h"

McParticle::McParticle() {

}

McParticle::McParticle(const McParticle &rhs)  : DParticle(rhs) {
    *this = rhs;
}

McParticle::~McParticle() {

}

bool McParticle::operator==(const McParticle &rhs) const {
    return static_cast<const DParticle &>(*this) == static_cast<const DParticle &>(rhs) &&
           ERemain == rhs.ERemain &&
           RecParticles == rhs.RecParticles &&
           SimHits == rhs.SimHits;
}

bool McParticle::operator!=(const McParticle &rhs) const {
    return !(rhs == *this);
}

McParticle &McParticle::operator=(const McParticle &rhs) {
    if (&rhs == this) { return *this; }
    DParticle::operator=(rhs);
    ERemain = rhs.ERemain;
    RecParticles = rhs.RecParticles;
    SimHits = rhs.SimHits;
    return *this;
}

double McParticle::getERemain() const {
    return ERemain;
}

const RecParticleVec &McParticle::getRecParticles() const {
    return RecParticles;
}

const SimulatedHitVec &McParticle::getSimHits() const {
    return SimHits;
}

void McParticle::setERemain(double eRemain) {
    ERemain = eRemain;
}

void McParticle::setRecParticles(const RecParticleVec &recParticles) {
    RecParticles = recParticles;
}

void McParticle::setSimHits(const SimulatedHitVec &simHits) {
    SimHits = simHits;
}


