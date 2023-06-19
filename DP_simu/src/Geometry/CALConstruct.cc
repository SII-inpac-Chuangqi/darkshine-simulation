#include "Geometry/CALConstruct.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
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
    fCALName = CALName;
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

    ifAbsorber = true;
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
    fAPDLVVector = in.fAPDLVVector;
    fFiberCladVis = in.fFiberCladVis;
    fFiberVis = in.fFiberVis;
    fFiberCladMaterial = in.fFiberCladMaterial;
    fFiberMaterial = in.fFiberMaterial;
    fFiberCladLV = in.fFiberCladLV;
    fFiberLV = in.fFiberLV;
    fAPDSD = in.fAPDSD;
    fFiberCladSD = in.fFiberCladSD;
    fFiberSD = in.fFiberSD;
    fAPDMaterial = in.fAPDMaterial;
    Wrap_LSkinSurface = in.Wrap_LSkinSurface;
    APD_LBorderSurface = in.APD_LBorderSurface;
    fAbsLVVector = in.fAbsLVVector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
CALConstruct::~CALConstruct() {
    delete fVis;
    delete fAPDWLV;
    delete fWrapLV;
    delete fCaloLV;
    delete fCALSD;
    delete fCALWrapSD;
    delete fWrapVis;
    delete fAPDVis;
    //delete fOutlineLV;

    fCaloLVVector.clear();
    fCaloLVVector.shrink_to_fit();

    fWrapLVVector.clear();
    fWrapLVVector.shrink_to_fit();

    fAPDLVVector.clear();
    fAPDLVVector.shrink_to_fit();

    PVVector.clear();
    PVVector.shrink_to_fit();

    fAbsLVVector.clear();
    fAbsLVVector.shrink_to_fit();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Volume relationship:
//   0 Outline (Wrap)
//   ├-1 Calo
//   └-2 APD
//                   ->|                 |<- CaloZHalfLength + APDZHalfLength + WrapZHalfLength
// ->| |<-             |             ->| |<- WrapZHalfLength
//   ┌-----------------|-----------------┐
//   |0┌---------------|-------------┐   |
//   | | 1             |             |   |
//   | |               |             ├-┐ |
//   | |              ┌╋             |2| | -------------> Z Axis
//   | |            ->||<- APDZHalf  ├┬┘ |
//   | |              |    Length    ||  |
//   | └--------------|--------------┘|  |
//   └----------------|--------------||--┘
//                    |            ->||<- APDZHalfLength
//                  ->|              |<- CaloZHalfLength
void CALConstruct::CalUnit1Construct() {
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
    fOutlineLV = WrapLV;

    if (fWrapVis) {
        fWrapVis->SetVisibility(true);
        WrapLV->SetVisAttributes(fWrapVis);
    } else WrapLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    if(fCALWrapSD) WrapLV->SetSensitiveDetector(fCALWrapSD);

    // Crystal
    auto CaloBox = new G4Box(fCALName + "_Box", CaloXHalfLength, CaloYHalfLength, CaloZHalfLength);
    auto CaloLV = new G4LogicalVolume(CaloBox, fCALMaterial, fCALName + "_LV",
                                      nullptr, nullptr, nullptr);
    fCaloLV = CaloLV;
    fCaloLVVector.push_back(CaloLV);
    if (fVis) {
        fVis->SetVisibility(true);
        CaloLV->SetVisAttributes(fVis);
    } else CaloLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    if (fCALSD) CaloLV->SetSensitiveDetector(fCALSD);

    // APD
    auto APDBox = new G4Box(fCALName + "_APDWorld_Box", APDXHalfLength, APDYHalfLength, APDZHalfLength);
    auto APDLV = new G4LogicalVolume(APDBox, G4Material::GetMaterial("vacuum"), fCALName + "_APDWorld_LV",
                                     nullptr, nullptr, nullptr);
    fAPDWLV = APDLV;

    if (fAPDVis) {
        fAPDVis->SetVisibility(true);
        APDLV->SetVisAttributes(fAPDVis);
    } else APDLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    /// construct physical volume
    // place crystal
    auto CaloPV = new G4PVPlacement(nullptr,G4ThreeVector(0, 0, - APDZHalfLength),
                                    fCaloLV, fCALName + "_PV", fWrapLV,
                                    false, fCopyNo, fCheckOverlap);
    PVVector.push_back(CaloPV);
    // place APD
    auto APDPV = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, CaloZHalfLength),
                                   fAPDWLV, fCALName + "_APDWorld_PV", fWrapLV,
                                   false, fCopyNo, fCheckOverlap);
    PVVector.push_back(APDPV);

    // optical surface
    if (dControl->if_optical) {
        new G4LogicalSkinSurface(fCALName + "_WrapSkinSurface", fCaloLV, dControl->Wrap_Surface);
        new G4LogicalBorderSurface(fCALName + "_APDGlueSurface", CaloPV, APDPV, dControl->APD_Surface);
    }
}

