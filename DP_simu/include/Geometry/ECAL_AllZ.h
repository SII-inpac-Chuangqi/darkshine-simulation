//
// Created by Zhang Yulei on 11/20/20.
//

#ifndef DSIMU_ECAL_ALLZ_H
#define DSIMU_ECAL_ALLZ_H

#include "DP_simu/CALConstruct.hh"
#include "DP_simu/DetectorConstruction.hh"

class ECAL_AllZ {
public:
    ECAL_AllZ() = default;

    virtual ~ECAL_AllZ() = default;

    const G4ThreeVector &getSizeEcalRegion() const {
        return Size_ECALRegion;
    }

    const G4ThreeVector &getPosEcalRegion() const {
        return Pos_ECALRegion;
    }

    void DefineParameters(const G4ThreeVector &pos_RecRegion, const G4ThreeVector &size_RecRegion);

    bool Build(int type, G4LogicalVolume *World_LV, RootManager *fRootMng, bool fCheckOverlaps);

    bool BuildSD(RootManager *fRootMng);

    bool BuildBias(BOptrMultiParticleChangeCrossSection *bias);

private:
    G4Material *ECALRegion_Mat{};
    G4Material *ECAL_Center_Mat{};
    G4Material *ECAL_Wrap_Mat{};
    G4ThreeVector Size_ECALRegion;
    G4ThreeVector Pos_ECALRegion;
    G4ThreeVector ECAL_Center_Wrap_Size;
    G4ThreeVector ECAL_Center_Size;
    G4ThreeVector ECAL_Center_Module_No;
    G4double ECAL_Module_Gap{};

    std::vector<G4LogicalVolume *> ECAL_Center_LV;

    /////////////////////////
    //  APD stuff
    /////////////////////////
    G4Material *APD_Mat{};
    G4Material *Glue_Mat{};
    G4ThreeVector APD_Size;
    G4ThreeVector Glue_Size;

};


#endif //DSIMU_ECAL_ALLZ_H
