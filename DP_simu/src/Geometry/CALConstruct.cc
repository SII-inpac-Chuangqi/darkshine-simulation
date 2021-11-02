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
    delete fAPDVis;
    //delete fOutlineLV;

    fCaloLVVector.clear();
    fCaloLVVector.shrink_to_fit();

    fWrapLVVector.clear();
    fWrapLVVector.shrink_to_fit();

    PVVector.clear();
    PVVector.shrink_to_fit();
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
//   | |              ┌╋             |2| |
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
    auto APDLV = new G4LogicalVolume(APDBox, G4Material::GetMaterial("vacuum"), fCALName,
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
    if (fOptical) {
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
//   | |              ┌╋             |3| |
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
    if (fOptical) {
        new G4LogicalBorderSurface(fCALName + "_WrapSurface", CaloPV, WrapPV, dControl->Wrap_Surface);
        new G4LogicalBorderSurface(fCALName + "_APDGlueSurface", CaloPV, APDPV, dControl->APD_Surface);
    }
}

// Volume relationship:
//   0 Wrap
//   └-1 Calo
//     ├-2 WLS fiber
//     └-3 SiPM
//                   ->|               |<- CaloZHalfLength + WrapZHalfLength
// ->| |<-             |           ->| |<- WrapZHalfLength
//   ┌---------------------------------┐
//   |0┌-----------------------------┐ |
//   | | 1                           | |
//   | |========2===================3╡ |
//   | |                           ->║<- APDZHalfLength
//   | └-----------------------------┘ |
//   └-------------------------------|-┘
//                   ->|             |<- CaloZHalfLength
void CALConstruct::CalWLSUnitConstruct() {

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
    fAbsLV = AbsLV;

    if (fVis) {
        fVis->SetVisibility(true);
        AbsLV->SetVisAttributes(fVis);
    } else AbsLV->SetVisAttributes(G4VisAttributes::GetInvisible());
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
    CalUnit1Construct();

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
                                          fAbsLV,
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

