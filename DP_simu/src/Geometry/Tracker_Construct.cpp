//
// Migrated by Xuliang Zhu on 12/05/20.
//

#include "Geometry/Tracker_Construct.h"

/// \brief Define Parameters of Trackers
/// \param[in] type
/// \param[in] preset  0 = Tagging Tracker; 1 = Recoil Tracker
/// \param[in] Target_Size  A back-up input. Only used when user don't input Tracker geometry.
void Tracker_Construct::DefineParameters(const G4int type , const G4double Trk_Tar_Dis ,
                                         const G4ThreeVector Target_Size
                                         = G4ThreeVector(10 * cm, 20 * cm, 350 * um)) {
    // Migrate from: DetectorConstruction::DefineParameters
    assert(Size_Tracker.size() == Pos_Tracker.size());

    if (Tracker_Mat == nullptr) Tracker_Mat = G4Material::GetMaterial("G4_Si");
    if (TrackerRegion_Mat == nullptr) TrackerRegion_Mat = G4Material::GetMaterial("vacuum");

    switch(type) {
        case dNone:
            break;

        case dTagging:

            /// If user don't input geometry, built-in Tagging Tracker geometry will be set.
            if (!Size_Tracker.size()) {
                for (int i = 0; i < 7; i++) {
                    Size_Tracker.push_back(G4ThreeVector(Target_Size.x(), Target_Size.y(), 0.1 * mm));
                    Pos_Tracker.push_back(G4ThreeVector(0 * cm, 0 * cm, (-30. + i * 10.) * cm));
                }
            }

            No_Tracker = Size_Tracker.size();

            /// Size and Position of Tracer Region
            
            Size_TrackerRegion = G4ThreeVector(
                2.0 * Size_Tracker[0].x(),
                2.0 * Size_Tracker[0].y(),
                Pos_Tracker[No_Tracker - 1].z() - Pos_Tracker[0].z() + 2.0 * No_Tracker * Size_Tracker[0].z()
            );

            Pos_TrackerRegion = G4ThreeVector(
                0 * cm,
                0 * cm,
                Pos_Tracker[0].z() - Trk_Tar_Dis - (Size_Tracker[0].z() + Target_Size.z()) / 2
            );

            break;
        case dRecoil:

            /// If user don't input geometry, built-in Recoil Tracker geometry will be set.
            if (!Size_Tracker.size()) {
                for (int i = 0; i < 6; i++) {
                    Size_Tracker.push_back(G4ThreeVector(Target_Size.x(), 20 * cm, 0.1 * mm));
                }
                Pos_Tracker.push_back(G4ThreeVector(0 * cm, 0 * cm, -86.25 * mm));
                Pos_Tracker.push_back(G4ThreeVector(0 * cm, 0 * cm, -71.25 * mm));
                Pos_Tracker.push_back(G4ThreeVector(0 * cm, 0 * cm, -55.25 * mm));
                Pos_Tracker.push_back(G4ThreeVector(0 * cm, 0 * cm, -40.25 * mm));
                Pos_Tracker.push_back(G4ThreeVector(0 * cm, 0 * cm, -4.25 * mm));
                Pos_Tracker.push_back(G4ThreeVector(0 * cm, 0 * cm, -86.25 * mm));
            }

            No_Tracker = Size_Tracker.size();

            /// Size and Position of Recoil Tracker Region

            Size_TrackerRegion = G4ThreeVector(
                2.0 * Size_Tracker[No_Tracker - 1].x(),
                2.0 * Size_Tracker[No_Tracker - 1].y(),
                17.25 * cm + 2 * No_Tracker * Size_Tracker[No_Tracker - 1].z()
            );

            Pos_TrackerRegion = G4ThreeVector(
                0 * cm,
                0 * cm,
                0.5 * Size_TrackerRegion.z() + 7.5 * mm + 0.5 * 350 * um
            );
            
            break;

        default:
            break;
    }


}

