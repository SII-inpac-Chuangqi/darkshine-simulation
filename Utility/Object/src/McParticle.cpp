//
// Created by Zhang Yulei on 9/18/20.
//

#include "Object/McParticle.h"

McParticle::McParticle_b1_5() {
    Parents = nullptr;
    Children = nullptr;
    RecParticles = nullptr;
}

McParticle::McParticle_b1_5(const McParticle &rhs) : DParticle(rhs) {
    *this = rhs;
}

McParticle::~McParticle_b1_5() {
    if (Children && !Children->empty()) {
        Children->clear();
        Children->shrink_to_fit();
    }
    delete Children;
}

McParticle &McParticle::operator=(const McParticle &rhs) {
    if (&rhs == this) { return *this; }
    DParticle::operator=(rhs);
    ERemain = rhs.ERemain;
    RecParticles = rhs.RecParticles;
    //SimHits = rhs.SimHits;
    Parents = rhs.Parents;
    if (rhs.Children && Children) Children = rhs.Children;
    return *this;
}

double McParticle::getERemain() const {
    return ERemain;
}

void McParticle::setERemain(double eRemain) {
    ERemain = eRemain;
}

bool McParticle::operator==(const McParticle &rhs) const {
    return static_cast<const DParticle &>(*this) == static_cast<const DParticle &>(rhs) &&
           ERemain == rhs.ERemain &&
           Parents == rhs.Parents &&
           Children == rhs.Children &&
           RecParticles == rhs.RecParticles ;
           //SimHits == rhs.SimHits;
}

bool McParticle::operator!=(const McParticle &rhs) const {
    return !(rhs == *this);
}


