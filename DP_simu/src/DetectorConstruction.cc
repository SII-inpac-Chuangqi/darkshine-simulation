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

#include <iterator>
#include <stdio.h>

//#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction(RootManager *rootMng) {
    fMessenger = new DetectorMessenger(this);

    ECAL_Con1 = new ECAL_XYCrossing();
    ECAL_Con2 = new ECAL_AllZ();
    HCAL_Con = new HCAL_Construct();

    fRootMng = rootMng;
    fCheckOverlaps = false;
    fStepLimit = nullptr;

    build_Target = true;
    build_TagTrk = true;
    build_RecTrk = true;
    build_HCAL = true;

    ECAL_Selection = 2;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction() {
    delete fStepLimit;
    delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *DetectorConstruction::Construct() {
    // Define materials
    DefineMaterials();
    DefineParameters();

    // Define volumes
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
    /* Define all the parameter used */
    /////////////////////////
    //  APD
    /////////////////////////
    APD_Mat = G4Material::GetMaterial("G4_Si");
    APD_Size = G4ThreeVector(1 * cm, 1 * cm, 1 * mm);

    Glue_Mat = G4Material::GetMaterial("G4_W");
    Glue_Size = G4ThreeVector(1 * cm, 1 * cm, 0.1 * mm);

    /////////////////////////
    //  EM Field
    /////////////////////////
    TagTrk_MagField_y = -1.5 * tesla;
    RecTrk_MagField_y = -0.5 * tesla;
    /////////////////////////
    //  Target
    /////////////////////////
    Target_Mat = G4Material::GetMaterial("G4_W");
    Target_Size = G4ThreeVector(10 * cm, 20 * cm, 350 * um);
    Target_Pos = G4ThreeVector(0 * cm, 0 * cm, 0 * cm);
    Trk_Tar_Dis = 7.5 * mm;

    /////////////////////////
    //  Tagging Tracker
    /////////////////////////
    TagTrk_Mat = G4Material::GetMaterial("G4_Si");
    TagRegion_Mat = G4Material::GetMaterial("vacuum");

    Tag_Angle = 0.1 * radian;

    No_TagTrk = 7;
    Size_TagTrk[0] = G4ThreeVector(Target_Size.x(), Target_Size.y(), 0.1 * mm);
    Size_TagTrk[1] = G4ThreeVector(Target_Size.x(), Target_Size.y(), 0.1 * mm);
    Size_TagTrk[2] = G4ThreeVector(Target_Size.x(), Target_Size.y(), 0.1 * mm);
    Size_TagTrk[3] = G4ThreeVector(Target_Size.x(), Target_Size.y(), 0.1 * mm);
    Size_TagTrk[4] = G4ThreeVector(Target_Size.x(), Target_Size.y(), 0.1 * mm);
    Size_TagTrk[5] = G4ThreeVector(Target_Size.x(), Target_Size.y(), 0.1 * mm);
    Size_TagTrk[6] = G4ThreeVector(Target_Size.x(), Target_Size.y(), 0.1 * mm);

    Pos_TagTrk[0] = G4ThreeVector(0 * cm, 0 * cm, -30. * cm);
    Pos_TagTrk[1] = G4ThreeVector(0 * cm, 0 * cm, -20. * cm);
    Pos_TagTrk[2] = G4ThreeVector(0 * cm, 0 * cm, -10. * cm);
    Pos_TagTrk[3] = G4ThreeVector(0 * cm, 0 * cm, 0. * cm);
    Pos_TagTrk[4] = G4ThreeVector(0 * cm, 0 * cm, 10. * cm);
    Pos_TagTrk[5] = G4ThreeVector(0 * cm, 0 * cm, 20. * cm);
    Pos_TagTrk[6] = G4ThreeVector(0 * cm, 0 * cm, 30. * cm);

    Size_TagRegion = G4ThreeVector(2.0 * Size_TagTrk[0].x(), 2.0 * Size_TagTrk[0].y(),
                                   60 * cm + 2 * No_TagTrk * Size_TagTrk[0].z());
    Pos_TagRegion = G4ThreeVector(0 * cm, 0 * cm, -30 * cm - Trk_Tar_Dis - (Size_TagTrk[0].z() + Target_Size.z()) / 2);

    /////////////////////////
    //  Recoil Tracker
    /////////////////////////
    RecTrk_Mat = G4Material::GetMaterial("G4_Si");
    RecRegion_Mat = G4Material::GetMaterial("vacuum");

    Rec_Angle = 0.1 * radian;

    No_RecTrk = 6;
    Size_RecTrk[0] = G4ThreeVector(Target_Size.x(), 20 * cm, 0.1 * mm);
    Size_RecTrk[1] = G4ThreeVector(Target_Size.x(), 20 * cm, 0.1 * mm);
    Size_RecTrk[2] = G4ThreeVector(Target_Size.x(), 20 * cm, 0.1 * mm);
    Size_RecTrk[3] = G4ThreeVector(Target_Size.x(), 20 * cm, 0.1 * mm);
    Size_RecTrk[4] = G4ThreeVector(Target_Size.x(), 20 * cm, 0.1 * mm);
    Size_RecTrk[5] = G4ThreeVector(Target_Size.x(), 20 * cm, 0.1 * mm);

    Pos_RecTrk[0] = G4ThreeVector(0 * cm, 0 * cm, -86.25 * mm);
    Pos_RecTrk[1] = G4ThreeVector(0 * cm, 0 * cm, -71.25 * mm);
    Pos_RecTrk[2] = G4ThreeVector(0 * cm, 0 * cm, -55.25 * mm);
    Pos_RecTrk[3] = G4ThreeVector(0 * cm, 0 * cm, -40.25 * mm);
    Pos_RecTrk[4] = G4ThreeVector(0 * cm, 0 * cm, -4.25 * mm);
    Pos_RecTrk[5] = G4ThreeVector(0 * cm, 0 * cm, 86.25 * mm);

    Size_RecRegion = G4ThreeVector(2.0 * Size_RecTrk[5].x(), 2.0 * Size_RecTrk[5].y(),
                                   17.25 * cm + 2 * No_TagTrk * 0.1 * mm);
    Pos_RecRegion = G4ThreeVector(0 * cm, 0 * cm, 0.5 * Size_RecRegion.z() + 7.5 * mm + 0.5 * 350 * um);

    /////////////////////////
    //  ECAL
    /////////////////////////
    G4ThreeVector Pos_ECAL;
    G4ThreeVector Size_ECAL;
    if (ECAL_Selection == 1) {
        ECAL_Con1->DefineParameters(Pos_RecRegion, Size_RecRegion);
        Pos_ECAL = ECAL_Con1->getPosEcalRegion();
        Size_ECAL = ECAL_Con1->getSizeEcalRegion();
    }
    if (ECAL_Selection == 2) {
        ECAL_Con2->DefineParameters(Pos_RecRegion, Size_RecRegion);
        Pos_ECAL = ECAL_Con2->getPosEcalRegion();
        Size_ECAL = ECAL_Con2->getSizeEcalRegion();
    }

    HCAL_Con->DefineParameters(Pos_ECAL, Size_ECAL);
    /////////////////////////
    //  World
    /////////////////////////
    World_Mat = G4Material::GetMaterial("vacuum");
    auto l = 2.0 * (HCAL_Con->getPosHcalRegion().z() + HCAL_Con->getSizeHcalRegion().x());
    Size_World = G4ThreeVector(l, l, l);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *DetectorConstruction::DefineVolumes() {

    DefineWorld();      // Build World
    if (build_Target) DefineTarget();     // Build Target
    if (build_TagTrk) DefineTagTracker(); // Build Tagging Tracker
    if (build_RecTrk) DefineRecTracker(); // Build Recoil Tracker
    // Build ECAL
    if (ECAL_Selection == 1)
        ECAL_Con1->Build(0, World_LV, fRootMng, fCheckOverlaps);
    if (ECAL_Selection == 2)
        ECAL_Con2->Build(0, World_LV, fRootMng, fCheckOverlaps);

    if (build_HCAL) HCAL_Con->Build(0, World_LV, fRootMng, fCheckOverlaps);

    // Set User Limit 
    G4double maxStep = 10 * mm;
    fStepLimit = new G4UserLimits(maxStep, DBL_MAX, 200 * s);

    // Book RootMng
    fRootMng->book();
    G4cout << "[Root Manager] ==> Root Manager initialized ..." << G4endl;
    G4cout << "[Root Manager] ==> Output File" << fRootMng->GetOutFileName() << " created ..." << G4endl;

    SetBiasLayer();

    return World_PV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineWorld() {
    ////////////////////////////////////////////////////////////
    //
    //  World
    //
    ////////////////////////////////////////////////////////////

    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(Size_World.z());

    G4cout << "Computed tolerance = "
           << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance() / mm
           << " mm" << G4endl;

    auto World_Box = new G4Box("World_Box", Size_World.x() / 2, Size_World.y() / 2, Size_World.z() / 2);
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

void DetectorConstruction::DefineTagTracker() {
    ////////////////////////////////////////////////////////////
    //
    //  Tagging Tracker
    //
    ////////////////////////////////////////////////////////////

    auto TagRegion_Box = new G4Box("TagTrk", Size_TagRegion.x() / 2., Size_TagRegion.y() / 2., Size_TagRegion.z() / 2.);
    TagRegion_LV = new G4LogicalVolume(TagRegion_Box, TagRegion_Mat, "TAGTrK", nullptr, nullptr, nullptr);
    new G4PVPlacement(nullptr, Pos_TagRegion, TagRegion_LV, "TAGTRK", World_LV, false, 0, fCheckOverlaps);
    //TagRegion_LV->SetVisAttributes(G4VisAttributes::GetInvisible());

    auto TagTrk1 = new TrkConstruct("TagTrk1", TagRegion_LV, 0, fCheckOverlaps);
    TagTrk1->SetRotation(0.);
    TagTrk1->SetTrkMaterial(TagTrk_Mat);
    TagTrk1->SetVis(new G4VisAttributes(G4Colour(0.5, 0.5, .0)));
    TagTrk1->LinearPlacement(No_TagTrk, Size_TagTrk, Pos_TagTrk);

    TagTrk_LV1 = TagTrk1->GetTrkLVVector();

    auto TagTrk2 = new TrkConstruct("TagTrk2", TagRegion_LV, 0, fCheckOverlaps);
    TagTrk2->SetZMove(Size_TagTrk[0].z());
    TagTrk2->SetRotation(Tag_Angle);
    TagTrk2->SetTrkMaterial(TagTrk_Mat);
    TagTrk2->SetVis(new G4VisAttributes(G4Colour(0.5, 0.5, .0)));
    TagTrk2->LinearPlacement(No_TagTrk, Size_TagTrk, Pos_TagTrk);

    TagTrk_LV2 = TagTrk2->GetTrkLVVector();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineRecTracker() {
    ////////////////////////////////////////////////////////////
    //
    //  Recoil Tracker
    //
    ////////////////////////////////////////////////////////////

    auto RecRegion_Box = new G4Box("RecTrk", Size_RecRegion.x() / 2., Size_RecRegion.y() / 2., Size_RecRegion.z() / 2.);
    RecRegion_LV = new G4LogicalVolume(RecRegion_Box, RecRegion_Mat, "RECTrK", nullptr, nullptr, nullptr);
    new G4PVPlacement(nullptr, Pos_RecRegion, RecRegion_LV, "RECTRK", World_LV, false, 0, fCheckOverlaps);
    //RecRegion_LV->SetVisAttributes(G4VisAttributes::GetInvisible());

    auto RecTrk1 = new TrkConstruct("RecTrk1", RecRegion_LV, 0, fCheckOverlaps);
    RecTrk1->SetRotation(0);
    RecTrk1->SetTrkMaterial(RecTrk_Mat);
    RecTrk1->SetVis(new G4VisAttributes(G4Colour(0.5, 0.5, .0)));
    RecTrk1->LinearPlacement(No_RecTrk, Size_RecTrk, Pos_RecTrk);

    RecTrk_LV1 = RecTrk1->GetTrkLVVector();

    auto RecTrk2 = new TrkConstruct("RecTrk2", RecRegion_LV, 0, fCheckOverlaps);
    RecTrk2->SetZMove(Size_RecTrk[0].z());
    RecTrk2->SetRotation(Rec_Angle);
    RecTrk2->SetTrkMaterial(RecTrk_Mat);
    RecTrk2->SetVis(new G4VisAttributes(G4Colour(0.5, 0.5, .0)));
    RecTrk2->LinearPlacement(No_RecTrk, Size_RecTrk, Pos_RecTrk);

    RecTrk_LV2 = RecTrk2->GetTrkLVVector();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField() {

    /*                          */
    /* Construct Magnetic Field */
    /*                          */

    G4bool allLocal = true;
    // Tagging Tracker
    if (build_TagTrk) {
        G4MagneticField *TagTrkMagField;
        TagTrkMagField = new G4UniformMagField(G4ThreeVector(0., TagTrk_MagField_y, 0.));
        auto *TagTrkFieldMng = new G4FieldManager();
        TagTrkFieldMng->SetDetectorField(TagTrkMagField);
        TagTrkFieldMng->CreateChordFinder(TagTrkMagField);

        TagRegion_LV->SetFieldManager(TagTrkFieldMng, allLocal);
    }
    // Recoil Tracker
    if (build_RecTrk) {
        G4MagneticField *RecTrkMagField;
        RecTrkMagField = new G4UniformMagField(G4ThreeVector(0., RecTrk_MagField_y, 0.));
        auto *RecTrkFieldMng = new G4FieldManager();
        RecTrkFieldMng->SetDetectorField(RecTrkMagField);
        RecTrkFieldMng->CreateChordFinder(RecTrkMagField);

        RecRegion_LV->SetFieldManager(RecTrkFieldMng, allLocal);
    }
    /*                              */
    /* Construct Sensitive Detector */
    /*                              */

    if (build_TagTrk) {
        auto *TagTrkSD1 = new DetectorSD(0, "TagTrk1", G4ThreeVector(1, 1, No_TagTrk), fRootMng);
        G4SDManager::GetSDMpointer()->AddNewDetector(TagTrkSD1);
        for (itr_LV = TagTrk_LV1.begin(); itr_LV != TagTrk_LV1.end(); itr_LV++)
            (*itr_LV)->SetSensitiveDetector(TagTrkSD1);

        auto *TagTrkSD2 = new DetectorSD(0, "TagTrk2", G4ThreeVector(1, 1, No_TagTrk), fRootMng);
        G4SDManager::GetSDMpointer()->AddNewDetector(TagTrkSD2);
        for (itr_LV = TagTrk_LV2.begin(); itr_LV != TagTrk_LV2.end(); itr_LV++)
            (*itr_LV)->SetSensitiveDetector(TagTrkSD2);
    }

    if (build_RecTrk) {
        auto *RecTrkSD1 = new DetectorSD(0, "RecTrk1", G4ThreeVector(1, 1, No_RecTrk), fRootMng);
        G4SDManager::GetSDMpointer()->AddNewDetector(RecTrkSD1);
        for (itr_LV = RecTrk_LV1.begin(); itr_LV != RecTrk_LV1.end(); itr_LV++)
            (*itr_LV)->SetSensitiveDetector(RecTrkSD1);

        auto *RecTrkSD2 = new DetectorSD(0, "RecTrk2", G4ThreeVector(1, 1, No_RecTrk), fRootMng);
        G4SDManager::GetSDMpointer()->AddNewDetector(RecTrkSD2);
        for (itr_LV = RecTrk_LV2.begin(); itr_LV != RecTrk_LV2.end(); itr_LV++)
            (*itr_LV)->SetSensitiveDetector(RecTrkSD2);
    }

    if (ECAL_Selection == 1) ECAL_Con1->BuildSD(fRootMng);
    if (ECAL_Selection == 2) ECAL_Con2->BuildSD(fRootMng);

    HCAL_Con->BuildSD(fRootMng);

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

    if (fRootMng->GetifBiasECAL()) {
        if (ECAL_Selection == 1) ECAL_Con1->BuildBias(bias);
        if (ECAL_Selection == 2) ECAL_Con2->BuildBias(bias);
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetTagTrkMagField(G4double in) {
    TagTrk_MagField_y = in;

    G4bool allLocal = false;
    // Tagging Tracker
    G4MagneticField *TagTrkMagField = new G4UniformMagField(G4ThreeVector(0., TagTrk_MagField_y, 0.));
    auto *TagTrkFieldMng = new G4FieldManager();
    TagTrkFieldMng->SetDetectorField(TagTrkMagField);
    TagTrkFieldMng->CreateChordFinder(TagTrkMagField);

    TagRegion_LV->SetFieldManager(TagTrkFieldMng, allLocal);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetRecTrkMagField(G4double in) {
    RecTrk_MagField_y = in;

    G4bool allLocal = false;
    // Recging Tracker
    G4MagneticField *RecTrkMagField = new G4UniformMagField(G4ThreeVector(0., RecTrk_MagField_y, 0.));
    auto *RecTrkFieldMng = new G4FieldManager();
    RecTrkFieldMng->SetDetectorField(RecTrkMagField);
    RecTrkFieldMng->CreateChordFinder(RecTrkMagField);

    RecRegion_LV->SetFieldManager(RecTrkFieldMng, allLocal);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void DetectorConstruction::SetOptical(G4bool in) {
    fRootMng->SetOptical(in);

}

void DetectorConstruction::SaveGeometry() {

    /* Save GDML to ROOT File */
    G4GDMLParser parser;
    parser.Write("geometry.gdml", World_PV);

    fRootMng->FillGeometry("geometry.gdml");

}