//   0 Outline (vacuum)
//   ├-1 Wrap
//   ├-2 Calo
//   └-3 APD
//                   ->|                 |<- CaloZHalfLength + APDZHalfLength + WrapZHalfLength
// ->| |<-             |             ->| |<- WrapZHalfLength
//   ┌-----------------|-----------------┐
//   |1┌---------------|-------------┬-┐ |
//   | |               |             |0| |
//   | | 2             |             ├-┤ |
//   | |              ┌╋             |3| | --------> Z Axis
//   | |            ->||<- APDZHalf  ├-┤ |
//   | |              |    Length    | | |
//   | └--------------|--------------┴┬┘ |
//   └----------------|--------------||--┘
//                    |            ->||<- APDZHalfLength
//                  ->|              |<- CaloZHalfLength
void CALConstruct::CalUnit2Construct() {

    if (!CaloXHalfLength || !CaloYHalfLength || !CaloZHalfLength) {
        G4cout << fCALName << " Construction Error: at least size of one dimension is zero." << G4endl;
        return;
    }
    /// construct logical volume
    // Outline
    auto OutlineBox = new G4Box(fCALName + "_OutlineBox",
                                CaloXHalfLength + WrapXHalfLength,
                                CaloYHalfLength + WrapYHalfLength,
                                CaloZHalfLength + WrapZHalfLength + APDZHalfLength);
    auto OutlineLV = new G4LogicalVolume(OutlineBox, G4Material::GetMaterial("vacuum"),
                                         fCALName + "_OutlineLV",
                                         nullptr, nullptr, nullptr);
    fOutlineLV = OutlineLV;
    OutlineLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    // Wrap
    auto InnerBox = new G4Box(fCALName + "+iBox",
                              CaloXHalfLength,
                              CaloYHalfLength,
                              CaloZHalfLength + APDZHalfLength);
    auto WrapBox = new G4SubtractionSolid(fCALName + "_WrapBox", OutlineBox, InnerBox);
    auto WrapLV = new G4LogicalVolume(WrapBox, fWrapMaterial, fCALName + "_LVW",
                                      nullptr, nullptr, nullptr);
    fWrapLV = WrapLV;

    if (fWrapVis) {
        fWrapVis->SetVisibility(true);
        WrapLV->SetVisAttributes(fWrapVis);
    } else WrapLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    if(fCALWrapSD) WrapLV->SetSensitiveDetector(fCALWrapSD);

    // Crystal
    auto CaloBox = new G4Box(fCALName + "_Box", CaloXHalfLength, CaloYHalfLength, CaloZHalfLength);
    auto CaloLV = new G4LogicalVolume(CaloBox, fCALMaterial, fCALName + "_LV",
                                      nullptr, nullptr, nullptr);
    fCaloLV = CaloLV;
    fCaloLVVector.push_back(CaloLV);
    if (fVis) {
        fVis->SetVisibility(true);
        CaloLV->SetVisAttributes(fVis);
    } else CaloLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    if (fCALSD) CaloLV->SetSensitiveDetector(fCALSD);

    // APD
    auto APDBox = new G4Box(fCALName + "_APDWorld_Box", APDXHalfLength, APDYHalfLength, APDZHalfLength);
    auto APDLV = new G4LogicalVolume(APDBox, G4Material::GetMaterial("vacuum"), fCALName,
                                     nullptr, nullptr, nullptr);
    fAPDWLV = APDLV;

    if (fAPDVis) {
        fAPDVis->SetVisibility(true);
        APDLV->SetVisAttributes(fAPDVis);
    } else APDLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    /// construct physical volume
    // place wrap
    auto WrapPV = new G4PVPlacement(nullptr, G4ThreeVector(0,0,0),
                                    fWrapLV, fCALName + "_WrapPV", fOutlineLV,
                                    false, fCopyNo, fCheckOverlap);
    PVVector.push_back(WrapPV);
    // place crystal
    auto CaloPV = new G4PVPlacement(nullptr,G4ThreeVector(0, 0, - APDZHalfLength),
                                    fCaloLV, fCALName + "_PV", fOutlineLV,
                                    false, fCopyNo, fCheckOverlap);
    PVVector.push_back(CaloPV);
    // place APD
    auto APDPV = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, CaloZHalfLength),
                                   fAPDWLV, fCALName + "_APDWorld_PV", fOutlineLV,
                                   false, fCopyNo, fCheckOverlap);
    PVVector.push_back(APDPV);

    // optical surface
    if (dControl->if_optical) {
        new G4LogicalBorderSurface(fCALName + "_WrapSurface", CaloPV, WrapPV, dControl->Wrap_Surface);
        new G4LogicalBorderSurface(fCALName + "_APDGlueSurface", CaloPV, APDPV, dControl->APD_Surface);
    }
}

