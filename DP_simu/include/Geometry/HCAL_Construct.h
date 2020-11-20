//
// Created by Zhang Yulei on 11/20/20.
//

#ifndef DSIMU_HCAL_CONSTRUCT_H
#define DSIMU_HCAL_CONSTRUCT_H

#include "DP_simu/CALConstruct.hh"
#include "DP_simu/DetectorConstruction.hh"

class HCAL_Construct {
public:
    HCAL_Construct() = default;

    virtual ~HCAL_Construct() = default;

    const G4ThreeVector &getSizeHcalRegion() const {
        return Size_HCALRegion;
    }

    const G4ThreeVector &getPosHcalRegion() const {
        return Pos_HCALRegion;
    }

    void DefineParameters(const G4ThreeVector &Pos_ECALRegion, const G4ThreeVector &Size_ECALRegion);

    bool Build(int type, G4LogicalVolume *World_LV, RootManager *fRootMng, bool fCheckOverlaps);

    bool BuildSD(RootManager *fRootMng);

private:

    G4Material *HCALRegion_Mat{};
    G4Material *HCAL_Mat{};
    G4Material *HCAL_Wrap_Mat{};
    G4Material *HCAL_Absorber_Mat{};
    G4ThreeVector Size_HCALRegion;
    G4ThreeVector Pos_HCALRegion;
    G4ThreeVector HCAL_Wrap_Size;
    G4ThreeVector HCAL_Size_Dir;
    G4ThreeVector HCAL_Mod_No_Dir;
    G4ThreeVector HCAL_Module_No;
    G4double HCAL_Module_Gap{};
    G4double HCAL_Absorber_Thickness{};

    std::vector<G4LogicalVolume *> HCAL_SD_LV[9];

    /////////////////////////
    //  APD stuff
    /////////////////////////
    G4Material *APD_Mat{};
    G4Material *Glue_Mat{};
    G4ThreeVector APD_Size;
    G4ThreeVector Glue_Size;
};


#endif //DSIMU_HCAL_CONSTRUCT_H
