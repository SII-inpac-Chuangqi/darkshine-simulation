//
// Created by Zhang Yulei on 9/18/20.
//

#include "Object/McParticle.h"

McParticle::McParticle() {

}

McParticle::McParticle(const McParticle &rhs) : DParticle(rhs) {
    *this = rhs;
}

McParticle::~McParticle() {

}

McParticle &McParticle::operator=(const McParticle &rhs) {
    if (&rhs == this) { return *this; }
    DParticle::operator=(rhs);
    ERemain = rhs.ERemain;
    RecParticles = rhs.RecParticles;
    SimHits = rhs.SimHits;
    Parents = rhs.Parents;
    Children = rhs.Children;
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

const MCParticleVec &McParticle::getParents() const {
    return Parents;
}

const MCParticleVec &McParticle::getChildren() const {
    return Children;
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

void McParticle::setParents(const MCParticleVec &parents) {
    Parents = parents;
}

void McParticle::setChildren(const MCParticleVec &children) {
    Children = children;
}

bool McParticle::operator==(const McParticle &rhs) const {
    return static_cast<const DParticle &>(*this) == static_cast<const DParticle &>(rhs) &&
           ERemain == rhs.ERemain &&
           Parents == rhs.Parents &&
           Children == rhs.Children &&
           RecParticles == rhs.RecParticles &&
           SimHits == rhs.SimHits;
}

bool McParticle::operator!=(const McParticle &rhs) const {
    return !(rhs == *this);
}


