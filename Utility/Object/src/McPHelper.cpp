//
// Created by xuliang on 2022/5/4.
//

#include "Object/McPHelper.h"

McPHelper::McPHelper_b1_5(const McPHelper &rhs) : DHit(rhs) {
    *this = rhs;
}

McPHelper &McPHelper::operator=(const McPHelper &rhs) {
    if (&rhs == this) {return *this;}
    DHit::operator=(rhs);
    PDG = rhs.PDG;
    Px = rhs.Px;
    Py = rhs.Py;
    Pz = rhs.Pz;
    Mass = rhs.Mass;
    is_incoming = rhs.is_incoming;
    if (rhs.MCParticle) MCParticle = rhs.MCParticle;

    return *this;
}

bool McPHelper::operator==(const McPHelper_b1_5 &rhs) const {
    return static_cast<const DHit &>(*this) == static_cast<const DHit &>(rhs) &&
    PDG == rhs.PDG &&
    Px == rhs.Px &&
    Py == rhs.Py &&
    Pz == rhs.Pz &&
    Mass == rhs.Mass &&
    is_incoming == rhs.is_incoming &&
    MCParticle == rhs.MCParticle;
}

bool McPHelper::operator!=(const McPHelper_b1_5 &rhs) const {
    return !(rhs == *this);
}