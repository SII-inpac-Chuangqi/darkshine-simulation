//
// Created by Zhang Yulei on 11/20/20.
//

#include "Geometry/ECAL_XYCrossing.h"

bool ECAL_XYCrossing::Build(int type, G4LogicalVolume *World_LV, RootManager *fRootMng, bool fCheckOverlaps) {
    /*
     * Type:
     *    -1: do not build
     *     0: build everything
     *     1: build center
     *     2: build outer
     */
    bool build_ECAL_Center = false;
    bool build_ECAL_Outer = false;
    switch (type) {
        case -1:
            break;
        case 0:
            build_ECAL_Center = true;
            build_ECAL_Outer = true;
            break;
        case 1:
            build_ECAL_Center = true;
            break;
        case 2:
            build_ECAL_Outer = true;
            break;
        default:
            break;
    }

    auto ECAL_Box = new G4Box("ecal", Size_ECALRegion.x() / 2, Size_ECALRegion.y() / 2, Size_ECALRegion.z() / 2);
    auto ECal_LV = new G4LogicalVolume(ECAL_Box, ECALRegion_Mat, "ECAL", nullptr, nullptr, nullptr);
    new G4PVPlacement(nullptr, Pos_ECALRegion, ECal_LV, "ECAL", World_LV, false, 0, fCheckOverlaps);
    ECal_LV->SetVisAttributes(G4VisAttributes::GetInvisible());

    /* Building Center Calorimeter with LYSO
     * Cell Size: 1*1*36 cm^3
     * Module No: 6*6
     */
    if (build_ECAL_Center) {
        auto ECAL_Center = new CALConstruct("ECAL_Center", ECal_LV, 0, true, true, fRootMng->GetOptical(),
                                            fCheckOverlaps);
        ECAL_Center->SetSizeXYZ(ECAL_Center_Size.x() / 2., ECAL_Center_Size.y() / 2., ECAL_Center_Size.z() / 2.);
        ECAL_Center->SetWrapSizeXYZ(ECAL_Center_Wrap_Size.x() / 2., ECAL_Center_Wrap_Size.y() / 2.,
                                    ECAL_Center_Wrap_Size.z() / 2.);
        ECAL_Center->SetCALMaterial(ECAL_Center_Mat);
        ECAL_Center->SetWrapMaterial(ECAL_Wrap_Mat);
        ECAL_Center->SetVis(new G4VisAttributes(G4Colour(0.5, 0.5, .0)));
        ECAL_Center->SetAPDSize(APD_Size, Glue_Size);
        ECAL_Center->SetAPDMat(APD_Mat, Glue_Mat);
        ECAL_Center->MatrixPlacement(ECAL_Center_Module_No.x(), ECAL_Center_Module_No.y(), ECAL_Center_Module_No.z(),
                                     G4ThreeVector(0, 0, 0));

        ECAL_Center_LV = ECAL_Center->GetCaloLVVector();
    }

    /* Building Surrounding Calorimeter with PWO4
     * Cell Size: 1*20*1 cm^3 or 20*1*1 cm^3
     * Totally 4 modules
     */
    if (build_ECAL_Outer) {
        int nECAL_Outer;
        nECAL_Outer = ECAL_Outer_Module_No.x() * ECAL_Outer_Module_No.y() * ECAL_Outer_Module_No.z();
        for (int ip = 1; ip <= nECAL_Outer; ip++) {
            double w1 = pow(-1, (ip % 2)) * Size_ECALRegion.x() / 4.;
            double w2 = pow(-1, (ip - 1) / 2) * Size_ECALRegion.x() / 4.;
            auto ECAL_Outer = new CALConstruct("ECAL_Outer_" + std::to_string(ip), ECal_LV, 0, true, true,
                                               fRootMng->GetOptical(), fCheckOverlaps);
            ECAL_Outer->SetSizeXYZ(ECAL_Outer_Size_Dir.x() / 2., ECAL_Outer_Size_Dir.y() / 2.,
                                   ECAL_Outer_Size_Dir.z() / 2.);
            ECAL_Outer->SetWrapSizeXYZ(ECAL_Outer_Wrap_Size.x() / 2., ECAL_Outer_Wrap_Size.y() / 2.,
                                       ECAL_Outer_Wrap_Size.z() / 2.);
            ECAL_Outer->SetCALMaterial(ECAL_Outer_Mat);
            ECAL_Outer->SetWrapMaterial(ECAL_Wrap_Mat);
            ECAL_Outer->SetVis(new G4VisAttributes(G4Colour(0.4, 0.57, 0.6)));
            ECAL_Outer->SetAPDSize(APD_Size, Glue_Size);
            ECAL_Outer->SetAPDMat(APD_Mat, Glue_Mat);
            ECAL_Outer->MatrixPlacementXYRemoved(ECAL_Outer_Mod_No_Dir.x(), ECAL_Outer_Mod_No_Dir.y(),
                                                 ECAL_Outer_Mod_No_Dir.z(), G4ThreeVector(w1, w2, 0), 3, ip);

            auto tmp_LV = ECAL_Outer->GetCaloLVVector();
            //ECAL_Outer_LV.insert( ECAL_Outer_LV.end(), tmp_LV.begin(), tmp_LV.end() );
            ECAL_Outer_LV[ip - 1] = ECAL_Outer->GetCaloLVVector();
        }
    }

    return true;
}


