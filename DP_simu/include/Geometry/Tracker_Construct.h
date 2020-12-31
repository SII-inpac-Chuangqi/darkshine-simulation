//
// Migrated by Xuliang Zhu on 12/05/20.
//

#ifndef DSIMU_TRACKER_CONSTRUCT_H
#define DSIMU_TRACKER_CONSTRUCT_H

#include "DP_simu/TrkConstruct.hh"
#include "DP_simu/DetectorConstruction.hh"
#include "Control.h"

#include "G4FieldManager.hh"
#include "G4RunManager.hh"

#include <cassert>

#include <vector>

enum Tracker_Type {dNone, dTagging, dRecoil};

/// Class description:
///  Users add new tracker by G4UIcmdWith3VectorAndUnit.
///  At first, there is no Tracker. User add each new Tracker
///  by repeatedly input the size and postion.
/// Example macro:
///  /DP/det/AddTrackerSize 0 * cm 0 * cm 0.1 * mm
class Tracker_Construct {
public:

    Tracker_Construct() = default;
    virtual ~Tracker_Construct() = default;

    void DefineParameters(Tracker_Type type);

    bool Build(G4int type, G4LogicalVolume *World_LV, G4bool fCheckOverlaps);

    bool BuildSDandField(G4int type, RootManager *fRootMng);

    double eps = dControl->eps;

private:

    /// Tracker parameters
    G4Material *Tracker_Mat = nullptr; 
    G4Material *TrackerRegion_Mat = nullptr; 
    std::vector<G4ThreeVector> Size_Tracker{};
    std::vector<G4ThreeVector> Pos_Tracker{};
    G4int No_Tracker{};
    G4ThreeVector Size_TrackerRegion;
    G4ThreeVector Pos_TrackerRegion;
    G4LogicalVolume *TrackerRegion_LV{};

    /// Tracker build parameters
    G4double Tracker1_Rotation{};
    G4double Tracker2_Rotation{};
    G4ThreeVector Tracker1_Color;
    G4ThreeVector Tracker2_Color;
    std::vector<G4LogicalVolume *> Tracker_LV1;
    std::vector<G4LogicalVolume *> Tracker_LV2;

    /// Magnetic Field
    G4bool allLocal = true;
    G4ThreeVector Tracker_MagField;

};

#endif //DSIMU_TRACKER_CONSTRUCT_H
