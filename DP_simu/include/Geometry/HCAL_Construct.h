//
// Created by Zhang Yulei on 11/20/20.
//

#ifndef DSIMU_HCAL_CONSTRUCT_H
#define DSIMU_HCAL_CONSTRUCT_H

#include "CALConstruct.hh"
#include "DP_simu/DetectorConstruction.hh"

#include "Control/Control.h"

#include "G4SystemOfUnits.hh"

class CALConstruct;

class HCAL_Construct {
public:
    HCAL_Construct() = default;

    ~HCAL_Construct();

    void DefineParameters();

    bool Build(G4LogicalVolume *World_LV, bool fCheckOverlaps);

    bool BuildSD();

    double eps = dControl->eps;
private:
    G4String Name;

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

    std::vector<CALConstruct* > HCAL_vec;
    std::vector<std::vector<G4LogicalVolume *> > HCAL_SD_LV;

    /////////////////////////
    //  APD stuff
    /////////////////////////
    G4Material *APD_Mat{};
    G4Material *Glue_Mat{};
    G4ThreeVector APD_Size;
    G4ThreeVector Glue_Size;
};


#endif //DSIMU_HCAL_CONSTRUCT_H
