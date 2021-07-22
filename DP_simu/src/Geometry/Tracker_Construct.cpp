//
// Migrated by Xuliang Zhu on 12/05/20.
//

#include "Geometry/Tracker_Construct.h"

/// \brief Define Parameters of Trackers
/// \param[in] type
/// \param[in] preset  0 = Tagging Tracker; 1 = Recoil Tracker
/// \param[in] Target_Size
void Tracker_Construct::DefineParameters(Tracker_Type type) {

    // Global tracker parameter
    Tracker_Mat = dControl->Tracker_Mat;
    TrackerRegion_Mat = dControl->TrackerRegion_Mat;
    Tracker1_Color = dControl->Tracker1_Color;
    Tracker2_Color = dControl->Tracker2_Color;

    switch (type) {
        case dNone:
            break;

        case dTagging:
            Size_Tracker = dControl->tag_Size_Tracker;
            Pos_Tracker = dControl->tag_Pos_Tracker;
            StripN_Tracker = dControl->tag_Tracker_Strip_N;
            Strip_Angle_Gap_Tracker = dControl->tag_Tracker_Angle_Gap;
            No_Tracker = dControl->tag_No_Tracker;
            Size_TrackerRegion = dControl->tag_Size_TrackerRegion;
            Pos_TrackerRegion = dControl->tag_Pos_TrackerRegion;

            Tracker_MagField = dControl->tag_Tracker_MagField;

            break;
        case dRecoil:
            Size_Tracker = dControl->rec_Size_Tracker;
            Pos_Tracker = dControl->rec_Pos_Tracker;
            StripN_Tracker = dControl->rec_Tracker_Strip_N;
            Strip_Angle_Gap_Tracker = dControl->rec_Tracker_Angle_Gap;
            No_Tracker = dControl->rec_No_Tracker;
            Size_TrackerRegion = dControl->rec_Size_TrackerRegion;
            Pos_TrackerRegion = dControl->rec_Pos_TrackerRegion;

            Tracker_MagField = dControl->rec_Tracker_MagField;

            break;

        default:
            return;
    }
}

bool Tracker_Construct::Build(G4int type, G4LogicalVolume *World_LV, G4bool fCheckOverlaps) {

    if (type == dNone) return false;

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

    auto Tracker = new TrkConstruct(
            (type == dTagging ? "TagTrk" : "RecTrk"),
            TrackerRegion_LV, 0, fCheckOverlaps
    );

    Tracker->SetTrkMaterial(Tracker_Mat);
    Tracker->SetVis1(new G4VisAttributes(G4Colour(Tracker1_Color[0], Tracker1_Color[1], Tracker1_Color[2])));
    Tracker->SetVis2(new G4VisAttributes(G4Colour(Tracker2_Color[0], Tracker2_Color[1], Tracker2_Color[2])));
    Tracker->TrackerPlacement(No_Tracker, &Size_Tracker[0], &Pos_Tracker[0], StripN_Tracker, &Strip_Angle_Gap_Tracker[0]);

    TrackerStrip_LV = Tracker->GetStripLVVector();

    return true;
}

bool Tracker_Construct::BuildSDandField(G4int type) {
    //Migrate from: DetectorConstruction::CionstructSDandField

    /// Constrtruct Magnetic Field.

    G4MagneticField *TrackerMagField;
    TrackerMagField = new G4UniformMagField(G4ThreeVector(Tracker_MagField));
    auto *TrackerFieldMng = new G4FieldManager();
    TrackerFieldMng->SetDetectorField(TrackerMagField);
    TrackerFieldMng->CreateChordFinder(TrackerMagField);

    TrackerRegion_LV->SetFieldManager(TrackerFieldMng, allLocal);

    /// Construct Sensitive Detector

    auto *SMTrackerSD = new DetectorSD(
            0,
            (type == dTagging ? "TagTrk" : "RecTrk"),
            G4ThreeVector(1, 1, No_Tracker)
    );
    G4SDManager::GetSDMpointer()->AddNewDetector(SMTrackerSD);
    for (auto LV : TrackerStrip_LV)
        LV->SetSensitiveDetector(SMTrackerSD);

    return true;
}
