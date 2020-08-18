
#ifndef TrkConstruct_h
#define TrkConstruct_h 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include <vector>

class TrkConstruct
{
  public:
    TrkConstruct(G4String, G4LogicalVolume*, G4int, G4bool);
    TrkConstruct(const TrkConstruct&);
    virtual ~TrkConstruct();

    G4ThreeVector Construct();
    G4ThreeVector LinearPlacement(G4int, G4ThreeVector*, G4ThreeVector*);

    // initializing the output LV
    void CleanLV() { fTrkLVVector.clear(); };
    // Set Methods
    void SetType(G4bool in)             { fType = in; }; 
    void SetCheckOverlap(G4bool in)     { fCheckOverlap = in; };
    void SetSizeX(G4double in)          { fSizeX = in; };
    void SetSizeY(G4double in)          { fSizeY = in; };
    void SetSizeZ(G4double in)          { fSizeZ = in; };
    void SetPosX(G4double in)           { fPosX  = in; };
    void SetPosY(G4double in)           { fPosY  = in; };
    void SetPosZ(G4double in)           { fPosZ  = in; };
    void SetCopyNo(G4int in)            { fCopyNo = in; };
    void SetTrkName(const G4String &in)        { fTrkName = in; };
    void SetVis(G4VisAttributes* in)    { fVis = in; };
    void SetMotherVolume(G4LogicalVolume* in)   { fMotherVolume = in; };
    void SetTrkMaterial(G4Material* in)         { fTrkMaterial = in; };

    void SetRotation(double in)         { fAngle = in; };
    void SetZMove(double in)            { fZMove = in; };
    void SetSizeXYZ(const G4ThreeVector &in)   { fSizeX = in.x(); fSizeY = in.y(); fSizeZ = in.z(); };
    void SetPosXYZ(const G4ThreeVector &in)    { fPosX = in.x();  fPosY = in.y();  fPosZ = in.z();  };

    std::vector<G4LogicalVolume*> GetTrkLVVector() { return fTrkLVVector; };

  private:
    G4bool fType; //
    G4bool fCheckOverlap;
    
    G4double fSizeX;
    G4double fSizeY;
    G4double fSizeZ;
    G4double fPosX;
    G4double fPosY;
    G4double fPosZ;

    G4double fAngle;
    G4double fZMove;

    G4int fCopyNo;
    G4String fTrkName;

    G4LogicalVolume* fMotherVolume;
    G4VisAttributes* fVis;

    G4Material* fTrkMaterial;

    std::vector<G4LogicalVolume*> fTrkLVVector;
};

#endif
