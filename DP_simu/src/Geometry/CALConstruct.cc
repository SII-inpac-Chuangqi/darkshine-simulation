#include "Geometry/CALConstruct.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4ios.hh"
#include <iterator>
#include <utility>
#include <cassert>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CALConstruct::CALConstruct(const G4String &CALName,
                           G4LogicalVolume *MotherVolume,
                           G4int CopyNo,
                           G4bool Type,
                           G4bool Wrap,
                           G4bool Optical,  /* if simulate optical photon */
                           G4bool CheckOverlap) {
    fCALName = std::move(CALName);
    fMotherVolume = MotherVolume;
    fCopyNo = CopyNo;
    fType = Type;
    fWrap = Wrap;
    fOptical = Optical;
    fCheckOverlap = CheckOverlap;

    CaloXHalfLength = 0.;
    CaloYHalfLength = 0.;
    CaloZHalfLength = 0.;
    UnitPosX = 0.;
    UnitPosY = 0.;
    UnitPosZ = 0.;
    WrapXHalfLength = 0.;
    WrapYHalfLength = 0.;
    WrapZHalfLength = 0.;

    fCALSD = nullptr;
    fCALWrapSD = nullptr;
    fVis = nullptr;
    fWrapVis = nullptr;
    fCALMaterial = nullptr;
    fWrapMaterial = nullptr;

    ifAbsorber = false;
    fRecordLV = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CALConstruct::CALConstruct(const CALConstruct &in) {
    fCALName = in.fCALName;
    fMotherVolume = in.fMotherVolume;
    fCopyNo = in.fCopyNo;
    fType = in.fType;
    fWrap = in.fWrap;
    fOptical = in.fOptical;
    fCheckOverlap = in.fCheckOverlap;
    CaloXHalfLength = in.CaloXHalfLength;
    CaloYHalfLength = in.CaloYHalfLength;
    CaloZHalfLength = in.CaloZHalfLength;
    UnitPosX = in.UnitPosX;
    UnitPosY = in.UnitPosY;
    UnitPosZ = in.UnitPosZ;
    WrapXHalfLength = in.WrapXHalfLength;
    WrapYHalfLength = in.WrapYHalfLength;
    WrapZHalfLength = in.WrapZHalfLength;
    fCALMaterial = in.fCALMaterial;
    fWrapMaterial = in.fWrapMaterial;
    fCALSD = in.fCALSD;
    fCALWrapSD = in.fCALWrapSD;
    fVis = in.fVis;
    fWrapVis = in.fWrapVis;
    fCaloLVVector = in.fCaloLVVector;
    fWrapLVVector = in.fWrapLVVector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
CALConstruct::~CALConstruct() {
    delete fVis;
    delete fAPDWLV;
    delete fWrapLV;
    delete fCaloLV;
    delete fAbsLV;
    delete fCALSD;
    delete fCALWrapSD;
    delete fWrapVis;

    fCaloLVVector.clear();
    fCaloLVVector.shrink_to_fit();

    fWrapLVVector.clear();
    fWrapLVVector.shrink_to_fit();

    for (auto pv : PVVector)
        delete pv;
    PVVector.clear();
    PVVector.shrink_to_fit();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CALConstruct::CalZUnitVolume() {
    if (!CaloXHalfLength || !CaloYHalfLength || !CaloZHalfLength) {
        G4cout << fCALName << " Construction Error: at least size of one dimension is zero." << G4endl;
        return;
    }
    /// construct logical volume
    // Wrap
    auto WrapBox = new G4Box(fCALName + "_WrapBox",
                             CaloXHalfLength + WrapXHalfLength,
                             CaloYHalfLength + WrapYHalfLength,
                             CaloZHalfLength + WrapZHalfLength + APDZHalfLength);
    auto WrapLV = new G4LogicalVolume(WrapBox, fWrapMaterial, fCALName + "_LVW",
                                      nullptr, nullptr, nullptr);
    fWrapLV = WrapLV;
    if (fWrapVis) {
        fWrapVis->SetVisibility(true);
        WrapLV->SetVisAttributes(fWrapVis);
    } else WrapLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    if (fCALWrapSD) WrapLV->SetSensitiveDetector(fCALWrapSD);

    // Crystal
    auto Calo = new G4Box(fCALName + "_Box", CaloXHalfLength, CaloYHalfLength, CaloZHalfLength);
    auto CaloLV = new G4LogicalVolume(Calo, fCALMaterial, fCALName + "_LV",
                                      nullptr, nullptr, nullptr);
    fCaloLV = CaloLV;
    if (fOptical) new G4LogicalSkinSurface("WrapSkinSurface", fCaloLV, dControl->Wrap_Surface);

    fCaloLVVector.push_back(CaloLV);
    if (fVis) {
        fVis->SetVisibility(true);
        CaloLV->SetVisAttributes(fVis);
    } else CaloLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    if (fCALSD) CaloLV->SetSensitiveDetector(fCALSD);

    // APD
    auto APDBox = new G4Box(fCALName + "_APDWorld_Box", ADPwXHalfLength, APDwYHalfLength, APDwZHalfLength);
    auto APDLV = new G4LogicalVolume(APDBox, G4Material::GetMaterial("vacuum"), fCALName,
                                     nullptr, nullptr, nullptr);
    fAPDWLV = APDLV;

    auto *wVis = new G4VisAttributes(G4Colour(0.5, 0.5, .0));
    wVis->SetVisibility(true);
    APDLV->SetVisAttributes(wVis);

    /// construct physical volume
    // place crystal
    auto CaloPV = new G4PVPlacement(nullptr,G4ThreeVector(0, 0, - APDwZHalfLength),
                                    fCaloLV, fCALName + "_PV", fWrapLV,
                                    false, fCopyNo, fCheckOverlap);
    PVVector.push_back(CaloPV);
    // place APD
    auto APDPV = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, CaloZHalfLength),
                                   fAPDWLV, fCALName + "_APDWorld_PV", fWrapLV,
                                   false, fCopyNo, fCheckOverlap);
    PVVector.push_back(APDPV);
    if (fOptical) new G4LogicalBorderSurface(fCALName + "APDGlueSurface", CaloPV, APDPV, dControl->APD_Surface);

}

void CALConstruct::CalXUnitVolume() {

}

void CALConstruct::AbsorberUnitVolume() {

}

void CALConstruct::ConstructLV() {
    // check consistency
    if (!CaloXHalfLength || !CaloYHalfLength || !CaloZHalfLength) {
        G4cout << fCALName << " Construction Error: at least size of one dimension is zero." << G4endl;
        return;
    }

    // Core Detector Region
    auto fName = ifAbsorber ? fCALName + "Abs" : fCALName;
    auto box = new G4Box(fName + "_Box", CaloXHalfLength, CaloYHalfLength, CaloZHalfLength);
    auto box_Large = new G4Box(fName + "_Box", CaloXHalfLength, CaloYHalfLength, CaloZHalfLength);

    auto boxLV = new G4LogicalVolume(box, fCALMaterial, fName + "_LV", nullptr, nullptr, nullptr);
    if (ifAbsorber) fAbsLV = boxLV;
    else fCaloLV = boxLV;

    if (fRecordLV) fCaloLVVector.push_back(boxLV);
    if (fVis) {
        fVis->SetVisibility(true);
        boxLV->SetVisAttributes(fVis);
    } else boxLV->SetVisAttributes(G4VisAttributes::GetInvisible());
    if (fCALSD) boxLV->SetSensitiveDetector(fCALSD);

    // Wrapper
    if (fWrap) {
        auto boxL = new G4Box(fCALName + "_BoxL", CaloXHalfLength + WrapXHalfLength,
                              CaloYHalfLength + WrapYHalfLength,
                              CaloZHalfLength + WrapZHalfLength);
        auto WrapBox = new G4SubtractionSolid(fCALName + "_BoxW", boxL, box_Large);
        auto WrapLV = new G4LogicalVolume(WrapBox, fWrapMaterial, fCALName + "_LVW", nullptr, nullptr, nullptr);
        fWrapLV = WrapLV;

        if (fWrapVis) {
            fWrapVis->SetVisibility(true);
            WrapLV->SetVisAttributes(fWrapVis);
        } else WrapLV->SetVisAttributes(G4VisAttributes::GetInvisible());

        if (fCALWrapSD) WrapLV->SetSensitiveDetector(fCALWrapSD);
    }

    // APD
    if (fWrap && fOptical) {
        //ADPwXHalfLength -= eps;
        //APDwYHalfLength -= eps;
        //APDwZHalfLength -= eps;

        auto abox = new G4Box(fCALName + "_APDWorld_Box", ADPwXHalfLength, APDwYHalfLength, APDwZHalfLength);
        auto aboxLV = new G4LogicalVolume(abox, G4Material::GetMaterial("vacuum"), fCALName + "_APDWorld_LV", nullptr,
                                          nullptr, nullptr);
        fAPDWLV = aboxLV;

        auto *wVis = new G4VisAttributes(G4Colour(0.5, 0.5, .0));
        wVis->SetVisibility(true);
        aboxLV->SetVisAttributes(wVis);
    }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Volume relationship:
//   0 Wrap
//   ├-1 box (Crystal)
//   └-2 abox (APD)
//         ->|       |<-CaloZHalfLength + APDZHalfLength + WrapZHalfLength
// ->| |<-   |   ->| |<-WrapZHalfLength
//   ┌-------|-------┐
//   |0┌-----|---┐   |
//   | | 1   |   ├-┐ |
//   | |    ┌╋   |2| |
//   | |  ->||<- ├┬┘ |
//   | └----|----┘|  |
//   └------|----||--┘
//          |  ->||<-APDwZHalfLength
//        ->|    |<-CaloZHalfLength
//
void CALConstruct::CalZUnitPlacement(G4LogicalVolume *wrapLV, G4double z_angle) {

    if (wrapLV == nullptr) {
        G4cerr << "warpLV is empty for " << fCALName << " " << fCopyNo << G4endl;
        return;
    }

    // placement of calorimeter unit
    auto Pos = G4ThreeVector(UnitPosX, UnitPosY, UnitPosZ);

    // rotation matrix
    HepRot = new G4RotationMatrix();
    HepRot->rotateZ(z_angle);
    G4RotationMatrix *fRotate = (z_angle == 0) ? nullptr : HepRot;

    // place wrap
    auto WrapPV = new G4PVPlacement(fRotate, Pos, wrapLV, fCALName + "_PVW", fMotherVolume,
                                    false, fCopyNo, fCheckOverlap);
    PVVector.push_back(WrapPV);
}

G4ThreeVector CALConstruct::CalXUnitPlacement(G4LogicalVolume *boxLV, G4LogicalVolume *WrapLV, G4LogicalVolume *aboxLV,
                                              double z_angle) {

}

G4ThreeVector CALConstruct::AbsorberUnitPlacement() {

}

G4ThreeVector
CALConstruct::Construct(G4LogicalVolume *boxLV, G4LogicalVolume *WrapLV, G4LogicalVolume *aboxLV,
                        double z_angle) {
    auto TotalSize = G4ThreeVector(0, 0, 0);

    // Placement of Calorimeter
    auto pos = G4ThreeVector(UnitPosX, UnitPosY, UnitPosZ);

    // Calculate Rotation Matrix
    HepRot = new G4RotationMatrix();
    HepRot->rotateZ(z_angle);


    G4RotationMatrix *fRotate = (z_angle == 0) ? nullptr : HepRot;

    auto fName = ifAbsorber ? fCALName + "Abs" : fCALName;
    if (boxLV == nullptr) {
        G4cerr << "boxLV is empty for " << fName << " " << fCopyNo << G4endl;
        return G4ThreeVector();
    }
    auto boxPV = new G4PVPlacement(fRotate, pos, boxLV, fName + "_PV", fMotherVolume, false, fCopyNo, fCheckOverlap);
    PVVector.push_back(boxPV);

    TotalSize = G4ThreeVector(CaloXHalfLength, CaloYHalfLength, CaloZHalfLength);

    // Placement of Wrap
    if (fWrap) {
        if (WrapLV == nullptr) {
            G4cerr << "WarpLV is empty for " << fCALName << " " << fCopyNo << G4endl;
            return G4ThreeVector();
        }
        auto WrapPV = new G4PVPlacement(fRotate, pos, WrapLV, fCALName + "_PVW", fMotherVolume, false, fCopyNo,
                                        fCheckOverlap);
        PVVector.push_back(WrapPV);

        TotalSize = G4ThreeVector(CaloXHalfLength + WrapXHalfLength, CaloYHalfLength + WrapYHalfLength, CaloZHalfLength + WrapZHalfLength);

        if (fOptical) {
            /* Set Optical Porperties for boundary surface */
            //new G4LogicalSkinSurface( fCALName+"WrapSurface", WrapLV, WrapSurface );
            new G4LogicalBorderSurface(fCALName + "WrapSurface", boxPV, WrapPV, dControl->Wrap_Surface);

            /* Placement of APD */
            if (aboxLV == nullptr) {
                G4cerr << "APDboxLV is empty for " << fCALName << " " << fCopyNo << G4endl;
                return G4ThreeVector();
            }
            auto APDPV = new G4PVPlacement(nullptr, G4ThreeVector(APDPosX, APDPosY, APDPosZ), aboxLV,
                                           fCALName + "_APDWorld_PV", boxLV,
                                           false, fCopyNo, fCheckOverlap);

            PVVector.push_back(APDPV);
        }

    }

    return TotalSize;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector CALConstruct::MatrixPlacement(G4int xNo, G4int yNo, G4int zNo, const G4ThreeVector &CentrePos) {
    auto TotalHalfSize = G4ThreeVector(0, 0, 0);
    // check consistency
    if (!xNo || !yNo || !zNo) {
        G4cout << fCALName << " Construction Error: at least one of the matrix element is zero." << G4endl;
        return TotalHalfSize;
    }

    ADPwXHalfLength = CaloXHalfLength;
    APDwYHalfLength = CaloYHalfLength;
    APDwZHalfLength = APDZHalfLength + GlueZHalfLength;
    //APDwZHalfLength = CaloZHalfLength;

    //fWrapVis = new G4VisAttributes(G4Colour(0.5, 0.23, 0.89));

    CalZUnitVolume();

    auto UnitBox = dynamic_cast<G4Box*>(fWrapLV->GetSolid());

    TotalHalfSize = G4ThreeVector(xNo * UnitBox->GetXHalfLength(),
                              yNo * UnitBox->GetYHalfLength(),
                              zNo * UnitBox->GetZHalfLength());

    for (int k = 0; k < zNo; k++) {
        for (int j = 0; j < yNo; j++) {
            for (int i = 0; i < xNo; i++) {
                UnitPosX = -1. * TotalHalfSize.x() + (2 * i + 1) * UnitBox->GetXHalfLength() + CentrePos.x();
                UnitPosY = -1. * TotalHalfSize.y() + (2 * j + 1) * UnitBox->GetYHalfLength() + CentrePos.y();
                UnitPosZ = -1. * TotalHalfSize.z() + (2 * k + 1) * UnitBox->GetZHalfLength() + CentrePos.z();

                CalZUnitPlacement(fWrapLV);

                fCopyNo++;
            }
        }
    }

    return TotalHalfSize;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//void
//CALConstruct::MatrixPlacementXYRemoved(G4int xNo, G4int yNo, G4int zNo, const G4ThreeVector &CentrePos, G4int NoRemoved,
//                                       G4int type) {
//    /* Type:
//     * 1) Left Bottom Corner
//     * 4) Right Top Corner
//     * 2) Right Bottom Corner
//     * 3) Left Top Corner
//     */
//    auto idx = xNo;
//    auto idy = yNo;
//    auto idz = zNo;
//
//    auto iSizeX = CaloXHalfLength;
//    auto iSizeY = CaloYHalfLength;
//
//    auto TotalSize = G4ThreeVector(xNo * (CaloXHalfLength + fWrap * WrapXHalfLength),
//                                   yNo * (CaloYHalfLength + fWrap * WrapYHalfLength),
//                                   zNo * (CaloZHalfLength + fWrap * WrapZHalfLength));
//
//    for (int k = 0; k < idz; k++) {
//        // along x direction
//        if (k % 2 == 0) {
//            idx = xNo;
//            idy = yNo;
//            CaloXHalfLength = iSizeX;
//            CaloYHalfLength = iSizeY;
//        }
//        // along y direction
//        if (k % 2 == 1) {
//            idx = yNo;
//            idy = xNo;
//            CaloXHalfLength = iSizeY;
//            CaloYHalfLength = iSizeX;
//        }
//        for (int j = 0; j < idy; j++) {
//            for (int i = 0; i < idx; i++) {
//                auto tmpX = ((k % 2 == 0) ? iSizeX : iSizeY);
//                auto tmpY = ((k % 2 == 1) ? iSizeX : iSizeY);
//
//
//                UnitPosX = -1. * TotalSize.x() + (2 * i + 1) * (tmpX + fWrap * WrapXHalfLength) + CentrePos.x();
//                UnitPosY = -1. * TotalSize.y() + (2 * j + 1) * (tmpY + fWrap * WrapYHalfLength) + CentrePos.y();
//                UnitPosZ = -1. * TotalSize.z() + (2 * k + 1) * (CaloZHalfLength + fWrap * WrapZHalfLength) + CentrePos.z();
//
//                double w1 = 0;
//                double w2 = 0;
//                double w3 = 0;
//                double w4 = 0;
//                double w5 = 0;
//                double w6 = 0;
//
//                if (type == 1) {
//                    w1 = 1;
//                    w2 = -1;
//                    w3 = NoRemoved;
//                    w4 = -1;
//                    w5 = 1;
//                    w6 = -(idx - NoRemoved - 1);
//                }
//                if (type == 4) {
//                    w1 = -1;
//                    w2 = 1;
//                    w3 = -(idy - NoRemoved - 1);
//                    w4 = 1;
//                    w5 = -1;
//                    w6 = NoRemoved;
//                }
//                if (type == 2) {
//                    w1 = 1;
//                    w2 = 1;
//                    w3 = NoRemoved;
//                    w4 = 1;
//                    w5 = 1;
//                    w6 = NoRemoved;
//                }
//                if (type == 3) {
//                    w1 = -1;
//                    w2 = -1;
//                    w3 = -(idy - NoRemoved - 1);
//                    w4 = -1;
//                    w5 = -1;
//                    w6 = -(idx - NoRemoved - 1);
//                }
//
//                if (k % 2 == 0) {
//                    CaloXHalfLength = iSizeX - (((w1 * j) < w3) ? NoRemoved * (iSizeY + fWrap * WrapYHalfLength) : 0.);
//                    UnitPosX = UnitPosX + w2 * (((w1 * j) < w3) ? NoRemoved * (iSizeY + fWrap * WrapYHalfLength) : 0.);
//                } else {
//                    CaloYHalfLength = iSizeX - (((w4 * i) < w6) ? NoRemoved * (iSizeY + fWrap * WrapXHalfLength) : 0.);
//                    UnitPosY = UnitPosY + w5 * (((w4 * i) < w6) ? NoRemoved * (iSizeY + fWrap * WrapXHalfLength) : 0.);
//                }
//
//                /* APD */
//                double f1 = 0, f2 = 0;
//                if (type == 1) {
//                    f1 = -1;
//                    f2 = 1;
//                }
//                if (type == 2) {
//                    f1 = 1;
//                    f2 = 1;
//                }
//                if (type == 3) {
//                    f1 = -1;
//                    f2 = -1;
//                }
//                if (type == 4) {
//                    f1 = 1;
//                    f2 = -1;
//                }
//
//                APDwZHalfLength = CaloZHalfLength;
//                APDPosZ = 0.;
//                if (k % 2 == 0) {
//                    ADPwXHalfLength = APDZHalfLength + GlueZHalfLength;
//                    APDwYHalfLength = CaloYHalfLength;
//                    APDPosY = 0.;
//
//                    APDPosX = f1 * CaloXHalfLength - f1 * 0.5 * ADPwXHalfLength;
//                } else if (k % 2 == 1) {
//                    ADPwXHalfLength = CaloXHalfLength;
//                    APDwYHalfLength = APDZHalfLength + GlueZHalfLength;
//                    APDPosX = 0.;
//
//                    APDPosY = f2 * CaloYHalfLength - f2 * 0.5 * APDwYHalfLength;
//                }
//
//
//                Construct();
//                fCopyNo++;
//            }
//        }
//    }
//
//}

void CALConstruct::MatrixPlacementXYwithAbsorber(G4int xNo, G4int yNo, G4int zNo, const G4ThreeVector &CentrePos,
                                                 G4double AbsThickness, G4Material *AbsMat) {

    auto ifwrap = fWrap;

    auto idx = xNo;
    auto idy = yNo;
    auto idz = zNo + zNo / 2;

    auto iSizeX = CaloXHalfLength;
    auto iSizeY = CaloYHalfLength;
    auto iSizeZ = CaloZHalfLength;

    auto TotalSize = G4ThreeVector(xNo * (CaloXHalfLength + fWrap * WrapXHalfLength),
                                   yNo * (CaloYHalfLength + fWrap * WrapYHalfLength),
                                   zNo * (CaloZHalfLength + fWrap * WrapZHalfLength) + zNo / 2. * AbsThickness / 2.);

    // Construct Detector LV
    ifAbsorber = false;
    fRecordLV = true;
    /* APD Size */
    ADPwXHalfLength = APDZHalfLength + GlueZHalfLength;
    APDwYHalfLength = CaloYHalfLength;
    APDwZHalfLength = CaloZHalfLength;
    ConstructLV();

    // Construct Absorber LV
    ifAbsorber = true;
    fWrap = false;
    fCALMaterial = AbsMat;
    fVis = new G4VisAttributes(G4Colour(0.5, 0.23, 0.89));
    fRecordLV = false;

    CaloXHalfLength = xNo * (iSizeX + fWrap * WrapXHalfLength);
    CaloYHalfLength = yNo * (iSizeY + fWrap * WrapYHalfLength);
    CaloZHalfLength = AbsThickness / 2.;
    ConstructLV();

    // Initialize
    ifAbsorber = false;
    fRecordLV = true;
    fWrap = ifwrap;
    CaloXHalfLength = iSizeX;
    CaloYHalfLength = iSizeY;
    CaloZHalfLength = iSizeZ;

    auto Abs_No = 0;
    // Z layer Loop
    for (int k = 0; k < idz; k++) {
        // place detector
        // along x direction
        if (k % 3 == 0) {
            idx = xNo;
            idy = yNo;
            CaloXHalfLength = iSizeX;
            CaloYHalfLength = iSizeY;
        }
        // along y direction
        if (k % 3 == 1) {
            idx = yNo;
            idy = xNo;
            CaloXHalfLength = iSizeY;
            CaloYHalfLength = iSizeX;
        }
        // place absorber
        if (k % 3 == 2) {
            idx = 1;
            idy = 1;
            CaloXHalfLength = xNo * (iSizeX + fWrap * WrapXHalfLength);
            CaloYHalfLength = yNo * (iSizeY + fWrap * WrapYHalfLength);
        }

        // Y layer Loop
        for (int j = 0; j < idy; j++) {
            // X layer Loop
            for (int i = 0; i < idx; i++) {
                auto tmpX = ((k % 3 == 0) ? iSizeX : iSizeY);
                auto tmpY = ((k % 3 == 1) ? iSizeX : iSizeY);

                UnitPosX = -1. * TotalSize.x() + (2 * i + 1) * (tmpX + fWrap * WrapXHalfLength) + CentrePos.x();
                UnitPosY = -1. * TotalSize.y() + (2 * j + 1) * (tmpY + fWrap * WrapYHalfLength) + CentrePos.y();
                UnitPosZ = -1. * TotalSize.z() + (2 * (k - Abs_No) + 1) * (CaloZHalfLength + fWrap * WrapZHalfLength) +
                        Abs_No * AbsThickness + CentrePos.z();

                if (k % 3 == 2) {
                    Abs_No++;
                    UnitPosX = CentrePos.x();
                    UnitPosY = CentrePos.y();
                    UnitPosZ = -1. * TotalSize.z() + (2 * (k - Abs_No) + 2) * (CaloZHalfLength + fWrap * WrapZHalfLength) +
                            (Abs_No - 0.5) * AbsThickness + CentrePos.z();

                    ifAbsorber = true;
                    fWrap = false;


                    CaloZHalfLength = AbsThickness / 2.;
                    Construct(fAbsLV, nullptr, nullptr);
                } else {
                    ifAbsorber = false;
                    if (k % 3 == 0) // along x
                        Construct(fCaloLV, fWrapLV, fAPDWLV, 0. * degree);
                    if (k % 3 == 1) // along y
                        Construct(fCaloLV, fWrapLV, fAPDWLV, 90 * degree);

                }
                fWrap = ifwrap;
                CaloZHalfLength = iSizeZ;

                if (!ifAbsorber) fCopyNo++;
            }
        }
    }
}

