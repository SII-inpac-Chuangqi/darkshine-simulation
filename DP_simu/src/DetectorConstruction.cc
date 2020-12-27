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
#include "DP_simu/DetectorMessenger.hh"
#include "DP_simu/DetectorSD.hh"
#include "DP_simu/RootManager.hh"
#include "DP_simu/BOptrChangeCrossSection.hh"
#include "DP_simu/BOptrMultiParticleChangeCrossSection.hh"
#include "DP_simu/CALConstruct.hh"
#include "DP_simu/TrkConstruct.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4RunManager.hh"
#include "G4FieldManager.hh"
#include "G4SDManager.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4ProductionCuts.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4GDMLParser.hh"

#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"

#include <iterator>
#include <cstdio>

//#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction(RootManager *rootMng) {
    fMessenger = new DetectorMessenger(this);

    // TODO (xuliang): 1. Move all to /Geometry.
    // TODO (xuliang): 2. Add commands for modifying geometry.
    //                  could be done by using python script to generate code.
    // TODO (xuliang): 3. Importing ASCII Text Models
    // see examples/extended/persistency/P03

    // Trackers

    TagTrk = new Tracker_Construct();
    RecTrk = new Tracker_Construct();

    // Build-in ECAL Confituration definition
    ECAL_Con1 = new ECAL_XYCrossing(); // Color grey blue
    ECAL_Con2 = new ECAL_AllZ(); // Color blue

    // Build-in HCAL Confituration definition
    HCAL_Con = new HCAL_Construct();

    fRootMng = rootMng;
    fCheckOverlaps = false;
    fStepLimit = nullptr;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction() {
    delete fStepLimit;
    delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *DetectorConstruction::Construct() {
    // Define materials, Only do it once.
    if (!reconstruct) DefineMaterials();
    
    DefineParameters();
    return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials() {

    G4NistManager *nistManager = G4NistManager::Instance();

    // Material definition 
    G4Element *elP = nistManager->FindOrBuildElement("P");
    G4Element *elW = nistManager->FindOrBuildElement("W");
    G4Element *elO = nistManager->FindOrBuildElement("O");
    G4Element *elN = nistManager->FindOrBuildElement("N");
    G4Element *elLu = nistManager->FindOrBuildElement("Lu");
    G4Element *elSi = nistManager->FindOrBuildElement("Si");
    G4Element *elH = nistManager->FindOrBuildElement("H");
    G4Element *elC = nistManager->FindOrBuildElement("C");
    G4Element *elY = nistManager->FindOrBuildElement("Y");

    // Vacuum
    auto *Vacuum = new G4Material("vacuum", universe_mean_density, 2);
    Vacuum->AddElement(elN, .7);
    Vacuum->AddElement(elO, .3);

    // Defining LYSO
    auto *LSO = new G4Material("LSO", 7.4 * g / cm3, 3);
    LSO->AddElement(elLu, 2);
    LSO->AddElement(elSi, 1);
    LSO->AddElement(elO, 5);

    auto *YSO = new G4Material("YSO", 4.5 * g / cm3, 3);
    YSO->AddElement(elY, 2);
    YSO->AddElement(elSi, 1);
    YSO->AddElement(elO, 5);

    auto *LYSO = new G4Material("LYSO", 7.1 * g / cm3, 2, kStateSolid);
    LYSO->AddMaterial(LSO, 90 * perCent);
    LYSO->AddMaterial(YSO, 10 * perCent);

    // Defining Polystyrene
    auto *PS = new G4Material("Polystyrene", 1.04 * g / cm3, 2);
    PS->AddElement(elC, 8);
    PS->AddElement(elH, 8);

    // Define PWO4
    auto *PWO4 = new G4Material("PWO4", 5.26 * g / cm3, 3);
    PWO4->AddElement(elP, 1);
    PWO4->AddElement(elW, 1);
    PWO4->AddElement(elO, 4);

    // Define Carbon Fiber
    auto *CarbonFiber = new G4Material("CarbonFiber", 1.93 * g / cm3, 1);
    CarbonFiber->AddElement(elC, 1);

    // Build By Nist
    nistManager->FindOrBuildMaterial("G4_W");
    nistManager->FindOrBuildMaterial("G4_Si");
    nistManager->FindOrBuildMaterial("G4_Al");
    nistManager->FindOrBuildMaterial("G4_Fe");

    /* Define Optical Properties */

    if (fRootMng->GetOptical()) {
        //
        // ------------ Generate & Add Material Properties Table ------------
        //

        G4double photonEnergy[] = {0.1 * eV, 2.21 * eV, 2.58 * eV, 2.82 * eV, 2.95 * eV, 3.10 * eV, 4.00 * eV};

        const G4int nEntries = sizeof(photonEnergy) / sizeof(G4double);

        G4double RefractionIdx[nEntries] = {1.85, 1.833, 1.821, 1.813, 1.809, 1.804, 1.79};
        G4double AbsorptionLength[nEntries] = {40. * cm, 40. * cm, 40. * cm, 40. * cm, 40. * cm, 40. * cm, 40. * cm};

        auto *MPT = new G4MaterialPropertiesTable();
        MPT->AddProperty("RINDEX", photonEnergy, RefractionIdx, nEntries);
        MPT->AddProperty("ABSLENGTH", photonEnergy, AbsorptionLength, nEntries);

        G4double ScintEnergy[nEntries] = {0.1 * eV, 2.21 * eV, 2.58 * eV, 2.82 * eV, 2.95 * eV, 3.10 * eV, 4.00 * eV};
        G4double ScintFast[nEntries] = {0.0, 0.23, 0.85, 1.93, 2.15, 1.08, 0.0};

        MPT->AddProperty("FASTCOMPONENT", ScintEnergy, ScintFast, nEntries);

        MPT->AddConstProperty("SCINTILLATIONYIELD", 200. / MeV);
        MPT->AddConstProperty("RESOLUTIONSCALE", 1.0);
        MPT->AddConstProperty("FASTTIMECONSTANT", 40. * ns);
        MPT->AddConstProperty("YIELDRATIO", 1.);

        LYSO->SetMaterialPropertiesTable(MPT);
    }

    // Print materials
    //G4cout << *(G4Material::GetMaterialTable()) < < G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineParameters() {
    
    if (reconstruct) {
        /* Define all the parameter used */
        /////////////////////////
        //  APD
        /////////////////////////
        APD_Mat = G4Material::GetMaterial("G4_Si");
        APD_Size = G4ThreeVector(1 * cm, 1 * cm, 1 * mm);

        Glue_Mat = G4Material::GetMaterial("G4_W");
        Glue_Size = G4ThreeVector(1 * cm, 1 * cm, 0.1 * mm);
    
        /////////////////////////
        //  Target
        /////////////////////////
        Target_Mat = G4Material::GetMaterial("G4_W");
        Target_Size = G4ThreeVector(10 * cm, 20 * cm, 350 * um);
        Target_Pos = G4ThreeVector(0 * cm, 0 * cm, 0 * cm);

    
        ///  Tagging Tracker

        TagTrk->DefineParameters(dTagging, Trk_Tar_Dis, Target_Size);
        Size_TagRegion = TagTrk->GetSizeTrkRegion();
        Pos_TagRegion = TagTrk->GetPosTrkRetion();

        ///  Recoil Tracker

        RecTrk->DefineParameters(dRecoil, Trk_Tar_Dis, Target_Size);
        Size_RecRegion = RecTrk->GetSizeTrkRegion(); 
        Pos_RecRegion = RecTrk->GetPosTrkRetion();

        ///  ECAL
    
        if (build_ECAL) {
            if (ECAL_Selection == 1) {
                ECAL_Con1->DefineParameters(Pos_RecRegion, Size_RecRegion);
                Pos_ECAL = ECAL_Con1->getPosEcalRegion();
                Size_ECAL = ECAL_Con1->getSizeEcalRegion();
            }
            else if (ECAL_Selection == 2) {
                ECAL_Con2->DefineParameters(Pos_RecRegion, Size_RecRegion);
                Pos_ECAL = ECAL_Con2->getPosEcalRegion();
                Size_ECAL = ECAL_Con2->getSizeEcalRegion();
            }
        }
        // If no ECAL but built Tracker, in order to determine the position of HCAL,
        // use Tagger's position instead. If only HCAL, use default Pos_ECAL = 0.
        else if (build_RecTrk) { 
            Pos_ECAL = Pos_RecRegion;
            Size_ECAL = Size_RecRegion;
        }
        else if (build_TagTrk) {
            Pos_ECAL = Pos_TagRegion;
            Size_ECAL = Size_TagRegion;
        }
        else {
            Pos_ECAL = G4ThreeVector(0 * cm, 0 * cm, 0* cm);
            Size_ECAL = G4ThreeVector(0 * cm, 0 * cm, 0 * cm);
        }

        if (build_HCAL ) HCAL_Con->DefineParameters(Pos_ECAL, Size_ECAL);
    }
    /////////////////////////
    //  World
    /////////////////////////
    World_Mat = G4Material::GetMaterial("vacuum");
    G4double l = 10 * m;
    // if (reconstruct && build_HCAL) {
    //     l = 2.0 * (HCAL_Con->getPosHcalRegion().z() + HCAL_Con->getSizeHcalRegion().x());
    // }
    Size_World = G4ThreeVector(l, l, l); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *DetectorConstruction::DefineVolumes() {

    DefineWorld();      // Build World
    if (reconstruct) {
        if (build_Target) DefineTarget();     // Build Target
        /// Build Tagging Tracker
        if (build_TagTrk) TagTrk->Build(dTagging, World_LV, fRootMng, fCheckOverlaps);
        /// Build Recoil Tracker
        if (build_RecTrk) RecTrk->Build(dRecoil, World_LV, fRootMng, fCheckOverlaps);
        // Build ECAL
        if (build_ECAL) {
            if (ECAL_Selection == 1)
                ECAL_Con1->Build(0, World_LV, fRootMng, fCheckOverlaps);
            else if (ECAL_Selection == 2)
                ECAL_Con2->Build(0, World_LV, fRootMng, fCheckOverlaps);
        }
        if (build_HCAL) HCAL_Con->Build(0, World_LV, fRootMng, fCheckOverlaps);

        // Book RootMng
        fRootMng->book();
        G4cout << "[Root Manager] ==> Root Manager initialized ..." << G4endl;
        G4cout << "[Root Manager] ==> Output File " << fRootMng->GetOutFileName() << " created ..." << G4endl;
    }
    // Set User Limit 
    G4double maxStep = 10 * mm;
    fStepLimit = new G4UserLimits(maxStep, DBL_MAX, 200 * s);

    //SetBiasLayer();

    return World_PV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineWorld() {
    ////////////////////////////////////////////////////////////
    //
    //  World
    //
    ////////////////////////////////////////////////////////////
    if (!reconstruct) {
        G4GeometryManager::GetInstance()->SetWorldMaximumExtent(Size_World.z());

        G4cout << "Computed tolerance = "
               << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance() / mm
               << " mm" << G4endl;
    }
    auto World_Box = new G4Box("World_Box", Size_World.x() / 2, Size_World.y() / 2, Size_World.z() / 2); // Solid of World.
    World_LV = new G4LogicalVolume(World_Box, World_Mat, "World_LV"); 
    World_PV = new G4PVPlacement(nullptr, G4ThreeVector(), World_LV, "World", nullptr, false, 0, fCheckOverlaps);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineTarget() {
    ////////////////////////////////////////////////////////////
    //
    //  Target
    //
    ////////////////////////////////////////////////////////////

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
    //G4cerr << "[DEBUG] ConstructSDandField" << G4endl;
    if (reconstruct) {
        // Construct Magnetic Field and Sensitive Detector of the Tagging Tracker
        if (build_TagTrk) TagTrk->BuildSDandField(dTagging, fRootMng);
        // Constructg Magnetic Field and Sensitive Detector of the Recoil Tracker
        if (build_RecTrk) RecTrk->BuildSDandField(dRecoil, fRootMng);
        
        /*                              */
        /* Construct Sensitive Detector */
        /*                              */

        if (build_ECAL) {
            if (ECAL_Selection == 1) ECAL_Con1->BuildSD(fRootMng);
            else if (ECAL_Selection == 2) ECAL_Con2->BuildSD(fRootMng);
        }

        if (build_HCAL ) HCAL_Con->BuildSD(fRootMng);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetifBias(G4bool in) {
    fRootMng->SetifBias(in);
    if (in) SetBiasLayer();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetBiasLayer() {
    G4cout << "[Bias Setting] ==> Setting bias layers ..." << G4endl;
    G4cout << "[Bias Setting] ==> Setting bias process: " << fRootMng->GetBiasProcess() << G4endl;
    G4cout << "[Bias Setting] ==> Setting bias factor:  " << fRootMng->GetBiasFactor() << G4endl;
    G4cout << "[Bias Setting] ==> Setting min bias E:   " << fRootMng->GetBiasEmin() << G4endl;
    G4cout << "[Bias Setting] ==> Biasing Layer Target: " << fRootMng->GetifBiasTarget() << G4endl;
    G4cout << "[Bias Setting] ==> Biasing Layer ECAL:   " << fRootMng->GetifBiasECAL() << G4endl;

    /*                           */
    /* define biasing parameters */
    /*                           */

    G4String BiasProcess = fRootMng->GetBiasProcess();
    G4double BiasFactor = fRootMng->GetBiasFactor();
    G4double BiasEmin = fRootMng->GetBiasEmin();

    auto bias = new BOptrMultiParticleChangeCrossSection(BiasProcess);
    bias->AddParticle("e-", BiasFactor, BiasEmin, BiasProcess);
    bias->AddParticle("gamma", BiasFactor, BiasEmin, BiasProcess);

    if (fRootMng->GetifBiasTarget()) {
        for (itr_LV = Target_LV.begin(); itr_LV != Target_LV.end(); itr_LV++)
            bias->AttachTo(*itr_LV);
    }
    if (reconstruct) {
        if (build_ECAL && fRootMng->GetifBiasECAL()) {
            if (ECAL_Selection == 1) ECAL_Con1->BuildBias(bias);
            else if (ECAL_Selection == 2) ECAL_Con2->BuildBias(bias);
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetOptical(G4bool in) {
    fRootMng->SetOptical(in);

}

void DetectorConstruction::SaveGeometry() {

    /* Save GDML to ROOT File */
    G4GDMLParser parser;
    parser.Write("geometry.gdml", World_PV);

    fRootMng->FillGeometry("geometry.gdml");

}

/// ------ Functions to rebuild geometry at runtime. ------

void DetectorConstruction::CleanGeometry(G4bool clean) {
    if (clean) { // clean-up previous geometry.
        G4SolidStore::GetInstance()->Clean();
        G4LogicalVolumeStore::GetInstance()->Clean();
        G4PhysicalVolumeStore::GetInstance()->Clean();
    }
}

void DetectorConstruction::ReConstruct(G4bool flag) {
    if(flag) CleanGeometry();
    // If this is the second construction,
    // we should clean-up the previous
    // geometry in safety concern.
    reconstruct = true;
    // When reconstruct = 0, World is empty.
    // When reconstruct = 1, World is filld.
    /// Define new one
    G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
    ConstructSDandField();
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

/// Setter

/// Target SEtter

void DetectorConstruction::SetifTarget(G4bool build) {
    build_Target = build;
    G4cout << "[Detector Geo.] ==> turned " << (build ? "ON " : "OFF ") << "Target" << G4endl;
}

/// TagTrk Setter

void DetectorConstruction::SetifTagTrk(G4bool build) {
    build_TagTrk = build;
    //if (!build) TagTrk->SetTrackerMagField(0.); // Clear magnetic field
    G4cout << "[Detector Geo.] ==> turned " << (build ? "ON " : "OFF ") << "Tagging Tracker" << G4endl;
}

void DetectorConstruction::AddNewTagTrkSize(const G4ThreeVector& in) {
    TagTrk->AddNewTrackerSize(in);
}

void DetectorConstruction::AddNewTagTrkPos(const G4ThreeVector& in) {
    TagTrk->AddNewTrackerPos(in);
}

void DetectorConstruction::DelTagTrk(G4bool in = true) {
    TagTrk->DelTracker(in);
}

void DetectorConstruction::SetTagTrk1Rotation(G4double in) {
    TagTrk->SetTracker1Rotation(in);
}

void DetectorConstruction::SetTagTrk2Rotation(G4double in) {
    TagTrk->SetTracker2Rotation(in);
}

void DetectorConstruction::SetTagTrk1Color(const G4ThreeVector& in) {
    TagTrk->SetTracker1Color(in);
}

void DetectorConstruction::SetTagTrk2Color(const G4ThreeVector& in) {
   TagTrk->SetTracker2Color(in);
}

void DetectorConstruction::SetTagTrkMagField(G4double in) {
    TagTrk->SetTrackerMagField(in);
}

/// RecTrk Setter

void DetectorConstruction::SetifRecTrk(G4bool build) {
    build_RecTrk = build;
    //if (!build) RecTrk->SetTrackerMagField(0.); // Clear magnetic field
    G4cout << "[det]==>turned " << (build ? "ON " : "OFF ") << "Recoil Tracker" << G4endl; 
}

void DetectorConstruction::AddNewRecTrkSize(const G4ThreeVector& in) {
    RecTrk->AddNewTrackerSize(in);
}

void DetectorConstruction::AddNewRecTrkPos(const G4ThreeVector& in) {
    RecTrk->AddNewTrackerPos(in);
}

void DetectorConstruction::DelRecTrk(G4bool in) {
    RecTrk->DelTracker(in);
}

void DetectorConstruction::SetRecTrk1Rotation(G4double in) {
    RecTrk->SetTracker1Rotation(in);
}

void DetectorConstruction::SetRecTrk2Rotation(G4double in) {
    RecTrk->SetTracker2Rotation(in);
}

void DetectorConstruction::SetRecTrk1Color(const G4ThreeVector& in) {
    RecTrk->SetTracker1Color(in);
}

void DetectorConstruction::SetRecTrk2Color(const G4ThreeVector& in) {
    RecTrk->SetTracker2Color(in);
}

void DetectorConstruction::SetRecTrkMagField(G4double in) {
    RecTrk->SetTrackerMagField(in);
}

/// ECAL Setter

void DetectorConstruction::SetifECAL(G4bool build) {
    build_ECAL = build;
    G4cout << "[Detector Geo.] ==> turned " << (build ? "ON " : "OFF ") << "ECAL" << G4endl;
}

void DetectorConstruction::SetECALSelection(unsigned int id) {
    ECAL_Selection = id;
    G4cout << "[Detector Geo.] ==> Selected ECAL Configuration " << id << G4endl;
}

void DetectorConstruction::SetECALCenterWrapSize(const G4ThreeVector& in) {
    ECAL_Con1->SetECALCenterWrapSize(in);
    ECAL_Con2->SetECALCenterWrapSize(in);
}

void DetectorConstruction::SetECALCenterSize(const G4ThreeVector& in) {
    ECAL_Con1->SetECALCenterSize(in);
    ECAL_Con2->SetECALCenterSize(in);
}

void DetectorConstruction::SetECALCenterModuleNo(const G4ThreeVector& in) {
    ECAL_Con1->SetECALCenterModuleNo(in);
    ECAL_Con2->SetECALCenterModuleNo(in);
}

/// HCAL Setter

void DetectorConstruction::SetifHCAL(G4bool build) {
    build_HCAL = build;
    G4cout << "[Detector Geo.] ==> turned " << (build ? "on " : "off ") << "HCAL" << G4endl;
}

void DetectorConstruction::SetHCALWrapSize(const G4ThreeVector& in) {
    HCAL_Con->SetHCALWrapSize(in);
}

void DetectorConstruction::SetHCALSizeDir(const G4ThreeVector& in) {
    HCAL_Con->SetHCALSizeDir(in);
}

void DetectorConstruction::SetHCALModNoDir(const G4ThreeVector& in) {
    HCAL_Con->SetHCALModNoDir(in);
}

void DetectorConstruction::SetHCALModuleNo(const G4ThreeVector& in) {
    HCAL_Con->SetHCALModuleNo(in);
}

void DetectorConstruction::SetHCALModuleGap(G4double in) {
    HCAL_Con->SetHCALModuleGap(in);
}

void DetectorConstruction::SetHCALAbsorberThickness(G4double in) {
    HCAL_Con->SetHCALAbsorberThickness(in);
}
