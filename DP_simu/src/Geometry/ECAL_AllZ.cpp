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
    ECAL_Center_Wrap_Size = dControl->ECAL_Center_Wrap_Size;
    ECAL_Center_Size = dControl->ECAL_Center_Size;
    ECAL_Center_Module_No = dControl->ECAL_Center_Module_No;

    G4cout << " ==> ECAL starts from " << Pos_ECALRegion.z() - Size_ECALRegion.z() / 2 << G4endl;

    /////////////////////////
    //  APD
    /////////////////////////
    APD_Mat = dControl->APD_Mat;
    ECAL_APD_Size = dControl->ECAL_APD_Size;

    Glue_Mat = dControl->Glue_Mat;
    Glue_Size = dControl->Glue_Size;
}

bool ECAL_AllZ::Build(int type, G4LogicalVolume *World_LV, bool fCheckOverlaps) {

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
    ECal_LV = new G4LogicalVolume(ECAL_Box, ECALRegion_Mat, "ECAL", nullptr, nullptr, nullptr);
    new G4PVPlacement(nullptr, Pos_ECALRegion, ECal_LV, "ECAL", World_LV, false, 0, fCheckOverlaps);
#ifndef DEBUG
    ECal_LV->SetVisAttributes(G4VisAttributes::GetInvisible());
#endif
    if (build_ECAL_Center) {
        auto ECAL_Center = new CALConstruct("ECAL", ECal_LV, 0, true, true, dControl->if_optical,
                                            fCheckOverlaps);
        ECAL_Center->SetSizeXYZ(ECAL_Center_Size.x() / 2., ECAL_Center_Size.y() / 2., ECAL_Center_Size.z() / 2.);
        ECAL_Center->SetWrapSizeXYZ(ECAL_Center_Wrap_Size.x() / 2., ECAL_Center_Wrap_Size.y() / 2.,
                                    ECAL_Center_Wrap_Size.z() / 2.);
        ECAL_Center->SetCALMaterial(ECAL_Center_Mat);
        ECAL_Center->SetWrapMaterial(ECAL_Wrap_Mat);
        ECAL_Center->SetVis(new G4VisAttributes(G4Colour(0.3, 0.5, 0.8)));
#ifdef DEBUG
        ECAL_Center->SetWrapVis(new G4VisAttributes(G4Colour(1, 1, 1)));
#endif
        ECAL_Center->SetAPDVis(new G4VisAttributes(G4Colour(0.5, 0.5, .0)));
        ECAL_Center->SetAPDSize(ECAL_APD_Size, Glue_Size);
        ECAL_Center->SetAPDMat(APD_Mat, Glue_Mat);
        ECAL_Center->CalUnit1Construct();
        ECal_Block_LV = ECAL_Center->MatrixConstruct(dControl->ECAL_Cell_No.x(),
                                                     dControl->ECAL_Cell_No.y(),
                                                     dControl->ECAL_Cell_No.z(),
                                                     ECAL_Center->GetOutlineLV(),
                                                     ECALRegion_Mat,
                                                     1,
                                                     G4ThreeVector(eps,eps,eps),
                                                     false);
        ECAL_Center->MatrixConstruct(dControl->ECAL_Block_No.x(),
                                     dControl->ECAL_Block_No.y(),
                                     dControl->ECAL_Block_No.z(),
                                     ECal_Block_LV,
                                     ECALRegion_Mat,
                                     2,
                                     G4ThreeVector(0.5 * eps, 0.5 * eps, 0.5 * eps),
                                     true);

        ECAL_Center_LV = ECAL_Center->GetCaloLVVector();
    }

    return true;
}

bool ECAL_AllZ::BuildSD() {
    // ECAL Center SD
    auto *ECalSD = new DetectorSD(nECAL, Name, ECAL_Center_Module_No);
    G4SDManager::GetSDMpointer()->AddNewDetector(ECalSD);
    for (auto &itr_LV : ECAL_Center_LV)
        itr_LV->SetSensitiveDetector(ECalSD);

    return true;
}

bool ECAL_AllZ::BuildBias(BOptrMultiParticleChangeCrossSection *bias) {
    for (auto &itr_LV : ECAL_Center_LV)
        bias->AttachTo(itr_LV);
    //bias->AttachTo(ECal_LV);

    return true;
}
