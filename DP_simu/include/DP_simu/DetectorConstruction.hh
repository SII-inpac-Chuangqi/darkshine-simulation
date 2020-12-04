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

#include "Geometry/ECAL_XYCrossing.h"
#include "Geometry/ECAL_AllZ.h"
#include "Geometry/HCAL_Construct.h"

#include <vector>
#include <iterator>

class G4VPhysicalVolume;

class G4LogicalVolume;

class G4Material;

class G4UserLimits;

class RootManager;

class DetectorMessenger;

class ECAL_XYCrossing;

class ECAL_AllZ;

class HCAL_Construct;

/// Detector construction class to define materials and geometry.
///
/// In addition a transverse uniform magnetic field is defined in
/// SetMagField() method which can be activated via a command
/// defined in the DetectorMessenger class. 

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    explicit DetectorConstruction(RootManager *);

    ~DetectorConstruction() override;

public:
    G4VPhysicalVolume *Construct() override;

    void SaveGeometry();

    // Set methods

    void SetTagTrkMagField(G4double in);

    void SetRecTrkMagField(G4double in);

    void SetifBias(G4bool);

    void SetBiasLayer();

    void SetOptical(G4bool);

    /// \brief Clean-up previous geometry.
    /// \param[in] clean if it is true,G4SolidStore, G4LogicalVolumeStore
    /// and G4PhysicalVolumeStore will be cleaned up
    void CleanGeometry(G4bool clean=true);

    /// \brief SHOULD be called after modifing geometry at runtime.
    /// \param[in] if is true, clean-up previous geometry and define a new one.
    void RebuildGeometry(G4bool rebuild=true);
    
    /// \brief Select build-in ECAL Configuration.
    /// \param[in] id 1=ECAL_XYCrossing,
    /// 2=ECAL_ALLZ
    void SetECALSelection(unsigned int id);

    void SetifTarget(G4bool);

    void SetifTagTrk(G4bool);

    void SetifRecTrk(G4bool);

    void SetifECAL(G4bool);

    void SetifHCAL(G4bool);

private:
    // methods
    G4VPhysicalVolume *DefineVolumes();

    void ConstructSDandField() override;

    void DefineMaterials();

    void DefineParameters();

    void DefineTarget();

    void DefineTagTracker();

    void DefineRecTracker();

    void DefineWorld();

    // Root Manaer
    RootManager *fRootMng;

    // Messenger
    DetectorMessenger *fMessenger;

    // ECAL Construction Class
    ECAL_XYCrossing *ECAL_Con1;
    ECAL_AllZ *ECAL_Con2;
    unsigned int ECAL_Selection = 0; 

    HCAL_Construct *HCAL_Con;

    //global option
    G4UserLimits *fStepLimit; // pointer to user step limits
    G4bool fCheckOverlaps;   // option to activate checking of volumes overlaps
    std::vector<G4LogicalVolume *>::iterator itr_LV;

    G4bool reconstruct = false; // flag. Set to true when it is not the first-time construction of geometry.
    
    G4bool build_Target = true; // build Target if it is ture.
    G4bool build_TagTrk = true; // build Tagging trackir if it is true.
    G4bool build_RecTrk = true; // build Recoil Tracker if it is ture.
    G4bool build_ECAL = true; // build ECAL if it is ture.
    G4bool build_HCAL = true; // build HCAL if it is true.
    /////////////////////////
    //  EM Field
    /////////////////////////
    G4double TagTrk_MagField_y{-1.5};
    G4double RecTrk_MagField_y{-0.5};

    /////////////////////////
    //  APD stuff
    /////////////////////////
    G4Material *APD_Mat{};
    G4Material *Glue_Mat{};
    G4ThreeVector APD_Size;
    G4ThreeVector Glue_Size;

    /////////////////////////
    //  World
    /////////////////////////
    G4Material *World_Mat{}; // Materials of Word (vaccum).
    G4ThreeVector Size_World; // Total side-length of the "Wolrd" box. Referenced in G4Box *World_Box.
    G4LogicalVolume *World_LV{}; // Logical Volume of World.
    G4PVPlacement *World_PV{}; // Placement (Phyisical Volume) of the World. 

    /////////////////////////
    //  Target
    /////////////////////////
    G4Material *Target_Mat{};
    G4ThreeVector Target_Size;
    G4ThreeVector Target_Pos;
    G4double Trk_Tar_Dis{};

    std::vector<G4LogicalVolume *> Target_LV;

    /////////////////////////
    //  Tagging Tracker
    /////////////////////////
    G4Material *TagTrk_Mat{};
    G4Material *TagRegion_Mat{};
    G4int No_TagTrk{};
    G4ThreeVector Size_TagTrk[7];
    G4ThreeVector Pos_TagTrk[7];
    G4ThreeVector Size_TagRegion;
    G4ThreeVector Pos_TagRegion;

    G4double Tag_Angle{};

    G4LogicalVolume *TagRegion_LV{};
    std::vector<G4LogicalVolume *> TagTrk_LV1;
    std::vector<G4LogicalVolume *> TagTrk_LV2;

    /////////////////////////
    //  Recoil Tracker
    /////////////////////////
    G4Material *RecTrk_Mat{};
    G4Material *RecRegion_Mat{};
    G4int No_RecTrk{};
    G4ThreeVector Size_RecTrk[6];
    G4ThreeVector Pos_RecTrk[6];
    G4ThreeVector Size_RecRegion;
    G4ThreeVector Pos_RecRegion;

    /////////////////////////
    //  ECAL
    //////////////////////////
    G4ThreeVector Pos_ECAL;
    G4ThreeVector Size_ECAL;

    G4double Rec_Angle{};

    G4LogicalVolume *RecRegion_LV{};
    std::vector<G4LogicalVolume *> RecTrk_LV1;
    std::vector<G4LogicalVolume *> RecTrk_LV2;


};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