// Volume relationship:
//   0 Wrap
//   ├-1 Calo
//   ├-2 Fiber Clad
//   ├-3 Fiber
//   └-4 SiPM
//                    ->|                  |<- CaloZHalfLength + APDZHalfLength + APDCaloHalfGap + WrapZHalfLength
// ->| |<-              |              ->| |<- WrapZHalfLength
//                      |             ->||<- APDZHalfLength
//   ┌------------------|---------------||-┐
//   |0┌----------------|------------┐  || |
//   | | 1           ->||<-          | ┌┴┤ |
//   | ╞========2=====╤╧╋============╪╤╡ │ | -------------> Z Axis
//   | |            ->| |<-          ||└ ┘ |<- APDZHalfLength
//   | └--------------|--------------┘|    |
//   └----------------|--------------||----┘
//                    |            ->||<- APDCaloHalfGap
//                  ->|              |<- CaloZHalfLength
void CALConstruct::CalWLSUnitConstruct() {
    // sanity check
    if (!CaloXHalfLength || !CaloYHalfLength || !CaloZHalfLength) {
        G4cout << fCALName << " Construction Error: at least size of one dimension is zero." << G4endl;
        return;
    }
    // -----------------------------------------
    // construct logical volume
    //

    // Wrap

    auto WrapBox = new G4Box(fCALName + "_WrapBox",
                             CaloXHalfLength + WrapXHalfLength,
                             CaloYHalfLength + WrapYHalfLength,
                             CaloZHalfLength + APDZHalfLength + APDCaloHalfGap + WrapZHalfLength);
    auto WrapLV = new G4LogicalVolume(WrapBox, fWrapMaterial, fCALName + "_LVW",
                                      nullptr, nullptr, nullptr);
    fWrapLV = WrapLV;
    fOutlineLV = WrapLV;
    if(fWrapVis) {
        fWrapVis->SetVisibility(true);
        WrapLV->SetVisAttributes(fWrapVis);
    } else WrapLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    if (fCALWrapSD) WrapLV->SetSensitiveDetector(fCALWrapSD);

    // Crystal

    auto CaloBox = new G4Box(fCALName + "_Box", CaloXHalfLength, CaloYHalfLength, CaloZHalfLength);

    auto CaloHoleTubs = new G4Tubs(fCALName + "_Tubs", 0, CaloHoleRadius,
                                   CaloZHalfLength * 2,
                                   0*deg, 360*deg );
    auto CaloWithHole = new G4SubtractionSolid(fCALName + "_Box_1",
                                                CaloBox,
                                                CaloHoleTubs,
                                                nullptr,
                                                G4ThreeVector(0,0,0));
    auto CaloLV = new G4LogicalVolume(CaloWithHole,
                                      fCALMaterial,
                                      fCALName + "_LV",
                                      nullptr, nullptr, nullptr);

    fCaloLV = CaloLV;
    fCaloLVVector.push_back(CaloLV);
    if (fVis) {
        fVis->SetVisibility(true);
        CaloLV->SetVisAttributes(fVis);
    } else CaloLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    if (fCALSD) CaloLV->SetSensitiveDetector(fCALSD);

    // Fiber Clad

    auto FiberCladTubs = new G4Tubs(fCALName + "_FiberClad_Tubs", FiberRadius, CaloHoleRadius,
                                    (CaloZHalfLength + APDCaloHalfGap),
                                    0*deg, 360*deg);

    auto FiberCladLV = new G4LogicalVolume(FiberCladTubs, fFiberCladMaterial, fCALName + "_FiberClad_LV",
                                           nullptr,nullptr,nullptr);
    fFiberCladLV = FiberCladLV;

    if (fFiberCladVis) {
        fFiberCladVis->SetVisibility(true);
        FiberCladLV->SetVisAttributes(fFiberCladVis);
    } else FiberCladLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    if (fFiberCladSD) FiberCladLV->SetSensitiveDetector(fFiberCladSD);

    // Optical Fiber
    auto FiberTubs = new G4Tubs(fCALName + "_Fiber_Tubs", 0, FiberRadius,
                                (CaloZHalfLength + APDCaloHalfGap),
                                0*deg, 360*deg);
    auto FiberLV = new G4LogicalVolume(FiberTubs, fFiberMaterial, fCALName + "_Fiber_LV",
                                       nullptr,nullptr,nullptr);
    fFiberLV = FiberLV;

    if (fFiberVis) {
        fFiberVis->SetVisibility(true);
        FiberLV->SetVisAttributes(fFiberVis);
    } else FiberLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    if(fFiberSD) FiberLV->SetSensitiveDetector(fFiberSD);

    // APD
    auto APDBox = new G4Box(fCALName + "_APDWorld_Box", APDXHalfLength, APDYHalfLength, APDZHalfLength);

    auto APDLV = new G4LogicalVolume(APDBox, fAPDMaterial, fCALName + "_APDWorld_LV");
    fAPDWLV = APDLV;
    fAPDLVVector.emplace_back(APDLV);
    if (fAPDVis) {
        fAPDVis->SetVisibility(true);
        APDLV->SetVisAttributes(fAPDVis);
    } else APDLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    if(fAPDSD) APDLV->SetSensitiveDetector(fAPDSD);

    // -----------------------------------------------
    // Placement
    //

    // Place Crystal
    auto CaloPV = new G4PVPlacement(nullptr,
                                    G4ThreeVector(0, 0, - APDZHalfLength - APDCaloHalfGap),
                                    fCaloLV, fCALName + "_PV",
                                    fWrapLV, false, fCopyNo, fCheckOverlap);
    PVVector.emplace_back(CaloPV);

    // Place Fiber Clad
    auto FiberCladPV = new G4PVPlacement(nullptr,
                                         G4ThreeVector(0, 0, -APDZHalfLength),
                                         fFiberCladLV, fCALName + "_FiberClad_PV",
                                         fWrapLV, false, fCopyNo, fCheckOverlap);
    PVVector.emplace_back(FiberCladPV);
    // Place Fiber
    auto FiberPV = new G4PVPlacement(nullptr,
                                     G4ThreeVector(0,0, - APDZHalfLength),
                                     fFiberLV, fCALName + "_Fiber_PV",
                                     fWrapLV, false, fCopyNo, fCheckOverlap);
    PVVector.emplace_back(FiberPV);

    // Place APD
    auto APDPV = new G4PVPlacement(nullptr,
                                   G4ThreeVector(0,0, CaloZHalfLength + APDCaloHalfGap ),
                                   APDLV, fCALName + "_APDWorld_PV",
                                   fWrapLV, false, fCopyNo, fCheckOverlap);
    PVVector.emplace_back(APDPV);

    // -----------------------------------------------
    // SkinSurface and BorderSurface
    //
    if (dControl->if_optical) {
        Wrap_LSkinSurface = new G4LogicalSkinSurface( "ESR_surface", fOutlineLV, dControl->Wrap_Surface); //here just use the inner skin surface, which is just between ESR and the scintallator
        APD_LBorderSurface = new G4LogicalBorderSurface( "SIPM_surface", FiberPV, APDPV, dControl->APD_Surface);
    }
}

