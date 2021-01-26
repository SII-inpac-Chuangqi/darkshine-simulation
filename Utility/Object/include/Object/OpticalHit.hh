
/// \file OpticalHit.hh
/// \brief Definition of the OpticalHit class, per hit
//we now not use the opticalHit in DEvent
/*
#ifndef OpticalHit_h
#define OpticalHit_h 1

#include "TObject.h"

#include <vector>

/// Optical hit class
///
/// It defines the optical photon hit, 
/// And carry the creation information
/// - fDepth, fcosTheta, fE, fT, 

class OpticalHit : public TObject {
public:

    OpticalHit() = default;

    ~OpticalHit() override = default;

    bool isFIsDetected() const;

    void setFIsDetected(bool fIsDetected);

    bool isFIsLut() const;

    void setFIsLut(bool fIsLut);

    int getFCreationType() const;

    void setFCreationType(int fCreationType);

    int getFDetId() const;

    void setFDetId(int fDetId);

    int getFDetType() const;

    void setFDetType(int fDetType);

    int getFCellIdX() const;

    void setFCellIdX(int fCellIdX);

    int getFCellIdY() const;

    void setFCellIdY(int fCellIdY);

    int getFCellIdZ() const;

    void setFCellIdZ(int fCellIdZ);

    double getFPosX() const;

    void setFPosX(double fPosX);

    double getFPosY() const;

    void setFPosY(double fPosY);

    double getFPosZ() const;

    void setFPosZ(double fPosZ);

    double getFDepth() const;

    void setFDepth(double fDepth);

    double getFCosTheta() const;

    void setFCosTheta(double fCosTheta);

    double getFArrivalE() const;

    void setFArrivalE(double fArrivalE);

    double getFDeltaE() const;

    void setFDeltaE(double fDeltaE);

    double getFArrivalT() const;

    void setFArrivalT(double fArrivalT);

    double getFDeltaT() const;

    void setFDeltaT(double fDeltaT);

private:

    //Basic information
    bool fIsDetected{};
    bool fIsLUT{};
    int fCreationType{}; //Scint or Cerekov or others (0; 1,2,3,...)

    // Location information
    int fDetID{}; // 1-D No.
    int fDetType{}; // Optical module type or version, like ECAL_Z_v1, HCAL_XS_v2, ..., encoded to enum
    int fCellID_X{}; // 3-D No., or we may store real hit position
    int fCellID_Y{};
    int fCellID_Z{};
    double fPos_X{}; // real position
    double fPos_Y{};
    double fPos_Z{};

    // generation point information
    double fDepth{}; //taken from track aux data
    double fCosTheta{};

    // arrival information
    double fArrivalE{}; //not Edep, means arrival photon energy
    double fDeltaE{};
    double fArrivalT{};
    double fDeltaT{};


ClassDefOverride(OpticalHit,1);
};

#endif
*/