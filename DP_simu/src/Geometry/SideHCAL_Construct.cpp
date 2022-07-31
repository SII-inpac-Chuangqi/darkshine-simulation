//
// Created by xuliang on 2022/7/22.
//

#include "Geometry/SideHCAL_Construct.h"

void SideHCAL_Construct::DefineParameters() {

    Name = dControl->SideHCAL_Name;

    HCAL_Absorber_Mat = dControl->HCAL_Absorber_Mat;
    HCALRegion_Mat = dControl->HCALRegion_Mat;
    HCAL_Mat = dControl->HCAL_Mat;
    HCAL_Wrap_Mat = dControl->HCAL_Wrap_Mat;

    HCAL_Wrap_Size = dControl->HCAL_Wrap_Size;
    SideHCAL_Size_Dir = dControl->SideHCAL_Size_Dir;

    Pos_SideHCALRegion = dControl->Pos_SideHCALRegion;
    Size_SideHCALRegion = dControl->Size_SideHCALRegion;

    G4cout << " ==> Side HCAL starts from " << Pos_SideHCALRegion.at(0).z() - Size_SideHCALRegion.z() / 2 << G4endl;

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

bool SideHCAL_Construct::Build(G4LogicalVolume *World_LV, bool fCheckOverlaps) {

    auto SideHCAL_Box = new G4Box("sidehcal",
                                  0.5 * Size_SideHCALRegion.z(),
                                  0.5 * Size_SideHCALRegion.y(),
                                  0.5 * Size_SideHCALRegion.x());
    auto SideHCAL_LV = new G4LogicalVolume(SideHCAL_Box, HCALRegion_Mat, "SideHCAL",
                                           nullptr, nullptr, nullptr);
#ifndef DEBUG
    SideHCAL_LV->SetVisAttributes(G4VisAttributes::GetInvisible());
#endif
    auto SideHCAL = new CALConstruct("SideHCAL", SideHCAL_LV, 0,
                                     true, true, dControl->if_optical, fCheckOverlaps);
    SideHCAL_vec.emplace_back(SideHCAL);
    SideHCAL->SetSizeXYZ(0.5 * SideHCAL_Size_Dir.x(),
                         0.5 * SideHCAL_Size_Dir.y(),
                         0.5 * SideHCAL_Size_Dir.z());
    SideHCAL->SetWrapSizeXYZ(0.5 * HCAL_Wrap_Size.x(),
                             0.5 * HCAL_Wrap_Size.y(),
                             0.5 * HCAL_Wrap_Size.z());
    SideHCAL->SetCaloHoleRadius(dControl->HCAL_CaloHoleRadius);
    SideHCAL->SetFiberRadius(dControl->HCAL_FiberRadius);
    SideHCAL->SetCALMaterial(HCAL_Mat);
    SideHCAL->SetFiberCladMaterial(dControl->HCAL_FiberClad_Mat);
    SideHCAL->SetFiberMaterial(dControl->HCAL_Fiber_Mat);
    SideHCAL->SetWrapMaterial(HCAL_Wrap_Mat);
//    if (dControl->HCAL_Show_Cell)
        SideHCAL->SetVis(new G4VisAttributes(G4Colour(0.2, 0.37, 0.8)));
    SideHCAL->SetAPDSize(APD_Size, Glue_Size);
    SideHCAL->SetAPDMat(APD_Mat, Glue_Mat);

    // construct
//    if (dControl->HCAL_Show_Cell)
        SideHCAL->SetAPDVis(new G4VisAttributes(G4Colour(0.5, 0.5, 0)));
    SideHCAL->CalWLSUnitConstruct();
    // rotation matrix
    auto rotY90X90 = new G4RotationMatrix();
    rotY90X90->rotateY(90 * degree);
    rotY90X90->rotateX(90 * degree);
    SideHCAL->SetHepRot(rotY90X90);
    SideHCAL->LinearPlacementWithAbsorber(dControl->SideHCAL_Layer_N,
                                          dControl->SideHCAL_Absorber_Thickness_List,
                                          SideHCAL->GetOutlineLV(),
                                          HCAL_Absorber_Mat,
                                          eps);
    SideHCAL_SD_LV.emplace_back(SideHCAL->GetCaloLVVector());
    SideHCAL_APD_SD_LV.emplace_back(SideHCAL->GetAPDLVVector());

    // create rotation matrix
    for (int i = 0; i < 4; i++) {
        auto rotMat = new G4RotationMatrix();
        rotMat->rotateZ(- i * 90 * degree);
        rotMat->rotateY(- 90 * degree);
        v_RotMat.emplace_back(rotMat);
    }
    // Side HCAL Placement
    for (int copyNo = 0; copyNo < 4; copyNo++) {
        new G4PVPlacement(v_RotMat.at(copyNo),Pos_SideHCALRegion.at(copyNo),
                          SideHCAL_LV,"SideHCAL",World_LV,
                          false, copyNo, fCheckOverlaps);
    }
    return true;
}

bool SideHCAL_Construct::BuildSD() {
    auto *SideHCALSD = new DetectorSD(nSideHCAL, Name, G4ThreeVector(1, 1, dControl->SideHCAL_Layer_N));
    auto *SideHCALAPDSD = new DetectorSD(nSideHCAL_APD, Name + "_APD", G4ThreeVector(1, 1, dControl->SideHCAL_Layer_N));
    G4SDManager::GetSDMpointer()->AddNewDetector(SideHCALSD);
    G4SDManager::GetSDMpointer()->AddNewDetector(SideHCALAPDSD);
    for (auto LV: SideHCAL_SD_LV.at(0)) {
        LV->SetSensitiveDetector(SideHCALSD);
    }
    for (auto LV: SideHCAL_APD_SD_LV.at(0)) {
        LV->SetSensitiveDetector(SideHCALAPDSD);
    }
    return true;
}

SideHCAL_Construct::~SideHCAL_Construct() {
    for (auto i: SideHCAL_vec) {
        delete i;
    }
    SideHCAL_vec.clear();
    SideHCAL_vec.shrink_to_fit();

    for (auto i : SideHCAL_SD_LV) {
        i.clear();
        i.shrink_to_fit();
    }
    SideHCAL_SD_LV.clear();
    SideHCAL_SD_LV.shrink_to_fit();

    SideHCAL_APD_SD_LV.clear();
    SideHCAL_APD_SD_LV.shrink_to_fit();
}