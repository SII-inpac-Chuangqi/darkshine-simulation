
#ifndef CALConstruct_h
#define CALConstruct_h 1

#include "DP_simu/DetectorSD.hh"

#include "globals.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4PVPlacement.hh"

#include <utility>
#include <vector>

/// Class decription:
class CALConstruct {
public:
    CALConstruct(const G4String&, G4LogicalVolume *, G4int, G4bool, G4bool, G4bool, G4bool);

    CALConstruct(const CALConstruct &);

    virtual ~CALConstruct();

    // Construct Logical Volume and child Physical Volume

    void CalUnit1Construct();
    void CalUnit2Construct();
    void AbsorberUnitConstruct();
    void ConstructLV();

    // Physical Volume placement

    void UnitPlacement(G4LogicalVolume *unitLV, G4RotationMatrix *fRot = nullptr);

    G4ThreeVector
    Construct(G4LogicalVolume *boxLV, G4LogicalVolume *WrapLV, G4LogicalVolume *aboxLV,
              double z_angle = 0.);

    G4ThreeVector MatrixPlacement(G4int, G4int, G4int, const G4ThreeVector &);

    //void MatrixPlacementXYRemoved(G4int, G4int, G4int, const G4ThreeVector &, G4int, G4int);

    void MatrixPlacementXYwithAbsorber(G4int, G4int, G4int, const G4ThreeVector &, G4double, G4Material *);

    // initializing the output LV
    void CleanLV() {
        fCaloLVVector.clear();
        fWrapLVVector.clear();
    };

    // Set Methods
    void SetType(G4bool in) { fType = in; };

    void SetWrap(G4bool in) { fWrap = in; };

    void SetCheckOverlap(G4bool in) { fCheckOverlap = in; };

    void SetSizeX(G4double in) { CaloXHalfLength = in; };

    void SetSizeY(G4double in) { CaloYHalfLength = in; };

    void SetSizeZ(G4double in) { CaloZHalfLength = in; };

    void SetPosX(G4double in) { UnitPosX = in; };

    void SetPosY(G4double in) { UnitPosY = in; };

    void SetPosZ(G4double in) { UnitPosZ = in; };

    void SetWrapSizeX(G4double in) { WrapXHalfLength = in; };

    void SetWrapSizeY(G4double in) { WrapYHalfLength = in; };

    void SetWrapSizeZ(G4double in) { WrapZHalfLength = in; };

    void SetCopyNo(G4int in) { fCopyNo = in; };

    void SetCALName(G4String in) { fCALName = std::move(in); };

    void SetCALSD(DetectorSD *in) { fCALSD = in; };

    void SetVis(G4VisAttributes *in) { fVis = in; };

    void SetCALWrapSD(DetectorSD *in) { fCALWrapSD = in; };

    void SetWrapVis(G4VisAttributes *in) { fWrapVis = in; };

    void SetMotherVolume(G4LogicalVolume *in) { fMotherVolume = in; };

    void SetCALMaterial(G4Material *in) { fCALMaterial = in; };

    void SetWrapMaterial(G4Material *in) { fWrapMaterial = in; };

    void SetSizeXYZ(G4double x, G4double y, G4double z) {
        CaloXHalfLength = x;
        CaloYHalfLength = y;
        CaloZHalfLength = z;
    };

    void SetPosXYZ(G4double x, G4double y, G4double z) {
        UnitPosX = x;
        UnitPosY = y;
        UnitPosZ = z;
    };

    void SetWrapSizeXYZ(G4double x, G4double y, G4double z) {
        WrapXHalfLength = x;
        WrapYHalfLength = y;
        WrapZHalfLength = z;
    };

    void SetAPDSize(const G4ThreeVector &apd, const G4ThreeVector &glue) {
        APDXHalfLength = apd.x() * 0.5;
        APDYHalfLength = apd.y() * 0.5;
        APDZHalfLength = apd.z() * 0.5;
        GlueXHalfLength = glue.x() * 0.5;
        GlueYHalfLength = glue.y() * 0.5;
        GlueZHalfLength = glue.z() * 0.5;
    };

    void SetAPDMat(G4Material *apd, G4Material *glue) {
        APD_Material = apd;
        Glue_Material = glue;
    };

    std::vector<G4LogicalVolume *> GetCaloLVVector() { return fCaloLVVector; };

    std::vector<G4LogicalVolume *> GetWrapLVVector() { return fWrapLVVector; };

    double eps = 1 * um;

private:
    G4bool fType{false}; // 0: Absorber; 1: Calorimeter
    G4bool fWrap{false}; // 0: No Wrap around; 1: with Wrap
    G4bool fOptical{false}; // 0: No optical photon, fast; 1: Full simulation with optical photon
    G4bool fCheckOverlap{false};
    G4bool fRecordLV{false};
    G4bool ifAbsorber{false};

    // For Inner part
    G4double CaloXHalfLength{0};
    G4double CaloYHalfLength{0};
    G4double CaloZHalfLength{0};
    G4double UnitPosX{0};
    G4double UnitPosY{0};
    G4double UnitPosZ{0};
    G4VisAttributes *fCaloVis{nullptr};

    // For Wrap
    G4double WrapXHalfLength{0};
    G4double WrapYHalfLength{0};
    G4double WrapZHalfLength{0};

    G4int fCopyNo{0};
    G4String fCALName;

    G4LogicalVolume *fMotherVolume{nullptr};
    DetectorSD *fCALSD{nullptr};
    DetectorSD *fCALWrapSD{nullptr};
    G4VisAttributes *fVis{nullptr};
    G4VisAttributes *fWrapVis{nullptr};

    G4Material *fCALMaterial{nullptr};
    G4Material *fWrapMaterial{nullptr};

    // APD Stuff (Optical Photon)
    G4double ADPwXHalfLength{0.};
    G4double APDwYHalfLength{0.};
    G4double APDwZHalfLength{0.};
    G4double APDXHalfLength{0.};
    G4double APDYHalfLength{0.};
    G4double APDZHalfLength{0.};
    G4double GlueXHalfLength{0.};
    G4double GlueYHalfLength{0.};
    G4double GlueZHalfLength{0.};
    G4double APDPosX{0.};
    G4double APDPosY{0.};
    G4double APDPosZ{0.};
    G4Material *APD_Material{nullptr};
    G4Material *Glue_Material{nullptr};
    G4VisAttributes *fAPDVis{nullptr};

    // Absorber
    G4double AbsXHalfLength{0.};
    G4double AbsYHalfLength{0.};
    G4double AbsZHalfLength{0.};

    G4RotationMatrix *HepRot{nullptr};

    // Unity LV
    // 0: all Z, 1: XYcrossing
    G4LogicalVolume *fCaloLV{nullptr}; // Core Detector Region
    G4LogicalVolume *fWrapLV{nullptr}; // Wrapper
    G4LogicalVolume *fAPDWLV{nullptr}; // APD world
    G4LogicalVolume *fAbsLV{nullptr}; // Absorber world
    G4LogicalVolume *fOutlineLV{nullptr}; // outline of unit

    // For Matrix Placement Output
    std::vector<G4LogicalVolume *> fCaloLVVector{};
    std::vector<G4LogicalVolume *> fWrapLVVector{};

    // For Memory Clean
    std::vector<G4PVPlacement*> PVVector;
};

#endif
