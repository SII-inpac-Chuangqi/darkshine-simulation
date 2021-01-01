//
// Created by Zhang Yulei on 11/20/20.
//

#include "Geometry/ECAL_AllZ.h"

#include "G4SDManager.hh"

void ECAL_AllZ::DefineParameters() {
    Name = dControl->ECAL_Name;

    ECALRegion_Mat = dControl->ECALRegion_Mat;
    ECAL_Center_Mat = dControl->ECAL_Center_Mat;
    ECAL_Wrap_Mat = dControl->ECAL_Wrap_Mat;
    Size_ECALRegion = dControl->Size_ECALRegion;
    Pos_ECALRegion = dControl->Pos_ECALRegion;

    G4cout << " ==> ECAL starts from " << Pos_ECALRegion.z() - Size_ECALRegion.z() / 2 << G4endl;

    /////////////////////////
    //  APD
    /////////////////////////
    APD_Mat = dControl->APD_Mat;
    APD_Size = dControl->APD_Size;

    Glue_Mat = dControl->Glue_Mat;
    Glue_Size = dControl->Glue_Size;
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
        auto ECAL_Center = new CALConstruct("ECAL_Center", ECal_LV, 0, true, true, dControl->if_optical,
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
    auto *ECalSD = new DetectorSD(1, Name, ECAL_Center_Module_No, fRootMng);
    G4SDManager::GetSDMpointer()->AddNewDetector(ECalSD);
    for (auto &itr_LV : ECAL_Center_LV)
        itr_LV->SetSensitiveDetector(ECalSD);

    return true;
}

bool ECAL_AllZ::BuildBias(BOptrMultiParticleChangeCrossSection *bias) {
    for (auto &itr_LV : ECAL_Center_LV)
        bias->AttachTo(itr_LV);

    return false;
}
