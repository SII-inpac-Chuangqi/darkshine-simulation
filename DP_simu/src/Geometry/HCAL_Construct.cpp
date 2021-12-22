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
    HCAL_Module_Gap = dControl->HCAL_Module_Gap;
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
    //HCAL_LV->SetVisAttributes(G4VisAttributes::GetInvisible());

    /* Building Surrounding Calorimeter with Scintillator
     * Cell Size: 2*100*1 cm^3 or 100*2*1 cm^3
     * Totally 3*3*1 modules
     */
    for (int iy = 0; iy < HCAL_Module_No.y(); iy++) {
        for (int ix = 0; ix < HCAL_Module_No.x(); ix++) {
            double wx = -Size_HCALRegion.x() * 0.5 + (Size_HCALRegion.x() / HCAL_Module_No.x() * (0.5 + ix));
            double wy = -Size_HCALRegion.y() * 0.5 + (Size_HCALRegion.y() / HCAL_Module_No.y() * (0.5 + iy));

            auto HCAL = new CALConstruct(Name + "_" + std::to_string((int) (ix + iy * HCAL_Module_No.x())), HCAL_LV, 0,
                                         true, true, dControl->if_optical, fCheckOverlaps);
            HCAL_vec.push_back(HCAL);

            HCAL->SetSizeXYZ(HCAL_Size_Dir.x() / 2., HCAL_Size_Dir.y() / 2., HCAL_Size_Dir.z() / 2.);
            HCAL->SetWrapSizeXYZ(HCAL_Wrap_Size.x() / 2., HCAL_Wrap_Size.y() / 2., HCAL_Wrap_Size.z() / 2.);
            HCAL->SetCaloHoleRadius(dControl->HCAL_CaloHoleRadius);
            HCAL->SetFiberRadius(dControl->HCAL_FiberRadius);
            HCAL->SetCALMaterial(HCAL_Mat);
            HCAL->SetFiberCladMaterial(dControl->HCAL_FiberClad_Mat);
            HCAL->SetFiberMaterial(dControl->HCAL_Fiber_Mat);
            HCAL->SetWrapMaterial(HCAL_Wrap_Mat);
            HCAL->SetVis(new G4VisAttributes(G4Colour(0.2, 0.37, 0.8)));
            HCAL->SetAPDSize(APD_Size, Glue_Size);
            HCAL->SetAPDMat(APD_Mat, Glue_Mat);
            HCAL->MatrixPlacementXYwithAbsorber(HCAL_Mod_No_Dir.x(), HCAL_Mod_No_Dir.y(), HCAL_Mod_No_Dir.z(),
                                                G4ThreeVector(wx, wy, 0), HCAL_Absorber_Thickness, HCAL_Absorber_Mat);

            //HCAL_SD_LV[(int) (ix + iy * HCAL_Module_No.x())] = HCAL->GetCaloLVVector();
            HCAL_SD_LV.emplace_back(HCAL->GetCaloLVVector());
            HCAL_APD_SD_LV.emplace_back(HCAL->GetAPDLVVector());
        }
    }

    return true;
}

bool HCAL_Construct::BuildSD() {
    std::vector<DetectorSD *> HCalSD;
    std::vector<DetectorSD *> HCalAPDSD;
    for (int iy = 0; iy < HCAL_Module_No.y(); iy++) {
        for (int ix = 0; ix < HCAL_Module_No.x(); ix++) {
            int index = (int) (ix + iy * HCAL_Module_No.x());
            HCalSD.push_back(new DetectorSD(HCAL, Name + "_" + std::to_string(index), HCAL_Mod_No_Dir));
            HCalAPDSD.emplace_back(new DetectorSD(HCAL_APD, Name + "_APD_" + std::to_string(index), HCAL_Mod_No_Dir));
            G4SDManager::GetSDMpointer()->AddNewDetector(HCalSD[index]);
            G4SDManager::GetSDMpointer()->AddNewDetector(HCalAPDSD[index]);
            for (auto LV : HCAL_SD_LV[index])
                LV->SetSensitiveDetector(HCalSD[index]);
            for (auto LV : HCAL_APD_SD_LV[index])
                LV->SetSensitiveDetector(HCalAPDSD[index]);
        }
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