//   0 Absorber
void CALConstruct::AbsorberUnitConstruct() {
    if (!AbsXHalfLength || !AbsYHalfLength || !AbsZHalfLength) {
        G4cout << fCALName << "Construction Error: at least size of one dimension is zero." << G4endl;
        return;
    }

    auto AbsBox = new G4Box(fCALName + "_AbsBox", AbsXHalfLength, AbsYHalfLength, AbsZHalfLength);
    auto AbsLV = new G4LogicalVolume(AbsBox, fAbsMaterial, fCALName + "_AbsLV",
                                     nullptr, nullptr, nullptr);
    fAbsLVVector.emplace_back(AbsLV);

    if (fVis) {
        fVis->SetVisibility(true);
        AbsLV->SetVisAttributes(fVis);
    } else AbsLV->SetVisAttributes(G4VisAttributes::GetInvisible());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LogicalVolume* CALConstruct::MatrixConstruct(G4int xNo, G4int yNo, G4int zNo,
                                               G4LogicalVolume *elementLV,
                                               G4Material * regionMat,
                                               G4int tree_height,
                                               G4ThreeVector gap,
                                               G4bool if_place_to_mother) {
    /// check consistency
    if (!xNo || !yNo || !zNo) {
        G4cout << fCALName << " Construction Error: at least one of the matrix element is zero." << G4endl;
        return nullptr;
    }

    /// construct unit LV and get size
    auto UnitBox = dynamic_cast<G4Box*>(elementLV->GetSolid());
    auto UnitXHalfLength = UnitBox->GetXHalfLength();
    auto UnitYHalfLength = UnitBox->GetYHalfLength();
    auto UnitZHalfLength = UnitBox->GetZHalfLength();

    /// construct Group LV
    auto GroupHalfSize = G4ThreeVector(xNo * UnitXHalfLength + xNo * 0.5 * gap.x(),
                                       yNo * UnitYHalfLength + yNo * 0.5 * gap.y(),
                                       zNo * UnitZHalfLength + zNo * 0.5 * gap.z());
    auto GroupBox = new G4Box(fCALName + "_Box_h" + std::to_string(tree_height), GroupHalfSize.x(), GroupHalfSize.y(), GroupHalfSize.z());

    G4LogicalVolume* motherLV = nullptr;
    if (if_place_to_mother) {
        motherLV = fMotherVolume;
    } else {
        motherLV = new G4LogicalVolume(GroupBox, regionMat, fCALName + "_LV_h" + std::to_string(tree_height),
                                           nullptr, nullptr, nullptr);
#ifndef DEBUG
        motherLV->SetVisAttributes(G4VisAttributes::GetInvisible());
#endif
    }

    /// Unit LV Placement
    G4String UnitName = (tree_height == 1 ? fCALName + "_UnitPV" : fCALName + "_PV_h" + std::to_string(tree_height - 1) );
    fCopyNo = 0;
    G4PVPlacement* UnitPV = nullptr;
    for (int k = 0; k < zNo; k++) {
        for (int j = 0; j < yNo; j++) {
            for (int i = 0; i < xNo; i++) {
                UnitPosX = -1. * GroupHalfSize.x() + (2 * i + 1) * UnitXHalfLength + (i + 0.5) * gap.x();
                UnitPosY = -1. * GroupHalfSize.y() + (2 * j + 1) * UnitYHalfLength + (j + 0.5) * gap.y();
                UnitPosZ = -1. * GroupHalfSize.z() + (2 * k + 1) * UnitZHalfLength + (k + 0.5) * gap.z();

                UnitPV = new G4PVPlacement(nullptr,
                                           G4ThreeVector(UnitPosX, UnitPosY, UnitPosZ),
                                           elementLV,
                                           UnitName,
                                           motherLV,
                                           false,
                                           fCopyNo,
                                           fCheckOverlap);
                PVVector.emplace_back(UnitPV);

                fCopyNo++;
            }
        }
    }

    return motherLV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LogicalVolume* CALConstruct::XYCrossingConstruct(G4int xNo, G4int yNo,
                                                   G4LogicalVolume *elementLV,
                                                   G4Material *regionMat, G4int tree_height, G4double gap,
                                                   G4bool onlyX) {
    /// check consistency
    if (!xNo || !yNo ) {
        G4cout << fCALName << " Construction Error: at least one of the matrix element is zero." << G4endl;
        return nullptr;
    }

    /// construct unit LV and get size
    auto UnitBox = dynamic_cast<G4Box*>(elementLV->GetSolid());
    auto UnitXHalfLength = UnitBox->GetXHalfLength();
    auto UnitYHalfLength = UnitBox->GetYHalfLength();
    auto UnitZHalfLength = UnitBox->GetZHalfLength();

    /// construct Group LV
    int zNo = 2;
    if (onlyX) zNo = 1;
    auto GroupHalfSize = G4ThreeVector(UnitZHalfLength + 0.25 * gap,
                                       UnitZHalfLength + 0.25 * gap,
                                       zNo * UnitXHalfLength + (zNo - 0.5) * 0.5 * gap);
    auto GroupBox = new G4Box(fCALName + "_Box_h" + std::to_string(tree_height), GroupHalfSize.x(), GroupHalfSize.y(), GroupHalfSize.z());
    auto GroupLV = new G4LogicalVolume(GroupBox, regionMat, fCALName + "_LV_h" + std::to_string(tree_height),
                                       nullptr, nullptr, nullptr);

    GroupLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    /// rotation matrix

    auto fRotY90 = new G4RotationMatrix();
    fRotY90->rotateY(- 90 * degree);
    auto fRotY90X90 = new G4RotationMatrix();
    fRotY90X90->rotateY(90 * degree);
    fRotY90X90->rotateX(90 * degree);

    /// Unit LV Placement
    G4String UnitName = (tree_height == 1 ? fCALName + "_UnitPV" : fCALName + "_PV_h" + std::to_string(tree_height - 1) );
    fCopyNo = 0;
    G4PVPlacement* UnitPV = nullptr;

    /// along Y
    if (! onlyX) {
        for (int i = 0; i < yNo; i++) {
            UnitPosX = 0;
            UnitPosY = -1. * GroupHalfSize.y() + (2 * i + 1) * UnitYHalfLength + (i + 0.25) * gap;
            UnitPosZ = -1. * GroupHalfSize.z() + UnitXHalfLength + 0.25 * gap;

            UnitPV = new G4PVPlacement(fRotY90,
                                       G4ThreeVector(UnitPosX, UnitPosY, UnitPosZ),
                                       elementLV,
                                       UnitName,
                                       GroupLV,
                                       false,
                                       fCopyNo,
                                       fCheckOverlap);
            PVVector.emplace_back(UnitPV);
            fCopyNo++;
        }
    }
    /// along X
    for (int i = 0; i < xNo; i++ ) {
        UnitPosX = -1. * GroupHalfSize.x() + (2 * i + 1) * UnitYHalfLength + (i + 0.25) * gap;
        UnitPosY = 0;
        UnitPosZ = -1. * GroupHalfSize.z() + 3 * UnitXHalfLength + 1.25 * gap;
        if(onlyX) UnitPosZ = 0;

        UnitPV = new G4PVPlacement(fRotY90X90,
                                   G4ThreeVector(UnitPosX, UnitPosY, UnitPosZ),
                                   elementLV,
                                   UnitName,
                                   GroupLV,
                                   false,
                                   fCopyNo,
                                   fCheckOverlap);
        PVVector.emplace_back(UnitPV);
        fCopyNo++;
    }
    return GroupLV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector CALConstruct::LinearPlacementWithAbsorber(G4int zNo, const std::vector<std::tuple<int, int, double>> abs_thickness_list,
                                               G4LogicalVolume *calLayerLV, G4Material *AbsMat,
                                               G4double gap,
                                               G4bool AlternateRotationZ) {
    auto TotalHalfSize = G4ThreeVector(0, 0, 0);
    // check consistency
    if (!zNo) {
        G4cout << fCALName << " Construction Error: at least one of the matrix element is zero." << G4endl;
        return TotalHalfSize;
    }

    // Calculate Calo layer total size
    auto UnitBox = dynamic_cast<G4Box*>(calLayerLV->GetSolid());
    auto vec = G4ThreeVector(UnitBox->GetXHalfLength(),
                                 UnitBox->GetYHalfLength(),
                                 UnitBox->GetZHalfLength());
    if (HepRot) vec.transform(HepRot->inverse());
    auto UnitXHalfLength = std::fabs(vec.x());
    auto UnitYHalfLength = std::fabs(vec.y());
    auto UnitZHalfLength = std::fabs(vec.z());

    G4int startn;
    G4int endn;
    G4double abs_thickness;
    G4double total_thickness = 0;
    std::vector<G4double> abs_thickness_vector{};
    for (auto thick_i : abs_thickness_list) {
        std::tie(startn, endn, abs_thickness) =  thick_i;
        /// calculate total Thickness
        for (int i = startn; i <= endn; i++) {
            total_thickness += abs_thickness;
            abs_thickness_vector.emplace_back(abs_thickness);
        }
    }

    TotalHalfSize = G4ThreeVector(UnitXHalfLength,
                                  UnitYHalfLength,
                                  zNo * UnitZHalfLength + 0.5 * total_thickness + (zNo - 1) * gap);

    // Construct Absorber LV
    AbsXHalfLength = UnitXHalfLength;
    AbsYHalfLength = UnitYHalfLength;
    fAbsMaterial = AbsMat;
    fVis = new G4VisAttributes(G4Colour(0.5, 0.23, 0.89));
    for (auto thickness_i : abs_thickness_vector) {
        AbsZHalfLength = 0.5 * thickness_i;
        if (AbsZHalfLength > 0) AbsorberUnitConstruct();
        else fAbsLVVector.emplace_back(nullptr);
    }

    // Initialize
    G4int Abs_No = 0;
    G4PVPlacement* UnitPV = nullptr;
    G4PVPlacement* AbsPV = nullptr;
    fCopyNo = 0;

    UnitPosX = 0;
    UnitPosY = 0;
    UnitPosZ = -TotalHalfSize.z() - gap;

    auto fRotZ90 = new G4RotationMatrix(); // only used by AlternateRotatinoZ
    fRotZ90->rotateZ(90 * degree);

    for (int i = 0; i < zNo; i++) {
        /// place Calo Layer
        UnitPosX = 0;
        UnitPosY = 0;
        UnitPosZ += gap + UnitZHalfLength;

        UnitPV = new G4PVPlacement((AlternateRotationZ && i % 2 == 1) ? fRotZ90 : HepRot,
                                   G4ThreeVector(UnitPosX, UnitPosY, UnitPosZ),
                                   calLayerLV,
                                   fCALName + "_LayerPV",
                                   fMotherVolume,
                                   false,
                                   fCopyNo,
                                   fCheckOverlap);
        PVVector.emplace_back(UnitPV);
        fCopyNo++;
        /// place Abs Layer
        if (i < zNo - 1) {
            UnitPosZ += UnitZHalfLength + gap + 0.5 * abs_thickness_vector.at(i);
            if (fAbsLVVector.at(i)) {
                AbsPV = new G4PVPlacement(nullptr,
                                          G4ThreeVector(UnitPosX, UnitPosY, UnitPosZ),
                                          fAbsLVVector.at(i),
                                          fCALName + "_AbsPV",
                                          fMotherVolume,
                                          false,
                                          Abs_No,
                                          fCheckOverlap);
                PVVector.emplace_back(AbsPV);
                Abs_No++;
            }
            UnitPosZ += 0.5 * abs_thickness_vector.at(i);
        }
    }

    return TotalHalfSize;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector CALConstruct::MatrixPlacement(G4int xNo, G4int yNo, G4int zNo, const G4ThreeVector &CentrePos) {
    auto TotalHalfSize = G4ThreeVector(0, 0, 0);
    // check consistency
    if (!xNo || !yNo || !zNo) {
        G4cout << fCALName << " Construction Error: at least one of the matrix element is zero." << G4endl;
        return TotalHalfSize;
    }

    // construct unit
    fAPDVis = new G4VisAttributes(G4Colour(0.5, 0.5, .0));
    CalUnit1Construct();
    //CalUnit2Construct();

    // calculate total size
    auto UnitBox = dynamic_cast<G4Box*>(fOutlineLV->GetSolid());
    auto UnitXHalfLength = UnitBox->GetXHalfLength();
    auto UnitYHalfLength = UnitBox->GetYHalfLength();
    auto UnitZHalfLength = UnitBox->GetZHalfLength();

    TotalHalfSize = G4ThreeVector(xNo * UnitXHalfLength,
                              yNo * UnitYHalfLength,
                              zNo * UnitZHalfLength);

    G4PVPlacement* UnitPV = nullptr;
    for (int k = 0; k < zNo; k++) {
        for (int j = 0; j < yNo; j++) {
            for (int i = 0; i < xNo; i++) {
                UnitPosX = -1. * TotalHalfSize.x() + (2 * i + 1) * UnitXHalfLength + CentrePos.x();
                UnitPosY = -1. * TotalHalfSize.y() + (2 * j + 1) * UnitYHalfLength + CentrePos.y();
                UnitPosZ = -1. * TotalHalfSize.z() + (2 * k + 1) * UnitZHalfLength + CentrePos.z();

                UnitPV = new G4PVPlacement(nullptr,
                                  G4ThreeVector(UnitPosX, UnitPosY, UnitPosZ),
                                  fOutlineLV,
                                  fCALName + "_UnitPV",
                                  fMotherVolume,
                                  false,
                                  fCopyNo,
                                  fCheckOverlap);
                PVVector.push_back(UnitPV);

                fCopyNo++;
            }
        }
    }

    return TotalHalfSize;
}

void CALConstruct::MatrixPlacementXYwithAbsorber(G4int xNo, G4int yNo, G4int zNo, const G4ThreeVector &CentrePos,
                                                 G4double AbsThickness, G4Material *AbsMat) {

    // rotation matrix
    auto fRotY90 = new G4RotationMatrix();
    fRotY90->rotateY(- 90 * degree);
    auto fRotY90X90 = new G4RotationMatrix();
    fRotY90X90->rotateY(90 * degree);
    fRotY90X90->rotateX(90 * degree);

    // Construct Detector LV
    fAPDVis = new G4VisAttributes(G4Colour(0.5, 0.5, .0));
    fFiberCladVis = new G4VisAttributes(G4Colour(0.6,0.7,0.8));
    fFiberVis = new G4VisAttributes(G4Colour(0.4,0.3,0.2));
    //CalUnit1Construct();
    CalWLSUnitConstruct();

    // Calculate total size
    auto UnitBox = dynamic_cast<G4Box*>(fOutlineLV->GetSolid());
    auto UnitXHalfLength = UnitBox->GetXHalfLength();
    auto UnitYHalfLength = UnitBox->GetYHalfLength();
    auto UnitZHalfLength = UnitBox->GetZHalfLength();

    auto TotalHalfSize = G4ThreeVector(UnitZHalfLength,
                                       UnitZHalfLength,
                                       ceil(0.5 * zNo) * 2 * UnitXHalfLength
                                       + floor(zNo / 2) * 0.5 * AbsThickness) ;

    // Construct Absorber LV
    AbsXHalfLength = TotalHalfSize.x();
    AbsYHalfLength = TotalHalfSize.y();
    AbsZHalfLength = 0.5 * AbsThickness;
    fAbsMaterial = AbsMat;
    fVis = new G4VisAttributes(G4Colour(0.5, 0.23, 0.89));

    if ( AbsThickness <= 0) ifAbsorber = false;

    if ( ifAbsorber ) AbsorberUnitConstruct();
    // Initialize
    G4int Abs_No = 0;
    G4PVPlacement* CaloUnitPV = nullptr;
    G4PVPlacement* AbsPV = nullptr;
    // Z layer Loop
    for (int k = 0; k < zNo; k++) {
        // place detector
        if (k % 2 == 0) {
            // along y
            for (int j = 0; j < yNo; j++) {
                UnitPosX = CentrePos.x();
                UnitPosY = -1. * TotalHalfSize.y() + (2 * j + 1) * UnitYHalfLength + CentrePos.y();
                UnitPosZ = -1. * TotalHalfSize.z() + (k / 2) * (4 * UnitXHalfLength + AbsThickness)
                           + UnitXHalfLength + CentrePos.z();

                CaloUnitPV = new G4PVPlacement(fRotY90,
                                               G4ThreeVector(UnitPosX, UnitPosY, UnitPosZ),
                                               fOutlineLV,
                                               fCALName + "_UnitPV",
                                               fMotherVolume,
                                               false,
                                               fCopyNo,
                                               fCheckOverlap);
                PVVector.push_back(CaloUnitPV);
                fCopyNo++;
            }
            // along x
            for (int i = 0; i < xNo; i++) {
                UnitPosX = -1. * TotalHalfSize.x() + (2 * i + 1) * UnitYHalfLength + CentrePos.x();
                UnitPosY = CentrePos.y();
                UnitPosZ = -1. * TotalHalfSize.z() + (k / 2) * (4 * UnitXHalfLength + AbsThickness)
                           + 3 * UnitXHalfLength + CentrePos.z();

                CaloUnitPV = new G4PVPlacement(fRotY90X90,
                                               G4ThreeVector(UnitPosX, UnitPosY, UnitPosZ),
                                               fOutlineLV,
                                               fCALName + "_UnitPV",
                                               fMotherVolume,
                                               false,
                                               fCopyNo,
                                               fCheckOverlap);
                PVVector.push_back(CaloUnitPV);
                fCopyNo++;
            }
        }
        // place absorber
        if (k % 2 == 1) {
            UnitPosX = CentrePos.x();
            UnitPosY = CentrePos.y();
            UnitPosZ = -1. * TotalHalfSize.z() + ((k / 2) + 1) * (4 * UnitXHalfLength + AbsThickness)
                       - 0.5 * AbsThickness + CentrePos.z();

            if ( ifAbsorber ) {
                AbsPV = new G4PVPlacement(nullptr,
                                          G4ThreeVector(UnitPosX, UnitPosY, UnitPosZ),
                                          fAbsLVVector.at(0),
                                          fCALName + "_AbsPV",
                                          fMotherVolume,
                                          false,
                                          Abs_No,
                                          fCheckOverlap);
                PVVector.push_back(AbsPV);
                Abs_No++;
            }
        }
    }

}

