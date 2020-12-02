
#ifndef CALConstruct_h
#define CALConstruct_h 1

#include "DetectorSD.hh"

#include "globals.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include <utility>
#include <vector>

class CALConstruct
{
  public:
    CALConstruct(G4String, G4LogicalVolume*, G4int, G4bool, G4bool, G4bool, G4bool);
    CALConstruct(const CALConstruct&);
    virtual ~CALConstruct();

    G4ThreeVector Construct();
    G4ThreeVector MatrixPlacement(G4int, G4int, G4int, const G4ThreeVector& );
    void          MatrixPlacementXYRemoved(G4int, G4int, G4int, G4ThreeVector, G4int, G4int);
    void          MatrixPlacementXYwithAbsorber( G4int, G4int, G4int, G4ThreeVector, G4double, G4Material*);

    // initializing the output LV
    void CleanLV() { fCaloLVVector.clear(); fWrapLVVector.clear(); };
    // Set Methods
    void SetType(G4bool in)             { fType = in; }; 
    void SetWrap(G4bool in)          { fWrap = in; }; 
    void SetCheckOverlap(G4bool in)     { fCheckOverlap = in; };
    void SetSizeX(G4double in)          { fSizeX = in; };
    void SetSizeY(G4double in)          { fSizeY = in; };
    void SetSizeZ(G4double in)          { fSizeZ = in; };
    void SetPosX(G4double in)           { fPosX  = in; };
    void SetPosY(G4double in)           { fPosY  = in; };
    void SetPosZ(G4double in)           { fPosZ  = in; };
    void SetWrapSizeX(G4double in)   { fWrapSizeX = in; };
    void SetWrapSizeY(G4double in)   { fWrapSizeY = in; };
    void SetWrapSizeZ(G4double in)   { fWrapSizeZ = in; };
    void SetCopyNo(G4int in)            { fCopyNo = in; };
    void SetCALName(G4String in)        { fCALName = std::move(in); };
    void SetCALSD(DetectorSD* in)    { fCALSD = in; };
    void SetVis(G4VisAttributes* in)    { fVis = in; };
    void SetCALWrapSD(DetectorSD* in)        { fCALWrapSD = in; };
    void SetWrapVis(G4VisAttributes* in)        { fWrapVis = in; };
    void SetMotherVolume(G4LogicalVolume* in)   { fMotherVolume = in; };
    void SetCALMaterial(G4Material* in)         { fCALMaterial = in; };
    void SetWrapMaterial(G4Material* in)     { fWrapMaterial = in; };

    void SetSizeXYZ(G4double x, G4double y, G4double z) { fSizeX = x; fSizeY = y; fSizeZ = z; };
    void SetPosXYZ(G4double x, G4double y, G4double z) { fPosX = x; fPosY = y; fPosZ = z; };
    void SetWrapSizeXYZ(G4double x, G4double y, G4double z) { fWrapSizeX = x; fWrapSizeY = y; fWrapSizeZ = z; };

    void SetAPDSize(const G4ThreeVector& apd, const G4ThreeVector& glue) {
        aSizeX = apd.x(); aSizeY = apd.y(); aSizeZ = apd.z(); 
        gSizeX = glue.x(); gSizeY = glue.y(); gSizeZ = glue.z(); 
    };
    void SetAPDMat( G4Material* apd, G4Material* glue) {
        APD_Material = apd; 
        Glue_Material = glue; 
    };

    std::vector<G4LogicalVolume*> GetCaloLVVector() { return fCaloLVVector; };
    std::vector<G4LogicalVolume*> GetWrapLVVector() { return fWrapLVVector; };

  private:
    G4bool fType; // 0: Absorber; 1: Calorimeter
    G4bool fWrap; // 0: No Wrap around; 1: with Wrap
    G4bool fOptical; // 0: No optical photon, fast; 1: Full simulation with optical photon
    G4bool fCheckOverlap;
    G4bool fRecordLV;
    G4bool ifAbsorber;

    // For Inner part
    G4double fSizeX;
    G4double fSizeY;
    G4double fSizeZ;
    G4double fPosX;
    G4double fPosY;
    G4double fPosZ;

    // For Wrap
    G4double fWrapSizeX;
    G4double fWrapSizeY;
    G4double fWrapSizeZ;

    G4int fCopyNo;
    G4String fCALName;

    G4LogicalVolume* fMotherVolume;
    DetectorSD* fCALSD;
    DetectorSD* fCALWrapSD;
    G4VisAttributes* fVis;
    G4VisAttributes* fWrapVis;

    G4Material* fCALMaterial;
    G4Material* fWrapMaterial;

    // APD Stuff (Optical Photon)
    G4double wSizeX;
    G4double wSizeY;
    G4double wSizeZ;
    G4double aSizeX;
    G4double aSizeY;
    G4double aSizeZ;
    G4double gSizeX;
    G4double gSizeY;
    G4double gSizeZ;
    G4double wPosX;
    G4double wPosY;
    G4double wPosZ;
    G4Material* APD_Material;
    G4Material* Glue_Material;

    G4RotationMatrix* HepRot;

    // For Matrix Placement
    std::vector<G4LogicalVolume*> fCaloLVVector;
    std::vector<G4LogicalVolume*> fWrapLVVector;
};

#endif
