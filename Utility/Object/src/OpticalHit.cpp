//
// Created by Zhang Yulei on 1/4/21.
//

#include "Object/OpticalHit.hh"

bool OpticalHit::isFIsDetected() const {
    return fIsDetected;
}

void OpticalHit::setFIsDetected(bool fIsdetected) {
    OpticalHit::fIsDetected = fIsdetected;
}

bool OpticalHit::isFIsLut() const {
    return fIsLUT;
}

void OpticalHit::setFIsLut(bool fIsLut) {
    fIsLUT = fIsLut;
}

int OpticalHit::getFCreationType() const {
    return fCreationType;
}

void OpticalHit::setFCreationType(int fCreationtype) {
    OpticalHit::fCreationType = fCreationtype;
}

int OpticalHit::getFDetId() const {
    return fDetID;
}

void OpticalHit::setFDetId(int fDetId) {
    fDetID = fDetId;
}

int OpticalHit::getFDetType() const {
    return fDetType;
}

void OpticalHit::setFDetType(int fDettype) {
    OpticalHit::fDetType = fDettype;
}

int OpticalHit::getFCellIdX() const {
    return fCellID_X;
}

void OpticalHit::setFCellIdX(int fCellIdX) {
    fCellID_X = fCellIdX;
}

int OpticalHit::getFCellIdY() const {
    return fCellID_Y;
}

void OpticalHit::setFCellIdY(int fCellIdY) {
    fCellID_Y = fCellIdY;
}

int OpticalHit::getFCellIdZ() const {
    return fCellID_Z;
}

void OpticalHit::setFCellIdZ(int fCellIdZ) {
    fCellID_Z = fCellIdZ;
}

double OpticalHit::getFPosX() const {
    return fPos_X;
}

void OpticalHit::setFPosX(double fPosX) {
    fPos_X = fPosX;
}

double OpticalHit::getFPosY() const {
    return fPos_Y;
}

void OpticalHit::setFPosY(double fPosY) {
    fPos_Y = fPosY;
}

double OpticalHit::getFPosZ() const {
    return fPos_Z;
}

void OpticalHit::setFPosZ(double fPosZ) {
    fPos_Z = fPosZ;
}

double OpticalHit::getFDepth() const {
    return fDepth;
}

void OpticalHit::setFDepth(double fdepth) {
    OpticalHit::fDepth = fdepth;
}

double OpticalHit::getFCosTheta() const {
    return fCosTheta;
}

void OpticalHit::setFCosTheta(double fcosTheta) {
    OpticalHit::fCosTheta = fcosTheta;
}

double OpticalHit::getFArrivalE() const {
    return fArrivalE;
}

void OpticalHit::setFArrivalE(double farrivalE) {
    OpticalHit::fArrivalE = farrivalE;
}

double OpticalHit::getFDeltaE() const {
    return fDeltaE;
}

void OpticalHit::setFDeltaE(double fdeltaE) {
    OpticalHit::fDeltaE = fdeltaE;
}

double OpticalHit::getFArrivalT() const {
    return fArrivalT;
}

void OpticalHit::setFArrivalT(double farrivalT) {
    OpticalHit::fArrivalT = farrivalT;
}

double OpticalHit::getFDeltaT() const {
    return fDeltaT;
}

void OpticalHit::setFDeltaT(double fdeltaT) {
    OpticalHit::fDeltaT = fdeltaT;
}
