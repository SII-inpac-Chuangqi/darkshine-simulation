//
// Migrated by Xuliang Zhu on 12/05/20.
//

#ifndef DSIMU_TRACKER_CONSTRUCT_H
#define DSIMU_TRACKER_CONSTRUCT_H

#include "DP_simu/TrkConstruct.hh"
#include "DP_simu/DetectorConstruction.hh"
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

    //Tracker_Construct();

    //~Tracker_Construct();

    void DefineParameters(Tracker_Type type, G4double Trk_Tar_Dis , const G4ThreeVector& Target_Size);

    bool Build(G4int type, G4LogicalVolume *World_LV, RootManager *fRootMng, G4bool fCheckOverlaps);

    bool BuildSDandField(G4int type, RootManager *fRootMng);

    /// Setter
    
    //void SetTrackerMat(G4String in) { Tracker_Mat = G4Material::GetMaterial(in); };
    //void SetTrackerRegionMat(G4String in) { TrackerRegion_Mat = G4Material::GetMaterial(in); };
    void AddNewTrackerSize(const G4ThreeVector& in) { Size_Tracker.push_back(in); }; // Set the Size of the new Tracker.
    void AddNewTrackerPos(const G4ThreeVector& in) { Pos_Tracker.push_back(in); }; // Set the Postion of the new Tracker.
    void DelTracker(G4bool in = true) { if(in) {
        Size_Tracker = {};
        Pos_Tracker = {}; 
        } 
    };
    void SetTracker1Rotation(G4double in) { Tracker1_Rotation = in; };
    void SetTracker2Rotation(G4double in) { Tracker2_Rotation = in; };
    void SetTracker1Color(const G4ThreeVector& in) {
        Tracker1_Color[0] = in.x();
        Tracker1_Color[1] = in.y();
        Tracker1_Color[2] = in.z();
    };
    void SetTracker2Color(const G4ThreeVector& in) {
        Tracker2_Color[0] = in.x();
        Tracker2_Color[1] = in.y();
        Tracker2_Color[2] = in.z();
    };
    void SetTrackerMagField(G4double in);

    /// Getter

    G4ThreeVector GetSizeTrkRegion() {return Size_TrackerRegion;};
    G4ThreeVector GetPosTrkRetion() {return Pos_TrackerRegion;};

    double eps = 1*um;
private:

    /// EM Field

    G4double MagField_y{};

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

    G4double Tracker1_Rotation = 0. * radian;
    G4double Tracker2_Rotation = 0.1 * radian;
    G4double Tracker1_Color[3] {0.5, 0.5, 0.};
    G4double Tracker2_Color[3] {0.5, 0.5, 0.};
    std::vector<G4LogicalVolume *> Tracker_LV1;
    std::vector<G4LogicalVolume *> Tracker_LV2;

    /// Tracker build SD and Field parameters

    /// Magnetic Field

    G4bool allLocal = true;
    G4double Tracker_MagField_y{};

    /// Sensitive Detector

    std::vector<G4LogicalVolume *>::iterator itr_LV;
};

#endif //DSIMU_TRACKER_CONSTRUCT_H
