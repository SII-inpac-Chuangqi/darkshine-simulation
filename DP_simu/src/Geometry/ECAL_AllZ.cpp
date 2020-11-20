//
// Created by Zhang Yulei on 11/20/20.
//

#include "Geometry/ECAL_AllZ.h"

void ECAL_AllZ::DefineParameters(const G4ThreeVector &Pos_RecRegion, const G4ThreeVector &Size_RecRegion) {
    ECALRegion_Mat = G4Material::GetMaterial("CarbonFiber");
    //ECAL_Center_Mat = G4Material::GetMaterial("LYSO");
    ECAL_Center_Mat = G4Material::GetMaterial("PWO4"); // X0 = 0.92 cm
    ECAL_Wrap_Mat = G4Material::GetMaterial("G4_Al");

    ECAL_Center_Wrap_Size = G4ThreeVector(0.3 * mm, 0.3 * mm, 0.3 * mm);
    ECAL_Center_Size = G4ThreeVector(1 * cm, 1 * cm, 37 * cm);
    ECAL_Center_Module_No = G4ThreeVector(45, 45, 1);

    ECAL_Module_Gap = 0.5 * mm;

    Size_ECALRegion = G4ThreeVector(0, 0, 0);
    Size_ECALRegion.setX((ECAL_Center_Size.x() + ECAL_Center_Wrap_Size.x()) * ECAL_Center_Module_No.x());
    Size_ECALRegion.setY((ECAL_Center_Size.y() + ECAL_Center_Wrap_Size.y()) * ECAL_Center_Module_No.y());
    Size_ECALRegion.setZ((ECAL_Center_Size.z() + ECAL_Center_Wrap_Size.z()) * ECAL_Center_Module_No.z());

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

bool ECAL_AllZ::Build(int type, G4LogicalVolume *World_LV, RootManager *fRootMng, bool fCheckOverlaps) {

    bool build_ECAL_Center = false;
    switch (type) {
        case -1:
            break;
        case 0:
            build_ECAL_Center = true;
            break;
        default:
            break;
    }

    auto ECAL_Box = new G4Box("ecal", Size_ECALRegion.x() / 2, Size_ECALRegion.y() / 2, Size_ECALRegion.z() / 2);
    auto ECal_LV = new G4LogicalVolume(ECAL_Box, ECALRegion_Mat, "ECAL", nullptr, nullptr, nullptr);
    new G4PVPlacement(nullptr, Pos_ECALRegion, ECal_LV, "ECAL", World_LV, false, 0, fCheckOverlaps);
    ECal_LV->SetVisAttributes(G4VisAttributes::GetInvisible());


    if (build_ECAL_Center) {
        auto ECAL_Center = new CALConstruct("ECAL_Center", ECal_LV, 0, true, true, fRootMng->GetOptical(),
                                            fCheckOverlaps);
        ECAL_Center->SetSizeXYZ(ECAL_Center_Size.x() / 2., ECAL_Center_Size.y() / 2., ECAL_Center_Size.z() / 2.);
        ECAL_Center->SetWrapSizeXYZ(ECAL_Center_Wrap_Size.x() / 2., ECAL_Center_Wrap_Size.y() / 2.,
                                    ECAL_Center_Wrap_Size.z() / 2.);
        ECAL_Center->SetCALMaterial(ECAL_Center_Mat);
        ECAL_Center->SetWrapMaterial(ECAL_Wrap_Mat);
        ECAL_Center->SetVis(new G4VisAttributes(G4Colour(0.3, 0.5, 0.8)));
        ECAL_Center->SetAPDSize(APD_Size, Glue_Size);
        ECAL_Center->SetAPDMat(APD_Mat, Glue_Mat);
        ECAL_Center->MatrixPlacement(ECAL_Center_Module_No.x(), ECAL_Center_Module_No.y(), ECAL_Center_Module_No.z(),
                                     G4ThreeVector(0, 0, 0));

        ECAL_Center_LV = ECAL_Center->GetCaloLVVector();
    }

    return true;
}

bool ECAL_AllZ::BuildSD(RootManager *fRootMng) {
    // ECAL Center SD
    auto *ECalSD = new DetectorSD(1, "ECAL_Center", ECAL_Center_Module_No, fRootMng);
    G4SDManager::GetSDMpointer()->AddNewDetector(ECalSD);
    for (auto &itr_LV : ECAL_Center_LV)
        itr_LV->SetSensitiveDetector(ECalSD);

    return true;
}

bool ECAL_AllZ::BuildBias(BOptrMultiParticleChangeCrossSection *bias) {
    for (auto &itr_LV : ECAL_Center_LV)
        bias->AttachTo(itr_LV);

    return false;}