bool Tracker_Construct::Build(G4int type, G4LogicalVolume *World_LV, RootManager *fRootMng, G4bool fCheckOverlaps) {
    // Migrate from: DectectorCoinstruction::DefineTagTracker / DetecotrConstruction::DefineRecTracker
    switch (type) {
        case dNone:
            return false;
        case dTagging:
            break;
        case dRecoil:
            break;
        default:
            break;
    }

    auto TrackerRegion_Box = new G4Box(
        (type == dTagging ? "TagTrk" : "RecTrk"),
        Size_TrackerRegion.x() / 2.,
        Size_TrackerRegion.y() / 2.,
        Size_TrackerRegion.z() / 2.
    );
    TrackerRegion_LV = new G4LogicalVolume(
        TrackerRegion_Box, TrackerRegion_Mat, 
        (type == dTagging ? "TAGTrk" : "RECTrk"),
        nullptr, nullptr, nullptr
    );
    new G4PVPlacement(
        nullptr, Pos_TrackerRegion, TrackerRegion_LV, 
        (type == dTagging ? "TAGTrk" : "RECTrk"),
        World_LV, false, 0, fCheckOverlaps
    );

    auto Tracker1 = new TrkConstruct(
        (type == dTagging ? "TagTrk1" : "RecTrk1"),
        TrackerRegion_LV, 0, fCheckOverlaps
    );

    Tracker1->SetRotation(Tracker1_Rotation);
    Tracker1->SetTrkMaterial(Tracker_Mat);
    Tracker1->SetVis(new G4VisAttributes(G4Colour(Tracker1_Color[0], Tracker1_Color[1], Tracker1_Color[2])));
    Tracker1->LinearPlacement(No_Tracker, &Size_Tracker[0], &Pos_Tracker[0]);

    Tracker_LV1 = Tracker1->GetTrkLVVector();

    auto Tracker2 = new TrkConstruct(
        (type == dTagging ? "TagTrk2" : "RecTrk2"),
        TrackerRegion_LV, 0, fCheckOverlaps
    );
    Tracker2->SetZMove(Size_Tracker[0].z());
    Tracker2->SetRotation(Tracker2_Rotation);
    Tracker2->SetTrkMaterial(Tracker_Mat);
    Tracker2->SetVis(new G4VisAttributes(G4Color(Tracker2_Color[0], Tracker2_Color[1], Tracker2_Color[2])));
    Tracker2->LinearPlacement(No_Tracker, &Size_Tracker[0], &Pos_Tracker[0]);

    Tracker_LV2 = Tracker2->GetTrkLVVector();

    return true;
}

bool Tracker_Construct::BuildSDandField(G4int type, RootManager *fRootMng) {
    //Migrate from: DetectorConstruction::CionstructSDandField

    /// Constrtruct Magnetic Field.
    
    G4MagneticField *TrackerMagField;
    TrackerMagField = new G4UniformMagField(G4ThreeVector(0., Tracker_MagField_y, 0.));
    auto *TrackerFieldMng = new G4FieldManager();
    TrackerFieldMng->SetDetectorField(TrackerMagField);
    TrackerFieldMng->CreateChordFinder(TrackerMagField);

    TrackerRegion_LV->SetFieldManager(TrackerFieldMng, allLocal);

    /// Construct Sensitive Detector

    auto *TrackerSD1 = new DetectorSD(
        0,
        (type == dTagging ? "TagTrk1" : "RecTrk1"),
         G4ThreeVector(1, 1, No_Tracker), fRootMng
    );
    G4SDManager::GetSDMpointer()->AddNewDetector(TrackerSD1);
    for (itr_LV = Tracker_LV1.begin(); itr_LV != Tracker_LV1.end(); itr_LV++)
        (*itr_LV)->SetSensitiveDetector(TrackerSD1);

    auto *TrackerSD2 = new DetectorSD(
        0,
        (type == dTagging ? "TagTrk2" : "RecTrk2"),
        G4ThreeVector(1, 1, No_Tracker), fRootMng
    );
    G4SDManager::GetSDMpointer()->AddNewDetector(TrackerSD2);
    for(itr_LV = Tracker_LV2.begin(); itr_LV != Tracker_LV2.end(); itr_LV++)
        (*itr_LV)->SetSensitiveDetector(TrackerSD2);

    return true;
}

/// Setter

void Tracker_Construct::SetTrackerMagField(G4double in) {
    Tracker_MagField_y = in;

    allLocal = false;
    
    G4MagneticField *TrackerMagField = new G4UniformMagField(G4ThreeVector(0., Tracker_MagField_y, 0.));
    auto *TrackerFieldMng = new G4FieldManager();
    TrackerFieldMng->SetDetectorField(TrackerMagField);
    TrackerFieldMng->CreateChordFinder(TrackerMagField);
    
    TrackerRegion_LV->SetFieldManager(TrackerFieldMng, allLocal);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
}