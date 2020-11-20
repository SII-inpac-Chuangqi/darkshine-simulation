//
// Created by Zhang Yulei on 11/20/20.
//

#include "Geometry/HCAL_Construct.h"

void HCAL_Construct::DefineParameters(const G4ThreeVector &Pos_ECALRegion, const G4ThreeVector &Size_ECALRegion) {
    HCAL_Absorber_Mat = G4Material::GetMaterial("G4_Fe");
    HCALRegion_Mat = G4Material::GetMaterial("CarbonFiber");
    HCAL_Mat = G4Material::GetMaterial("Polystyrene");
    HCAL_Wrap_Mat = G4Material::GetMaterial("G4_Al");

    HCAL_Absorber_Thickness = 3 * cm;
    HCAL_Wrap_Size = G4ThreeVector(0.3 * mm, 0.3 * mm, 0.3 * mm);
    HCAL_Size_Dir = G4ThreeVector(100 * cm + 19 * HCAL_Wrap_Size.x(), 5 * cm, 1 * cm);
    //HCAL_Mod_No_Dir = G4ThreeVector( 1, 20, 2 );
    HCAL_Mod_No_Dir = G4ThreeVector(1, 20, 120);
    HCAL_Module_No = G4ThreeVector(3, 3, 1);
    HCAL_Module_Gap = 0.5 * mm;

    Size_HCALRegion = G4ThreeVector(0, 0, 0);
    Size_HCALRegion.setX(
            HCAL_Module_No.x() * (HCAL_Size_Dir.x() + HCAL_Wrap_Size.x()) + HCAL_Module_Gap * (HCAL_Module_No.x() - 1));
    Size_HCALRegion.setY(HCAL_Module_No.y() * HCAL_Mod_No_Dir.y() * (HCAL_Size_Dir.y() + HCAL_Wrap_Size.y()) +
                         HCAL_Module_Gap * (HCAL_Module_No.y() - 1));
    Size_HCALRegion.setZ(
            HCAL_Mod_No_Dir.z() / 2 * (2 * (HCAL_Size_Dir.z() + HCAL_Wrap_Size.z()) + HCAL_Absorber_Thickness) +
            HCAL_Module_Gap * (HCAL_Module_No.z() - 1));

    Pos_HCALRegion = G4ThreeVector(0, 0,
                                   0.5 * Size_HCALRegion.z() + Pos_ECALRegion.z() + 0.5 * Size_ECALRegion.z());

    G4cout << " ==> HCAL starts from " << Pos_HCALRegion.z() - Size_HCALRegion.z() / 2 << G4endl;

    /////////////////////////
    //  APD
    /////////////////////////
    APD_Mat = G4Material::GetMaterial("G4_Si");
    APD_Size = G4ThreeVector(1 * cm, 1 * cm, 1 * mm);

    Glue_Mat = G4Material::GetMaterial("G4_W");
    Glue_Size = G4ThreeVector(1 * cm, 1 * cm, 0.1 * mm);
}

bool HCAL_Construct::Build(int type, G4LogicalVolume *World_LV, RootManager *fRootMng, bool fCheckOverlaps) {

    switch (type) {
        case -1:
            return false;
        default:
            break;
    }

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

            auto HCAL = new CALConstruct("HCAL_" + std::to_string((int) (ix + iy * HCAL_Module_No.x())), HCAL_LV, 0,
                                         true, true, fRootMng->GetOptical(), fCheckOverlaps);
            HCAL->SetSizeXYZ(HCAL_Size_Dir.x() / 2., HCAL_Size_Dir.y() / 2., HCAL_Size_Dir.z() / 2.);
            HCAL->SetWrapSizeXYZ(HCAL_Wrap_Size.x() / 2., HCAL_Wrap_Size.y() / 2., HCAL_Wrap_Size.z() / 2.);
            HCAL->SetCALMaterial(HCAL_Mat);
            HCAL->SetWrapMaterial(HCAL_Wrap_Mat);
            HCAL->SetVis(new G4VisAttributes(G4Colour(0.2, 0.37, 0.8)));
            HCAL->SetAPDSize(APD_Size, Glue_Size);
            HCAL->SetAPDMat(APD_Mat, Glue_Mat);
            HCAL->MatrixPlacementXYwithAbsorber(HCAL_Mod_No_Dir.x(), HCAL_Mod_No_Dir.y(), HCAL_Mod_No_Dir.z(),
                                                G4ThreeVector(wx, wy, 0), HCAL_Absorber_Thickness, HCAL_Absorber_Mat);

            HCAL_SD_LV[(int) (ix + iy * HCAL_Module_No.x())] = HCAL->GetCaloLVVector();
        }
    }

    return true;
}

bool HCAL_Construct::BuildSD(RootManager *fRootMng) {
    //DetectorSD *HCalSD[ (int)(HCAL_Module_No.x() * HCAL_Module_No.y() * HCAL_Module_No.z()) ];
    std::vector<DetectorSD* > HCalSD;
    for (int iy = 0; iy < HCAL_Module_No.y(); iy++) {
        for (int ix = 0; ix < HCAL_Module_No.x(); ix++) {
            int index = (int) (ix + iy * HCAL_Module_No.x());
            HCalSD.push_back(new DetectorSD(2, "HCAL_" + std::to_string(index), HCAL_Mod_No_Dir, fRootMng));
            G4SDManager::GetSDMpointer()->AddNewDetector(HCalSD[index]);
            for (auto itr_LV = HCAL_SD_LV[index].begin(); itr_LV != HCAL_SD_LV[index].end(); itr_LV++)
                (*itr_LV)->SetSensitiveDetector(HCalSD[index]);
        }
    }
    return false;
}
