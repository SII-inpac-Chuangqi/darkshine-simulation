//
// Created by Zhang Yulei on 11/20/20.
//

#include "Geometry/HCAL_Construct.h"

void HCAL_Construct::DefineParameters() {
    Name = dControl->HCAL_Name;

    HCAL_Absorber_Mat = dControl->HCAL_Absorber_Mat;
    HCALRegion_Mat = dControl->HCALRegion_Mat;
    HCAL_Mat = dControl->HCAL_Mat;
    HCAL_Wrap_Mat = dControl->HCAL_Wrap_Mat;

    HCAL_Wrap_Size = dControl->HCAL_Wrap_Size;
    HCAL_Size_Dir = dControl->HCAL_Size_Dir;
    HCAL_Mod_No_Dir = dControl->HCAL_Mod_No_Dir;
    HCAL_Module_No = dControl->HCAL_Module_No;
    HCAL_Absorber_Thickness = dControl->HCAL_Absorber_Thickness;

    Size_HCALRegion = dControl->Size_HCALRegion;
    Pos_HCALRegion = dControl->Pos_HCALRegion;



    G4cout << " ==> HCAL starts from " << Pos_HCALRegion.z() - Size_HCALRegion.z() / 2 << G4endl;

    /////////////////////////
    //  APD
    /////////////////////////
    APD_Mat = dControl->APD_Mat;
    APD_Size = dControl->HCAL_APD_Size;

    Glue_Mat = dControl->Glue_Mat;
    Glue_Size = dControl->Glue_Size;

    ////////////////////////
    // Optical
    ////////////////////////
}

bool HCAL_Construct::Build(G4LogicalVolume *World_LV, bool fCheckOverlaps) {

    auto HCAL_Box = new G4Box("hcal", Size_HCALRegion.x() / 2, Size_HCALRegion.y() / 2, Size_HCALRegion.z() / 2);
    auto HCAL_LV = new G4LogicalVolume(HCAL_Box, HCALRegion_Mat, "HCAL", nullptr, nullptr, nullptr);
    new G4PVPlacement(nullptr, Pos_HCALRegion, HCAL_LV, "HCAL", World_LV, false, 0, fCheckOverlaps);
    HCAL_LV->SetVisAttributes(G4VisAttributes::GetInvisible());

    auto HCAL = new CALConstruct("HCAL", HCAL_LV, 0,
                                 true, true, dControl->if_optical, fCheckOverlaps);
    HCAL_vec.emplace_back(HCAL);
    HCAL->SetSizeXYZ(HCAL_Size_Dir.x() / 2., HCAL_Size_Dir.y() / 2., HCAL_Size_Dir.z() / 2.);
    HCAL->SetWrapSizeXYZ(HCAL_Wrap_Size.x() / 2., HCAL_Wrap_Size.y() / 2., HCAL_Wrap_Size.z() / 2.);
    HCAL->SetCaloHoleRadius(dControl->HCAL_CaloHoleRadius);
    HCAL->SetFiberRadius(dControl->HCAL_FiberRadius);
    HCAL->SetCALMaterial(HCAL_Mat);
    HCAL->SetFiberCladMaterial(dControl->HCAL_FiberClad_Mat);
    HCAL->SetFiberMaterial(dControl->HCAL_Fiber_Mat);
    HCAL->SetWrapMaterial(HCAL_Wrap_Mat);
    if (dControl->HCAL_Show_Cell) HCAL->SetVis(new G4VisAttributes(G4Colour(0.2, 0.37, 0.8)));
    HCAL->SetAPDSize(APD_Size, Glue_Size);
    HCAL->SetAPDMat(APD_Mat, Glue_Mat);

    // construct
    if (dControl->HCAL_Show_Cell) HCAL->SetAPDVis(new G4VisAttributes(G4Colour(0.5, 0.5, .0)));
    //HCAL->SetFiberCladVis(new G4VisAttributes(G4Colour(0.6,0.7,0.8)));
    //HCAL->SetFiberVis(new G4VisAttributes(G4Colour(0.4,0.3,0.2)));
    HCAL->CalWLSUnitConstruct();
    HCAL_Module_LV = HCAL->XYCrossingConstruct(dControl->HCAL_Cell_XY_N, dControl->HCAL_Cell_XY_N,
                                               HCAL->GetOutlineLV(),
                                               dControl->World_Mat,
                                               1, eps);
    if(!dControl->HCAL_Show_Cell)
        HCAL_Module_LV->SetVisAttributes(new G4VisAttributes(true,G4Colour(0, 0.5, 0.5)));
    HCAL_Layer_LV = HCAL->MatrixConstruct(dControl->HCAL_Module_No.x(), dControl->HCAL_Module_No.y(), dControl->HCAL_Module_No.z(),
                                          HCAL_Module_LV,
                                          HCALRegion_Mat,
                                          2,
                                          dControl->HCAL_Module_Gap,
                                          false);
    HCAL->LinearPlacementWithAbsorber(dControl->HCAL_Layer_N,
                                      dControl->HCAL_Absorber_Thickness_List,
                                      HCAL_Layer_LV,
                                      dControl->World_Mat,
                                      HCAL_Absorber_Mat,
                                      eps);

    HCAL_SD_LV.emplace_back(HCAL->GetCaloLVVector());
    HCAL_APD_SD_LV.emplace_back(HCAL->GetAPDLVVector());

    return true;
}

bool HCAL_Construct::BuildSD() {
    auto *HCalSD = new DetectorSD(nHCAL, Name + "_0", HCAL_Mod_No_Dir );
    auto *HCalAPDSD = new DetectorSD(nHCAL_APD, Name + "_APD_0", HCAL_Mod_No_Dir);
    G4SDManager::GetSDMpointer()->AddNewDetector(HCalSD);
    G4SDManager::GetSDMpointer()->AddNewDetector(HCalAPDSD);
    for (auto LV : HCAL_SD_LV.at(0)) {
        LV->SetSensitiveDetector(HCalSD);
    }
    for (auto LV: HCAL_APD_SD_LV.at(0)) {
        LV->SetSensitiveDetector(HCalAPDSD);
    }

    return false;
}

HCAL_Construct::~HCAL_Construct() {
    for (auto i : HCAL_vec) {
        delete i;
    }
    HCAL_vec.clear();
    HCAL_vec.shrink_to_fit();

    for (auto i : HCAL_SD_LV) {
        i.clear();
        i.shrink_to_fit();
    }
    HCAL_SD_LV.clear();
    HCAL_SD_LV.shrink_to_fit();

    HCAL_APD_SD_LV.clear();
    HCAL_APD_SD_LV.shrink_to_fit();

}
