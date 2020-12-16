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

    // Setter

    void SetHCALWrapSize(G4ThreeVector in) { HCAL_Wrap_Size = in; };

    void SetHCALSizeDir(G4ThreeVector in) { HCAL_Size_Dir = in; };

    void SetHCALModNoDir(G4ThreeVector in) { HCAL_Mod_No_Dir = in; };

    void SetHCALModuleNo(G4ThreeVector in) { HCAL_Module_No = in; };

    void SetHCALModuleGap(G4double in) { HCAL_Module_Gap = in; };

    void SetHCALAbsorberThickness(G4double in) { HCAL_Absorber_Thickness = in; };

private:

    G4Material *HCALRegion_Mat{};
    G4Material *HCAL_Mat{};
    G4Material *HCAL_Wrap_Mat{};
    G4Material *HCAL_Absorber_Mat{};
    G4ThreeVector Size_HCALRegion;
    G4ThreeVector Pos_HCALRegion;
    G4ThreeVector HCAL_Wrap_Size = G4ThreeVector(0.3 * mm, 0.3 * mm, 0.3 * mm); // MODIFY
    G4ThreeVector HCAL_Size_Dir = G4ThreeVector(100 * cm + 19 * HCAL_Wrap_Size.x(), 5 * cm, 1 * cm); // MODIFY
    G4ThreeVector HCAL_Mod_No_Dir = G4ThreeVector(1, 20, 120); // MODIFY
    G4ThreeVector HCAL_Module_No = G4ThreeVector(3, 3, 1); // MODIFY
    G4double HCAL_Module_Gap = 0.5 * mm; //MODIFY
    G4double HCAL_Absorber_Thickness = 3 * cm; // MODIFY

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
