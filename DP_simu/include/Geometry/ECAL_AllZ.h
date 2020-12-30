//
// Created by Zhang Yulei on 11/20/20.
//

#ifndef DSIMU_ECAL_ALLZ_H
#define DSIMU_ECAL_ALLZ_H

#include "DP_simu/CALConstruct.hh"
#include "DP_simu/DetectorConstruction.hh"
#include "G4SystemOfUnits.hh"


class ECAL_AllZ {
public:
    ECAL_AllZ() = default;

    virtual ~ECAL_AllZ() = default;

    [[nodiscard]] const G4ThreeVector &getSizeEcalRegion() const {
        return Size_ECALRegion;
    }

    [[nodiscard]] const G4ThreeVector &getPosEcalRegion() const {
        return Pos_ECALRegion;
    }

    void DefineParameters(const G4ThreeVector &pos_RecRegion, const G4ThreeVector &size_RecRegion);

    bool Build(int type, G4LogicalVolume *World_LV, RootManager *fRootMng, bool fCheckOverlaps);

    bool BuildSD(RootManager *fRootMng);

    bool BuildBias(BOptrMultiParticleChangeCrossSection *bias);

    /// Setter

    void SetECALCenterWrapSize(const G4ThreeVector& in) { ECAL_Center_Wrap_Size = in; };

    void SetECALCenterSize(const G4ThreeVector& in) { ECAL_Center_Size = in; };
    
    void SetECALCenterModuleNo(const G4ThreeVector& in) { ECAL_Center_Module_No = in; };

private:
    G4String Name = "ECAL_Center";

    G4Material *ECALRegion_Mat{};
    G4Material *ECAL_Center_Mat{};
    G4Material *ECAL_Wrap_Mat{};
    G4ThreeVector Size_ECALRegion;
    G4ThreeVector Pos_ECALRegion;
    G4ThreeVector ECAL_Center_Wrap_Size = G4ThreeVector(0.3 * mm, 0.3 * mm, 0.3 * mm);
    G4ThreeVector ECAL_Center_Size = G4ThreeVector(2.5 * cm, 2.5 * cm, 2.0 * cm);
    G4ThreeVector ECAL_Center_Module_No = G4ThreeVector(20, 20, 18);
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
