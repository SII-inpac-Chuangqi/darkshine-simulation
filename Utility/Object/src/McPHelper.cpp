//
// Created by xuliang on 2022/5/4.
//

#include "Object/McPHelper.h"

McPHelper::McPHelper_b1_5(const McPHelper &rhs) : DParticle(rhs) {
    *this = rhs;
}

McPHelper &McPHelper::operator=(const McPHelper &rhs) {
    if (&rhs == this) {return *this;}
    DParticle::operator=(rhs);
    Detector = rhs.Detector;
    CellID = rhs.CellID;
    CellID_X = rhs.CellID_X;
    CellID_Y = rhs.CellID_Y;
    CellID_Z = rhs.CellID_Z;
    is_incoming = rhs.is_incoming;
    MCParticle = rhs.MCParticle;
}

bool McPHelper::operator==(const McPHelper_b1_5 &rhs) const {
    return static_cast<const DParticle &>(*this) == static_cast<const DParticle &>(rhs) &&
    Detector == rhs.Detector &&
    CellID == rhs.CellID &&
    CellID_X == rhs.CellID_X &&
    CellID_Y == rhs.CellID_Y &&
    CellID_Z == rhs.CellID_Z &&
    is_incoming == rhs.is_incoming &&
    MCParticle == rhs.MCParticle;
}

bool McPHelper::operator!=(const McPHelper_b1_5 &rhs) const {
    return !(rhs == *this);
}