void ECAL_XYCrossing::DefineParameters(const G4ThreeVector &Pos_RecRegion, const G4ThreeVector &Size_RecRegion) {

    ECALRegion_Mat = G4Material::GetMaterial("CarbonFiber");
    ECAL_Center_Mat = G4Material::GetMaterial("LYSO");
    ECAL_Outer_Mat = G4Material::GetMaterial("PWO4");
    ECAL_Wrap_Mat = G4Material::GetMaterial("G4_Al");

    ECAL_Center_Wrap_Size = G4ThreeVector(0.3 * mm, 0.3 * mm, 0.3 * mm);
    ECAL_Center_Size = G4ThreeVector(1 * cm, 1 * cm, 36 * cm + 35 * ECAL_Center_Wrap_Size.z());
    ECAL_Center_Module_No = G4ThreeVector(6, 6, 1);

    ECAL_Outer_Wrap_Size = ECAL_Center_Wrap_Size;
    ECAL_Outer_Size_Dir = G4ThreeVector(20 * cm + 19 * ECAL_Outer_Wrap_Size.x(), 1 * cm, 1 * cm);
    ECAL_Outer_Mod_No_Dir = G4ThreeVector(1, 20, 36);
    ECAL_Outer_Module_No = G4ThreeVector(2, 2, 1);
    ECAL_Module_Gap = 0.5 * mm;

    Size_ECALRegion = G4ThreeVector(0, 0, 0);
    Size_ECALRegion.setX(
            ECAL_Outer_Module_No.x() * (ECAL_Outer_Size_Dir.x() + ECAL_Outer_Wrap_Size.x()) + ECAL_Module_Gap);
    Size_ECALRegion.setY(ECAL_Outer_Module_No.y() * ECAL_Outer_Mod_No_Dir.y() *
                         (ECAL_Outer_Size_Dir.y() + ECAL_Outer_Wrap_Size.y()) + ECAL_Module_Gap);
    Size_ECALRegion.setZ(
            ECAL_Outer_Mod_No_Dir.z() * (ECAL_Outer_Size_Dir.z() + ECAL_Outer_Wrap_Size.z()) + ECAL_Module_Gap);

    Pos_ECALRegion = G4ThreeVector(0, 0,
                                   0.5 * Size_ECALRegion.z() + Pos_RecRegion.z() + 0.5 * Size_RecRegion.z() + 1.0 * mm);

    G4cout << " ==> ECAL starts from " << Pos_ECALRegion.z() - Size_ECALRegion.z() / 2 << G4endl;

    /////////////////////////
    //  APD
    /////////////////////////
    APD_Mat = G4Material::GetMaterial("G4_Si");
    APD_Size = G4ThreeVector(1 * cm, 1 * cm, 1 * mm);

    Glue_Mat = G4Material::GetMaterial("G4_W");
    Glue_Size = G4ThreeVector(1 * cm, 1 * cm, 0.1 * mm);

}

bool ECAL_XYCrossing::BuildSD(RootManager *fRootMng) {

    // ECAL Center SD
    auto *ECalSD = new DetectorSD(1, "ECAL_Center", ECAL_Center_Module_No, fRootMng);
    G4SDManager::GetSDMpointer()->AddNewDetector(ECalSD);
    for (auto &itr_LV : ECAL_Center_LV)
        itr_LV->SetSensitiveDetector(ECalSD);

    // ECAL Outer SD
    const int nECAL_Outer = ECAL_Outer_Module_No.x() * ECAL_Outer_Module_No.y() * ECAL_Outer_Module_No.z();
    DetectorSD *ECalOutSD[4];
    for (int i = 1; i <= nECAL_Outer; i++) {
        ECalOutSD[i - 1] = new DetectorSD(2, "ECAL_Outer_" + std::to_string(i), ECAL_Outer_Mod_No_Dir,
                                          fRootMng);
        G4SDManager::GetSDMpointer()->AddNewDetector(ECalOutSD[i - 1]);
        for (auto itr_LV = (ECAL_Outer_LV[i - 1]).begin(); itr_LV != (ECAL_Outer_LV[i - 1]).end(); itr_LV++)
            (*itr_LV)->SetSensitiveDetector(ECalOutSD[i - 1]);
    }

    return true;
}

bool ECAL_XYCrossing::BuildBias(BOptrMultiParticleChangeCrossSection *bias) {

    for (auto &itr_LV : ECAL_Center_LV)
        bias->AttachTo(itr_LV);

    for (int i = 1; i <= 4; i++) {
        for (auto &itr_LV : ECAL_Outer_LV[i - 1])
            bias->AttachTo(itr_LV);
    }

    return false;
}
