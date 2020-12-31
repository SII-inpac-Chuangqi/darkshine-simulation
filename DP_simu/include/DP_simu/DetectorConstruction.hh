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
#include "Bias_Filter/BOptrMultiParticleChangeCrossSection.hh"

#include "Geometry/ECAL_XYCrossing.h"
#include "Geometry/ECAL_AllZ.h"
#include "Geometry/HCAL_Construct.h"
#include "Geometry/Tracker_Construct.h"

#include <vector>

class G4VPhysicalVolume;

class G4LogicalVolume;

class G4Material;

class G4UserLimits;

class RootManager;

class DetectorMessenger;

class Tracker_Construct;

class ECAL_XYCrossing;

class ECAL_AllZ;

class HCAL_Construct;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    explicit DetectorConstruction(RootManager *);

    ~DetectorConstruction() override;

    void ConstructSDandField() override; // build SD in run action

public:
    G4VPhysicalVolume *Construct() override;

    /// \brief Save Geometry to ROOT file
    void SaveGeometry();

    /// \brief Clean-up previous geometry.
    /// \param[in] clean If it is true,G4SolidStore, G4LogicalVolumeStore
    /// and G4PhysicalVolumeStore will be cleaned up
    [[maybe_unused]] static void CleanGeometry(G4bool clean=true);

private:
    // methods
    G4VPhysicalVolume *DefineVolumes();

    void DefineParameters();

    void DefineTarget();

    void DefineWorld();

    void SetBiasLayer();

    // Root Manaer
    RootManager *fRootMng;

    // Messenger
    DetectorMessenger *fMessenger;

    /// Tracker Construction Class

    Tracker_Construct *TagTrk;
    Tracker_Construct *RecTrk;

    // ECAL Construction Class
    ECAL_XYCrossing *ECAL_Con1;
    ECAL_AllZ *ECAL_Con2;

    // HCAL Construction Class
    HCAL_Construct *HCAL_Con;

    //global option
    G4UserLimits *fStepLimit; // pointer to user step limits
    G4bool fCheckOverlaps;   // option to activate checking of volumes overlaps

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

    std::vector<G4LogicalVolume *> Target_LV;
};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
