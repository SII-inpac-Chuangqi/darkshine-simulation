#include "Geometry/TrkConstruct.hh"

#include <iterator>
#include <utility>

#include "G4Box.hh"

// TODO: clean PV, HepRot pointers

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrkConstruct::TrkConstruct(G4String TrkName,
                           G4LogicalVolume *MotherVolume,
                           G4int CopyNo,
                           G4bool CheckOverlap) {
    fTrkName = std::move(TrkName);
    fMotherVolume = MotherVolume;
    fCopyNo = CopyNo;
    fCheckOverlap = CheckOverlap;

    fSizeX = 0.;
    fSizeY = 0.;
    fSizeZ = 0.;
    fPosX = 0.;
    fPosY = 0.;
    fPosZ = 0.;
    fAngle1 = 0.;
    fAngle2 = 0.;

    fAngle = 0.;
    fZMove = 0.;

    fVis = nullptr;
    fTrkMaterial = nullptr;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrkConstruct::TrkConstruct(const TrkConstruct &in) {
    fTrkName = in.fTrkName;
    fMotherVolume = in.fMotherVolume;
    fCopyNo = in.fCopyNo;
    fType = in.fType;
    fCheckOverlap = in.fCheckOverlap;
    fSizeX = in.fSizeX;
    fSizeY = in.fSizeY;
    fSizeZ = in.fSizeZ;
    fPosX = in.fPosX;
    fPosY = in.fPosY;
    fPosZ = in.fPosZ;
    fTrkMaterial = in.fTrkMaterial;
    fVis = in.fVis;
    fTrkLVVector = in.fTrkLVVector;
    fBlockLVVector = in.fBlockLVVector;
    fStripBlockN = in.fStripBlockN;
    fAngle = in.fAngle;
    fZMove = in.fZMove;
    fPos1 = in.fPos1;
    fPos2 = in.fPos2;
    fTrkLV = in.fTrkLV;
    fBlockLV = in.fBlockLV;
    fAngle1 = in.fAngle1;
    fAngle2 = in.fAngle2;
    fStripNum = in.fStripNum;
    fPixelNum = in.fPixelNum;
    fStripSizeX = in.fStripSizeX;
    fStripSizeY = in.fStripSizeY;
    fStripSizeZ = in.fStripSizeZ;
    fStripPosX = in.fStripPosX;
    fStripPosY = in.fStripPosY;
    fStripPosZ = in.fStripPosZ;
    fStripDistanceX = in.fStripDistanceX;
    fStripVis = in.fStripVis;
    fStripLV = in.fStripLV;
    fStripLVVector = in.fStripLVVector;
    PVVector = in.PVVector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TrkConstruct::~TrkConstruct() {
    delete fVis;
    delete fStripVis;
    delete fStripLV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector TrkConstruct::BoxConstruct() {
    auto TotalHalfSize = G4ThreeVector(0, 0, 0);

    if (!fSizeX || !fSizeY || !fSizeZ) {
        G4cout << fTrkName << " Construction Error: at least size of one dimension is zero." << G4endl;
        return TotalHalfSize;
    }
    // box LV
    auto box = new G4Box(fTrkName + "_Box", (fSizeX + eps) / 2., fSizeY / 2., fSizeZ / 2.);
    auto boxLV = new G4LogicalVolume(box, fTrkMaterial, fTrkName + "_LV", nullptr, nullptr, nullptr);

    fTrkLV = boxLV;
    fTrkLVVector.emplace_back(boxLV);

    if (fVis) {
        fVis->SetVisibility(true);
        boxLV->SetVisAttributes(fVis);
    } else boxLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    return TotalHalfSize;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector TrkConstruct::BoxPlacement() {
    auto TotalSize = G4ThreeVector(0, 0, 0);

    // Placement of Tracker
    auto pos = G4ThreeVector(fPosX, fPosY, fPosZ + fZMove);
    TotalSize = G4ThreeVector(fSizeX, fSizeY, fSizeZ);
    BoxConstruct();
    new G4PVPlacement(nullptr, pos, fTrkLV, fTrkName + "_PV", fMotherVolume, false, fCopyNo, fCheckOverlap);

    return TotalSize;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Volume relationship:
// 2 Outline (Trk)
// └-1 SMTBlock
//   └-0 Silicon micro-strip (Strip)
//  (└-0 Silicon pixel (Pixel) )
G4ThreeVector TrkConstruct::SMTConstruct() {
    auto TotalHalfSize = G4ThreeVector(0, 0, 0);
    // check consistency
    if (!fSizeX || !fSizeY || !fSizeZ) {
        G4cout << "[Error] ==> " << fTrkName << " construction error: size of at least one dimension is zero." << G4endl;
        return TotalHalfSize;
    }
    // outline
    auto TrkBox = new G4Box(fTrkName + "_Box", (fSizeX + 2 * eps) / 2., (fSizeY + 2 * eps) / 2., (fSizeZ + 2 * eps) / 2.);
    auto TrkLV = new G4LogicalVolume(TrkBox,
                                     G4Material::GetMaterial("vacuum"),
                                     fTrkName + "_LV",
                                     nullptr, nullptr, nullptr);

    fTrkLV = TrkLV;
    fTrkLVVector.emplace_back(TrkLV);
    if (fVis) {
        fVis->SetVisibility(true);
        TrkLV->SetVisAttributes(fVis);
    } else TrkLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    // SMT Block
    assert(fStripNum % fStripBlockN == 0 && "Strip number per Block number must be a common divisor of Strip Number.");
    auto BlockBox = new G4Box(fTrkName + "_Block_Box", (fStripBlockN * fStripDistanceX) * 0.5 - 0.25 * fStripGap, (fSizeY + eps) * 0.5, (fSizeZ + eps) * 0.5);
    auto BlockLV = new G4LogicalVolume(BlockBox,
                                       G4Material::GetMaterial("vacuum"),
                                       fTrkName + "_Block_LV",
                                       nullptr, nullptr, nullptr);
    fBlockLV = BlockLV;
    fBlockLVVector.emplace_back(BlockLV);
#ifndef DEBUG
    BlockLV->SetVisAttributes(G4VisAttributes::GetInvisible());
#endif

    // Silicon micro-strip
    G4cout << "[INFO] ==> TrkConstruct::SMTConstruct() fPixelNum: " << fPixelNum << G4endl;
    auto StripBox = new G4Box(fTrkName + "_Strip_Box", fStripSizeX / 2., fStripSizeY / 2., fStripSizeZ / 2.);
    auto StripLV = new G4LogicalVolume(StripBox, fTrkMaterial, fTrkName + "_Strip_LV",
                                       nullptr, nullptr, nullptr);
    fStripLV = StripLV;
    fStripLVVector.emplace_back(StripLV);
#ifdef DSIMU_DEBUG
    fStripVis = fVis;
#endif
    if (fStripVis)
    {
        fStripVis->SetVisibility(true);
        StripLV->SetVisAttributes(fStripVis);
    } else StripLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    /// construct nphysical volume
    // place strip
    TotalHalfSize = G4ThreeVector(0.5 * fStripBlockN * fStripDistanceX,
                                  0.5 * fSizeY,
                                  0.5 * fSizeZ);
    fStripPosY = 0;
    fStripPosZ = 0;
    // G4PVPlacement* StripPV = nullptr;
    G4int fStripCopyNo = 0;
    for (int i = 0; i < fStripBlockN; i++) {
        fStripPosX = -1 * TotalHalfSize.x() + (i + 0.5) * fStripDistanceX;
        auto StripPV = new G4PVPlacement(nullptr,
                                         G4ThreeVector(fStripPosX, fStripPosY, fStripPosZ),
                                         fStripLV,
                                         fTrkName + "_Strip_PV",
                                         fBlockLV,
                                         false,
                                         fStripCopyNo,
                                         fCheckOverlap);
        PVVector.emplace_back(StripPV);
        fStripCopyNo++;
    }

    TotalHalfSize = G4ThreeVector(0.5 * fSizeX,
                                  0.5 * fSizeY,
                                  0.5 * fSizeZ);
    G4double fBlockPosX = 0;
    G4double fBlockPosY = 0;
    G4double fBlockPosZ = 0;

    G4int fBlockCopyNo = 0;
    for (int i = 0; i < fStripNum / fStripBlockN; i++) {
        fBlockPosX = -1 * TotalHalfSize.x() + (i + 0.5) * fStripBlockN * fStripDistanceX;
        auto BlockPV = new G4PVPlacement(nullptr,
                                         G4ThreeVector(fBlockPosX, fBlockPosY, fBlockPosZ),
                                         fBlockLV,
                                         fTrkName + "_Block_PV",
                                         fTrkLV,
                                         false,
                                         fBlockCopyNo,
                                         fCheckOverlap);
        PVVector.emplace_back(BlockPV);
        fBlockCopyNo++;
    }

    return TotalHalfSize;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector TrkConstruct::LinearPlacement(G4int zNo,
                                            G4ThreeVector *SizeVec,
                                            G4ThreeVector *PosVec,
                                            std::vector<G4int> StripNVec,
                                            std::vector<G4int> PixelNVec,
                                            G4ThreeVector *AngleGapVec,
                                            G4int stripBlockN) {
    auto TotalHalfSize = G4ThreeVector(0, 0, 0);

    PVVector.reserve(zNo*2);

    G4PVPlacement *UnitPV = nullptr;
    // construct Tracker1s
    for (int k = 0; k < zNo; k++) {
        auto CurSizeVec = *(SizeVec + k);
        auto CurPosVec = *(PosVec + k);
        auto CurStripN = StripNVec.at(k);
        auto CurPixelN = PixelNVec.at(k);
        auto CurAngleGapVec = *(AngleGapVec + k);

        G4cout << "[INFO] ==> TrkConstruct::LinearPlacement() CurPixelN: " << CurPixelN << G4endl;
        G4cout << "[INFO] ==> TrkConstruct::LinearPlacement() build_silicon_micro_strip: " << dControl->build_silicon_micro_strip << G4endl;
        G4cout << "[INFO] ==> TrkConstruct::LinearPlacement() build_silicon_pixel: " << dControl->build_silicon_pixel << G4endl;

        SetSizeXYZ(CurSizeVec);
        SetPosXYZ(CurPosVec);
        SetStrip_Param(CurStripN, CurPixelN, CurAngleGapVec);
        SetStrip_Block_N(stripBlockN);

        if (! dControl->build_silicon_micro_strip ) {
            fStripNum = 1;
            fStripBlockN = 1;
            fStripDistanceX = fSizeX;
            fStripGap = eps;
            fStripSizeX = fSizeX - fStripGap;
        }

        if (! dControl->build_silicon_pixel ) {
            fPixelNum = 1;
        }

        auto HepRot1 = new G4RotationMatrix();
        HepRot1->rotateZ(fAngle1);
        auto HepRot2 = new G4RotationMatrix();
        HepRot2->rotateZ(fAngle2);

        fZMove = 0.5 * fSizeZ + 2 * eps;
        fPos1 = G4ThreeVector(fPosX, fPosY, fPosZ - fZMove);
        fPos2 = G4ThreeVector(fPosX, fPosY, fPosZ + fZMove);


        G4ThreeVector CurColor = dControl->Tracker1_Color;
        fVis = fVis1;
        //fStripVis = fVis1;
        SMTConstruct();
        UnitPV = new G4PVPlacement(HepRot1,
                                   fPos1,
                                   fTrkLV,
                                   fTrkName + "1_PV",
                                   fMotherVolume,
                                   false,
                                   fCopyNo,
                                   fCheckOverlap);
        PVVector.emplace_back(UnitPV);

        fVis = fVis2;
        //fStripVis = fVis2;
        SMTConstruct();
        UnitPV = new G4PVPlacement(HepRot2,
                                   fPos2,
                                   fTrkLV,
                                   fTrkName + "2_PV",
                                   fMotherVolume,
                                   false,
                                   fCopyNo,
                                   fCheckOverlap);
        PVVector.emplace_back(UnitPV);
        fCopyNo++;
    }

    PVVector.shrink_to_fit();

    return TotalHalfSize;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrkConstruct::SetStrip_Param(const G4int &stripN, const G4int &pixelN, const G4ThreeVector &angleGap) {
    fStripNum = stripN;
    fPixelNum = pixelN;
    fStripDistanceX = fSizeX / stripN;
    fStripSizeX = fStripDistanceX - angleGap.z();
    fStripSizeY = fSizeY;
    fStripSizeZ = fSizeZ;
    fStripGap = angleGap.z();

    fAngle1 = angleGap.x();
    fAngle2 = angleGap.y();
}
