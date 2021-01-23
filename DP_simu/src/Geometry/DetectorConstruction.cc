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
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DP_simu/DetectorConstruction.hh"
#include "DP_simu/RootManager.hh"
#include "Bias_Filter/BOptrChangeCrossSection.hh"
#include "Bias_Filter/BOptrMultiParticleChangeCrossSection.hh"
#include "DP_simu/TrkConstruct.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4FieldManager.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4ProductionCuts.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4GDMLParser.hh"

#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"

#include <iterator>
#include <cstdio>
#include <filesystem>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction(RootManager *rootMng) {
    // Trackers
    TagTrk = new Tracker_Construct();
    RecTrk = new Tracker_Construct();

    // Build-in ECAL Configuration definition
    ECAL_Con1 = new ECAL_XYCrossing(); // Color grey blue
    ECAL_Con2 = new ECAL_AllZ(); // Color blue

    // Build-in HCAL Configuration definition
    HCAL_Con = new HCAL_Construct();

    fRootMng = rootMng;
    fCheckOverlaps = dControl->check_overlaps;
    fStepLimit = nullptr;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction() {
    delete fStepLimit;
    delete TagTrk;
    delete RecTrk;
    delete ECAL_Con1;
    delete ECAL_Con2;
    delete HCAL_Con;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *DetectorConstruction::Construct() {
    DefineParameters();
    return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineParameters() {

    ///  Target
    Target_Mat = dControl->Target_Mat;
    Target_Size = dControl->Target_Size;
    Target_Pos = dControl->Target_Pos;

    ///  Tagging Tracker
    TagTrk->DefineParameters(dTagging);

    ///  Recoil Tracker
    RecTrk->DefineParameters(dRecoil);

    ///  ECAL
    ECAL_Con2->DefineParameters();

    /// HCAL
    HCAL_Con->DefineParameters();

    ///  World
    World_Mat = dControl->World_Mat;
    Size_World = dControl->Size_World;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *DetectorConstruction::DefineVolumes() {

    DefineWorld();      // Build World

    // Build Target
    if (dControl->build_target) DefineTarget();
    // Build Tagging Tracker
    if (dControl->build_tag_tracker) TagTrk->Build(dTagging, World_LV, fCheckOverlaps);
    // Build Recoil Tracker
    if (dControl->build_rec_tracker) RecTrk->Build(dRecoil, World_LV, fCheckOverlaps);
    // Build ECAL
    if (dControl->build_ECAL) ECAL_Con2->Build(0, World_LV, fRootMng, fCheckOverlaps);
    // Build HCAL
    if (dControl->build_HCAL) HCAL_Con->Build(World_LV, fRootMng, fCheckOverlaps);

    // Book RootMng
    fRootMng->book();
    G4cout << "[Root Manager] ==> Root Manager initialized ..." << G4endl;
    G4cout << "[Root Manager] ==> Output File " << dControl->outfile_Name << " created ..." << G4endl;

    // Save Geometry
    if (dControl->save_geometry) SaveGeometry();

    // Set User Limit
    G4double maxStep = 10 * mm;
    fStepLimit = new G4UserLimits(maxStep, DBL_MAX, 200 * s);

    if (dControl->if_bias) SetBiasLayer();

    return World_PV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineWorld() {

    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(Size_World.z());

    G4cout << "Computed tolerance = "
           << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance() / mm
           << " mm" << G4endl;

    auto World_Box = new G4Box("World_Box", Size_World.x() / 2, Size_World.y() / 2,
                               Size_World.z() / 2); // Solid of World.
    World_LV = new G4LogicalVolume(World_Box, World_Mat, "World_LV");
    World_PV = new G4PVPlacement(nullptr, G4ThreeVector(), World_LV, "World", nullptr, false, 0, fCheckOverlaps);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineTarget() {

    auto Target = new TrkConstruct("Target", World_LV, 0, fCheckOverlaps);
    Target->SetSizeXYZ(Target_Size);
    Target->SetPosXYZ(Target_Pos);
    Target->SetTrkMaterial(Target_Mat);
    Target->SetVis(new G4VisAttributes(G4Colour(0, 1.0, 0)));
    Target->Construct();

    Target_LV = Target->GetTrkLVVector();

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField() {

    /*                              */
    /* Construct Sensitive Detector */
    /*                              */

    if (dControl->build_tag_tracker) TagTrk->BuildSDandField(dTagging, fRootMng);
    if (dControl->build_rec_tracker) RecTrk->BuildSDandField(dRecoil, fRootMng);
    if (dControl->build_ECAL) ECAL_Con2->BuildSD(fRootMng);
    if (dControl->build_HCAL) HCAL_Con->BuildSD(fRootMng);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetBiasLayer() {
    G4cout << "[Bias Setting] ==> Setting bias layers ..." << G4endl;
    G4cout << "[Bias Setting] ==> Setting bias process: " << dControl->BiasProcess << G4endl;
    G4cout << "[Bias Setting] ==> Setting bias factor:  " << dControl->BiasFactor << G4endl;
    G4cout << "[Bias Setting] ==> Setting min bias E:   " << dControl->BiasEmin << G4endl;
    G4cout << "[Bias Setting] ==> Biasing Layer Target: " << dControl->if_bias_target << G4endl;
    G4cout << "[Bias Setting] ==> Biasing Layer ECAL:   " << dControl->if_bias_ECAL << G4endl;

    /*                           */
    /* define biasing parameters */
    /*                           */

    G4String BiasProcess = dControl->BiasProcess;
    G4double BiasFactor = dControl->BiasFactor;
    G4double BiasEmin = dControl->BiasEmin;

    auto bias = new BOptrMultiParticleChangeCrossSection(BiasProcess);
    bias->AddParticle("e-", BiasFactor, BiasEmin, BiasProcess);
    bias->AddParticle("gamma", BiasFactor, BiasEmin, BiasProcess);

    if (dControl->if_bias_target) {
        for (auto LV : Target_LV)
            bias->AttachTo(LV);
    }

    if (dControl->build_ECAL && dControl->if_bias_ECAL) {
        ECAL_Con2->BuildBias(bias);
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SaveGeometry() {

    /* Save GDML to ROOT File */
    auto filename = "geometry.gdml";
    if (std::filesystem::exists(filename))
        std::remove(filename);

    G4GDMLParser parser;
    parser.Write(filename, World_PV);

    fRootMng->FillGeometry(filename);

}

/// ------ Functions to rebuild geometry at runtime. ------

[[maybe_unused]] void DetectorConstruction::CleanGeometry(G4bool clean) {
    if (clean) { // clean-up previous geometry.
        G4SolidStore::Clean();
        G4LogicalVolumeStore::Clean();
        G4PhysicalVolumeStore::Clean();
    }
}
