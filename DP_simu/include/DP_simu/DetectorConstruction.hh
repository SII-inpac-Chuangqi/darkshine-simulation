//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id$
//
/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "DetectorSD.hh"
#include "G4UniformMagField.hh"
#include "G4PVPlacement.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "RootManager.hh"
#include "BOptrMultiParticleChangeCrossSection.hh"

#include <vector>
#include <iterator>

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4UserLimits;

class RootManager;
class DetectorMessenger;

/// Detector construction class to define materials and geometry.
///
/// In addition a transverse uniform magnetic field is defined in
/// SetMagField() method which can be activated via a command
/// defined in the DetectorMessenger class. 

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    explicit DetectorConstruction(RootManager*);
    ~DetectorConstruction() override;

  public:
    G4VPhysicalVolume* Construct() override;

    // Set methods
    void SetCheckOverlaps(G4bool );
    void SetTagTrkMagField(G4double in);
    void SetRecTrkMagField(G4double in);

    void SetifBias(G4bool );
    void SetBiasLayer();
    void SetOptical(G4bool );

  private:
    // methods
    G4VPhysicalVolume* DefineVolumes();
    void ConstructSDandField() override;
    void  DefineMaterials();
    void  DefineParameters();
    void  DefineTarget();
    void  DefineTagTracker();
    void  DefineRecTracker();
    void  DefineECAL();
    void  DefineHCAL();
    void  DefineWorld();

    // Root Manaer
    RootManager* fRootMng;

    DetectorMessenger*  fMessenger;   // messenger
    
    //global option
    G4UserLimits* fStepLimit; // pointer to user step limits
    G4bool  fCheckOverlaps;   // option to activate checking of volumes overlaps
    std::vector<G4LogicalVolume*>::iterator itr_LV;

    G4bool build_Target;
    G4bool build_TagTrk;
    G4bool build_RecTrk;
    G4bool build_ECAL_Center;
    G4bool build_ECAL_Outer;
    G4bool build_HCAL;
    /////////////////////////
    //  EM Field
    /////////////////////////
    G4double TagTrk_MagField_y{};
    G4double RecTrk_MagField_y{};

    /////////////////////////
    //  APD stuff
    /////////////////////////
    G4Material* APD_Mat{};
    G4Material* Glue_Mat{};
    G4ThreeVector APD_Size;
    G4ThreeVector Glue_Size;

    /////////////////////////
    //  World
    /////////////////////////
    G4Material* World_Mat{};
    G4ThreeVector Size_World;
    G4LogicalVolume* World_LV{};
    G4PVPlacement* World_PV{};

    /////////////////////////
    //  Target
    /////////////////////////
    G4Material* Target_Mat{};
    G4ThreeVector Target_Size;
    G4ThreeVector Target_Pos;
    G4double Trk_Tar_Dis{};

    std::vector<G4LogicalVolume*> Target_LV;

    /////////////////////////
    //  Tagging Tracker
    /////////////////////////
    G4Material* TagTrk_Mat{};
    G4Material* TagRegion_Mat{};
    G4int No_TagTrk{};
    G4ThreeVector Size_TagTrk[7];
    G4ThreeVector Pos_TagTrk[7];
    G4ThreeVector Size_TagRegion;
    G4ThreeVector Pos_TagRegion;

    G4double Tag_Angle{};

    G4LogicalVolume* TagRegion_LV{};
    std::vector<G4LogicalVolume*> TagTrk_LV1;
    std::vector<G4LogicalVolume*> TagTrk_LV2;

    /////////////////////////
    //  Recoil Tracker
    /////////////////////////
    G4Material* RecTrk_Mat{};
    G4Material* RecRegion_Mat{};
    G4int No_RecTrk{};
    G4ThreeVector Size_RecTrk[6];
    G4ThreeVector Pos_RecTrk[6];
    G4ThreeVector Size_RecRegion;
    G4ThreeVector Pos_RecRegion;

    G4double Rec_Angle{};

    G4LogicalVolume* RecRegion_LV{};
    std::vector<G4LogicalVolume*> RecTrk_LV1;
    std::vector<G4LogicalVolume*> RecTrk_LV2;

    /////////////////////////
    //  ECAL
    /////////////////////////
    G4Material* ECALRegion_Mat{};
    G4Material* ECAL_Center_Mat{};
    G4Material* ECAL_Outer_Mat{};
    G4Material* ECAL_Wrap_Mat{};
    G4ThreeVector Size_ECALRegion;
    G4ThreeVector Pos_ECALRegion;
    G4ThreeVector ECAL_Center_Wrap_Size;
    G4ThreeVector ECAL_Center_Size;
    G4ThreeVector ECAL_Center_Module_No;
    G4ThreeVector ECAL_Outer_Wrap_Size;
    G4ThreeVector ECAL_Outer_Size_Dir;
    G4ThreeVector ECAL_Outer_Mod_No_Dir;
    G4ThreeVector ECAL_Outer_Module_No;
    G4double ECAL_Module_Gap{};

    std::vector<G4LogicalVolume*> ECAL_Center_LV;
    std::vector<G4LogicalVolume*> ECAL_Outer_LV[4];

    /////////////////////////
    //  HCAL
    /////////////////////////
    G4Material* HCALRegion_Mat{};
    G4Material* HCAL_Mat{};
    G4Material* HCAL_Wrap_Mat{};
    G4Material* HCAL_Absorber_Mat{};
    G4ThreeVector Size_HCALRegion;
    G4ThreeVector Pos_HCALRegion;
    G4ThreeVector HCAL_Wrap_Size;
    G4ThreeVector HCAL_Size_Dir;
    G4ThreeVector HCAL_Mod_No_Dir;
    G4ThreeVector HCAL_Module_No;
    G4double HCAL_Module_Gap{};
    G4double HCAL_Absorber_Thickness{};

    std::vector<G4LogicalVolume*> HCAL_SD_LV[9];

};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
