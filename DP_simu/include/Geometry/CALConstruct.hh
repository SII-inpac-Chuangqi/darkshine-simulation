
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

class CALConstruct {
public:
    CALConstruct(const G4String&, G4LogicalVolume *, G4int, G4bool, G4bool, G4bool, G4bool);

    CALConstruct(const CALConstruct &);

    virtual ~CALConstruct();

    void ConstructLV();

    /// \brief main construct
    G4ThreeVector
    Construct(G4LogicalVolume *boxLV, G4LogicalVolume *WrapLV, G4LogicalVolume *aboxLV,
              double z_angle = 0.);

    G4ThreeVector MatrixPlacement(G4int, G4int, G4int, const G4ThreeVector &);

    void MatrixPlacementXYRemoved(G4int, G4int, G4int, const G4ThreeVector &, G4int, G4int);

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

    void SetSizeX(G4double in) { fSizeX = in; };

    void SetSizeY(G4double in) { fSizeY = in; };

    void SetSizeZ(G4double in) { fSizeZ = in; };

    void SetPosX(G4double in) { fPosX = in; };

    void SetPosY(G4double in) { fPosY = in; };

    void SetPosZ(G4double in) { fPosZ = in; };

    void SetWrapSizeX(G4double in) { fWrapSizeX = in; };

    void SetWrapSizeY(G4double in) { fWrapSizeY = in; };

    void SetWrapSizeZ(G4double in) { fWrapSizeZ = in; };

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
        fSizeX = x;
        fSizeY = y;
        fSizeZ = z;
    };

    void SetPosXYZ(G4double x, G4double y, G4double z) {
        fPosX = x;
        fPosY = y;
        fPosZ = z;
    };

    void SetWrapSizeXYZ(G4double x, G4double y, G4double z) {
        fWrapSizeX = x;
        fWrapSizeY = y;
        fWrapSizeZ = z;
    };

    void SetAPDSize(const G4ThreeVector &apd, const G4ThreeVector &glue) {
        aSizeX = apd.x();
        aSizeY = apd.y();
        aSizeZ = apd.z();
        gSizeX = glue.x();
        gSizeY = glue.y();
        gSizeZ = glue.z();
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
    G4double fSizeX{0};
    G4double fSizeY{0};
    G4double fSizeZ{0};
    G4double fPosX{0};
    G4double fPosY{0};
    G4double fPosZ{0};

    // For Wrap
    G4double fWrapSizeX{0};
    G4double fWrapSizeY{0};
    G4double fWrapSizeZ{0};

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
    G4double wSizeX{0.};
    G4double wSizeY{0.};
    G4double wSizeZ{0.};
    G4double aSizeX{0.};
    G4double aSizeY{0.};
    G4double aSizeZ{0.};
    G4double gSizeX{0.};
    G4double gSizeY{0.};
    G4double gSizeZ{0.};
    G4double wPosX{0.};
    G4double wPosY{0.};
    G4double wPosZ{0.};
    G4Material *APD_Material{nullptr};
    G4Material *Glue_Material{nullptr};

    G4RotationMatrix *HepRot{nullptr};

    // Unity LV
    // 0: all Z, 1: XYcrossing
    G4LogicalVolume *fCaloLV{nullptr}; // Core Detector Region
    G4LogicalVolume *fWrapLV{nullptr}; // Wrapper
    G4LogicalVolume *fAPDWLV{nullptr}; // APD world
    G4LogicalVolume *fAbsLV{nullptr}; // Absorber world

    // For Matrix Placement Output
    std::vector<G4LogicalVolume *> fCaloLVVector{};
    std::vector<G4LogicalVolume *> fWrapLVVector{};

    // For Memory Clean
    std::vector<G4PVPlacement*> PVVector;
};

#endif
