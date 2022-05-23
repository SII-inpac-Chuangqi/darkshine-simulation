
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
#include "G4PVPlacement.hh"
#include "Control/Control.h"

#include <vector>

// Volume relationship:
// 2 Outline (Trk)
// └-1 SMTBlock
//   └-0 Silicon micro-strip (Strip)

class TrkConstruct
{
  public:
    TrkConstruct(G4String, G4LogicalVolume*, G4int, G4bool);
    TrkConstruct(const TrkConstruct&);
    virtual ~TrkConstruct();

    G4ThreeVector BoxConstruct();
    G4ThreeVector BoxPlacement();
    G4ThreeVector SMTConstruct();
    G4ThreeVector LinearPlacement(G4int zNo,
                                  G4ThreeVector *SizeVec,
                                  G4ThreeVector *PosVec,
                                  std::vector<G4int> StripNVec,
                                  G4ThreeVector *AngleGapVec,
                                  G4int stripBlockN);

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
    void SetVis1(G4VisAttributes* in) { fVis1 = in; };
    void SetVis2(G4VisAttributes* in) { fVis2 = in; };
    void SetMotherVolume(G4LogicalVolume* in)   { fMotherVolume = in; };
    void SetTrkMaterial(G4Material* in)         { fTrkMaterial = in; };

    void SetRotation(double in)         { fAngle = in; };
    void SetZMove(double in)            { fZMove = in; };
    void SetSizeXYZ(const G4ThreeVector &in)   { fSizeX = in.x(); fSizeY = in.y(); fSizeZ = in.z(); };
    void SetPosXYZ(const G4ThreeVector &in)    { fPosX = in.x();  fPosY = in.y();  fPosZ = in.z();  };
    void SetStrip_Angle_Gap(const G4int &stripN, const G4ThreeVector &angleGap);
    void SetStrip_Block_N(G4int in) { fStripBlockN = in;};

    std::vector<G4LogicalVolume*> GetTrkLVVector() { return fTrkLVVector; };
    std::vector<G4LogicalVolume*> GetStripLVVector() { return fStripLVVector; };

  private:
    G4bool fType{false}; //
    G4bool fCheckOverlap;

    G4double eps = dControl->eps;
    G4double fSizeX;
    G4double fSizeY;
    G4double fSizeZ;
    G4double fPosX;
    G4double fPosY;
    G4double fPosZ;
    G4ThreeVector fPos1; // position of front Tracker, with ZMove
    G4ThreeVector fPos2; // position of back Tracker, with ZMove

    G4double fAngle;
    G4double fZMove;

    G4int fCopyNo;
    G4String fTrkName;

    G4LogicalVolume* fMotherVolume;
    G4VisAttributes* fVis;
    G4VisAttributes* fVis1{nullptr};
    G4VisAttributes* fVis2{nullptr};

    G4Material* fTrkMaterial;

    G4LogicalVolume* fTrkLV{nullptr};
    G4double fAngle1;
    G4double fAngle2;
    std::vector<G4LogicalVolume*> fTrkLVVector;

    // Silicon Micro-Strip-Block
    G4int fStripBlockN{-1};
    G4LogicalVolume* fBlockLV{nullptr};
    std::vector<G4LogicalVolume*> fBlockLVVector;

    // Silicon Micro-Strip
    G4int fStripNum{-1};
    G4double fStripSizeX{0};
    G4double fStripSizeY{0};
    G4double fStripSizeZ{0};
    G4double fStripPosX{0};
    G4double fStripPosY{0};
    G4double fStripPosZ{0};
    G4double fStripDistanceX{0};
    G4VisAttributes* fStripVis{nullptr};
    G4LogicalVolume* fStripLV{nullptr};
    std::vector<G4LogicalVolume*> fStripLVVector;


    std::vector<G4PVPlacement*> PVVector;
};

#endif
