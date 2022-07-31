//
// Created by xuliang on 2022/7/22.
//

#ifndef DSIMU_SIDEHCAL_CONSTRUCT_H
#define DSIMU_SIDEHCAL_CONSTRUCT_H

#include "CALConstruct.hh"
#include "DP_simu/DetectorConstruction.hh"

#include "Control/Control.h"

#include "G4SystemOfUnits.hh"

class CALConstruct;

class SideHCAL_Construct {
public:
    SideHCAL_Construct() = default;
    ~SideHCAL_Construct();

    void DefineParameters();
    bool Build(G4LogicalVolume *World_LV, bool fCheckOverlaps);
    bool BuildSD();

    static constexpr double eps = 0;

private:
    G4String Name;

    G4Material *HCALRegion_Mat{};
    G4Material *HCAL_Mat{};
    G4Material *HCAL_Wrap_Mat{};
    G4Material *HCAL_Absorber_Mat{};
    std::vector<G4ThreeVector> Pos_SideHCALRegion;
    G4ThreeVector Size_SideHCALRegion;
    G4ThreeVector HCAL_Wrap_Size; // MODIFY
    G4ThreeVector SideHCAL_Size_Dir; // MODIFY
    //G4ThreeVector SideHCAL_Module_No; // MODIFY
    //G4double HCAL_Module_Gap = 0.5 * mm; //MODIFY

    std::vector<CALConstruct* > SideHCAL_vec;
    std::vector<std::vector<G4LogicalVolume *> > SideHCAL_SD_LV;
    std::vector<std::vector<G4LogicalVolume *> > SideHCAL_APD_SD_LV;

    std::vector<G4RotationMatrix*> v_RotMat;

    //G4LogicalVolume* HCAL_Module_LV;
    //G4LogicalVolume* HCAL_Layer_LV;

    /////////////////////////
    //  APD stuff
    /////////////////////////
    G4Material *APD_Mat{};
    G4Material *Glue_Mat{};
    G4ThreeVector APD_Size;
    G4ThreeVector Glue_Size;
};


#endif //DSIMU_SIDEHCAL_CONSTRUCT_H
