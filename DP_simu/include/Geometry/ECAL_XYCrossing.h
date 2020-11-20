//
// Created by Zhang Yulei on 11/20/20.
//

#ifndef DSIMU_ECAL_XYCROSSING_H
#define DSIMU_ECAL_XYCROSSING_H

#include "DP_simu/CALConstruct.hh"
#include "DP_simu/DetectorConstruction.hh"

class ECAL_XYCrossing {
public:
    ECAL_XYCrossing() = default;

    virtual ~ECAL_XYCrossing() = default;

    const G4ThreeVector &getSizeEcalRegion() const {
        return Size_ECALRegion;
    }

    const G4ThreeVector &getPosEcalRegion() const {
        return Pos_ECALRegion;
    }

    const std::vector<G4LogicalVolume *> &getEcalCenterLv() const {
        return ECAL_Center_LV;
    }

    const std::vector<G4LogicalVolume *> *getEcalOuterLv() const {
        return ECAL_Outer_LV;
    }

    void DefineParameters(const G4ThreeVector &pos_RecRegion, const G4ThreeVector &size_RecRegion);

    bool Build(int type, G4LogicalVolume *World_LV, RootManager *fRootMng, bool fCheckOverlaps);

    bool BuildSD(RootManager *fRootMng);

    bool BuildBias(BOptrMultiParticleChangeCrossSection *bias);

private:
    G4Material *ECALRegion_Mat{};
    G4Material *ECAL_Center_Mat{};
    G4Material *ECAL_Outer_Mat{};
    G4Material *ECAL_Wrap_Mat{};
    G4ThreeVector Size_ECALRegion;
    G4ThreeVector Pos_ECALRegion;
    G4ThreeVector ECAL_Center_Wrap_Size;
    G4ThreeVector ECAL_Center_Size;
    G4ThreeVector ECAL_Center_Module_No;
    G4ThreeVector ECAL_Outer_Wrap_Size;
    G4ThreeVector ECAL_Outer_Size_Dir;
    G4ThreeVector ECAL_Outer_Mod_No_Dir;
    G4ThreeVector ECAL_Outer_Module_No;
    G4double ECAL_Module_Gap{};

    std::vector<G4LogicalVolume *> ECAL_Center_LV;
    std::vector<G4LogicalVolume *> ECAL_Outer_LV[4];

    /////////////////////////
    //  APD stuff
    /////////////////////////
    G4Material *APD_Mat{};
    G4Material *Glue_Mat{};
    G4ThreeVector APD_Size;
    G4ThreeVector Glue_Size;

};

#endif //DSIMU_ECAL_XYCROSSING_H
