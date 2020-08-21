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
 
#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "DetectorSD.hh"
#include "RootManager.hh"
#include "BOptrChangeCrossSection.hh"
#include "BOptrMultiParticleChangeCrossSection.hh"
#include "CALConstruct.hh"
#include "TrkConstruct.hh"

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

#include <iterator>

//#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
DetectorConstruction::DetectorConstruction(RootManager* rootMng)
{
    fMessenger = new DetectorMessenger(this);
    fRootMng   = rootMng;
    fCheckOverlaps = false;
    fStepLimit = nullptr;

    build_Target = true;
    build_TagTrk = true;
    build_RecTrk = true;
    build_ECAL = true; 
    build_HCAL = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
DetectorConstruction::~DetectorConstruction()
{
    delete fStepLimit;
    delete fMessenger;             
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // Define materials
    DefineMaterials();
    DefineParameters();
    
    // Define volumes
    return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
    G4NistManager* nistManager = G4NistManager::Instance();
    
    // Material definition 
    G4Element* elP  = nistManager->FindOrBuildElement("P");
    G4Element* elW  = nistManager->FindOrBuildElement("W");
    G4Element* elO  = nistManager->FindOrBuildElement("O");
    G4Element* elN  = nistManager->FindOrBuildElement("N");
    G4Element* elLu = nistManager->FindOrBuildElement("Lu");
    G4Element* elSi = nistManager->FindOrBuildElement("Si");
    G4Element* elH  = nistManager->FindOrBuildElement("H");
    G4Element* elC  = nistManager->FindOrBuildElement("C");
    G4Element* elY  = nistManager->FindOrBuildElement("Y");
    
    // Vacuum
    auto* Vacuum = new G4Material("vacuum", universe_mean_density, 2);
    Vacuum-> AddElement(elN, .7);
    Vacuum-> AddElement(elO, .3);
                        
    // Defining LYSO
    auto* LSO = new G4Material("LSO", 7.4*g/cm3, 3);
    LSO->AddElement(elLu, 2);
    LSO->AddElement(elSi, 1);
    LSO->AddElement(elO,  5);
    
    auto* YSO = new G4Material("YSO", 4.5*g/cm3, 3);
    YSO->AddElement(elY,  2);
    YSO->AddElement(elSi, 1);
    YSO->AddElement(elO,  5);
    
    auto* LYSO = new G4Material("LYSO", 7.1*g/cm3, 2, kStateSolid);
    LYSO->AddMaterial(LSO, 90*perCent);
    LYSO->AddMaterial(YSO, 10*perCent);
    
    // Defining Polystyrene
    auto* PS = new G4Material("Polystyrene", 1.04*g/cm3, 2);
    PS->AddElement(elC, 8);
    PS->AddElement(elH, 8);

    // Define PWO4
    auto* PWO4 = new G4Material("PWO4", 5.26*g/cm3, 3);
    PWO4->AddElement(elP, 1);
    PWO4->AddElement(elW, 1);
    PWO4->AddElement(elO, 4);
    
    // Define Carbon Fiber
    auto* CarbonFiber = new G4Material("CarbonFiber", 1.93*g/cm3, 1);
    CarbonFiber->AddElement(elC,1);
    
    // Build By Nist
    nistManager->FindOrBuildMaterial("G4_W");
    nistManager->FindOrBuildMaterial("G4_Si");
    nistManager->FindOrBuildMaterial("G4_Al");
    nistManager->FindOrBuildMaterial("G4_Fe");

    /* Define Optical Properties */
    
    if ( fRootMng->GetOptical() ) {
    //
    // ------------ Generate & Add Material Properties Table ------------
    //
      
        G4double photonEnergy[] = { 2.273*eV, 3.064*eV };
            
        const G4int nEntries = sizeof(photonEnergy)/sizeof(G4double);

        G4double RefractionIdx[nEntries] = {1.806,1.833};
        G4double AbsorptionLength[nEntries] = {100.*cm,100.*cm};

        auto* MPT = new G4MaterialPropertiesTable();
        MPT->AddProperty("RINDEX", photonEnergy, RefractionIdx, nEntries);
        MPT->AddProperty("ABSLENGTH", photonEnergy, AbsorptionLength, nEntries);

        G4double ScintEnergy[nEntries] = {3.26*eV, 3.44*eV};
        G4double ScintFast[nEntries] = {1.0, 1.0};

        MPT->AddProperty("FASTCOMPONENT",ScintEnergy,ScintFast,nEntries);

        MPT->AddConstProperty("SCINTILLATIONYIELD",30./MeV);
        MPT->AddConstProperty("RESOLUTIONSCALE",1.);
        MPT->AddConstProperty("FASTTIMECONSTANT",30.*ns);
        MPT->AddConstProperty("YIELDRATIO",1.);

        LYSO->SetMaterialPropertiesTable(MPT);
    }

    // Print materials
    //G4cout << *(G4Material::GetMaterialTable()) < < G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineParameters()
{
    /* Define all the parameter used */
    /////////////////////////
    //  APD
    /////////////////////////
    APD_Mat  = G4Material::GetMaterial("G4_Si");
    APD_Size = G4ThreeVector( 1*cm, 1*cm, 1*mm );

    Glue_Mat  = G4Material::GetMaterial("G4_W");
    Glue_Size = G4ThreeVector( 1*cm, 1*cm, 0.1*mm );

    /////////////////////////
    //  EM Field
    /////////////////////////
    TagTrk_MagField_x = -1.5*tesla;
    RecTrk_MagField_x = -0.5*tesla;
    /////////////////////////
    //  Target
    /////////////////////////
    Target_Mat  = G4Material::GetMaterial("G4_W");
    Target_Size = G4ThreeVector( 10*cm, 20*cm, 350*um );
    Target_Pos  = G4ThreeVector(  0*cm,  0*cm,   0*cm );
    Trk_Tar_Dis = 7.5*mm;

    /////////////////////////
    //  Tagging Tracker
    /////////////////////////
    TagTrk_Mat    = G4Material::GetMaterial("G4_Si");
    TagRegion_Mat = G4Material::GetMaterial("vacuum");

    Tag_Angle = 0.1*radian;

    No_TagTrk = 7; 
    Size_TagTrk[0] = G4ThreeVector( Target_Size.x(), Target_Size.y(), 0.1*mm);
    Size_TagTrk[1] = G4ThreeVector( Target_Size.x(), Target_Size.y(), 0.1*mm);
    Size_TagTrk[2] = G4ThreeVector( Target_Size.x(), Target_Size.y(), 0.1*mm);
    Size_TagTrk[3] = G4ThreeVector( Target_Size.x(), Target_Size.y(), 0.1*mm);
    Size_TagTrk[4] = G4ThreeVector( Target_Size.x(), Target_Size.y(), 0.1*mm);
    Size_TagTrk[5] = G4ThreeVector( Target_Size.x(), Target_Size.y(), 0.1*mm);
    Size_TagTrk[6] = G4ThreeVector( Target_Size.x(), Target_Size.y(), 0.1*mm);

    Pos_TagTrk[0] = G4ThreeVector( 0*cm, 0*cm, -30.*cm);
    Pos_TagTrk[1] = G4ThreeVector( 0*cm, 0*cm, -20.*cm);
    Pos_TagTrk[2] = G4ThreeVector( 0*cm, 0*cm, -10.*cm);
    Pos_TagTrk[3] = G4ThreeVector( 0*cm, 0*cm,   0.*cm);
    Pos_TagTrk[4] = G4ThreeVector( 0*cm, 0*cm,  10.*cm);
    Pos_TagTrk[5] = G4ThreeVector( 0*cm, 0*cm,  20.*cm);
    Pos_TagTrk[6] = G4ThreeVector( 0*cm, 0*cm,  30.*cm);

    Size_TagRegion = G4ThreeVector( Target_Size.x(), Target_Size.y(), 60*cm + Size_TagTrk[0].z() );
    Pos_TagRegion  = G4ThreeVector(  0*cm,  0*cm, -30*cm - Trk_Tar_Dis - (Size_TagTrk[0].z() + Target_Size.z())/2 );

    /////////////////////////
    //  Recoil Tracker
    /////////////////////////
    RecTrk_Mat    = G4Material::GetMaterial("G4_Si");
    RecRegion_Mat = G4Material::GetMaterial("vacuum");

    Rec_Angle = 0.1*radian;

    No_RecTrk = 6; 
    Size_RecTrk[0] = G4ThreeVector( Target_Size.x(), 20*cm, 0.1*mm);
    Size_RecTrk[1] = G4ThreeVector( Target_Size.x(), 20*cm, 0.1*mm);
    Size_RecTrk[2] = G4ThreeVector( Target_Size.x(), 20*cm, 0.1*mm);
    Size_RecTrk[3] = G4ThreeVector( Target_Size.x(), 20*cm, 0.1*mm);
    Size_RecTrk[4] = G4ThreeVector( Target_Size.x(), 20*cm, 0.1*mm);
    Size_RecTrk[5] = G4ThreeVector( Target_Size.x(), 20*cm, 0.1*mm);

    Pos_RecTrk[0] = G4ThreeVector( 0*cm, 0*cm, -86.25*mm);
    Pos_RecTrk[1] = G4ThreeVector( 0*cm, 0*cm, -71.25*mm);
    Pos_RecTrk[2] = G4ThreeVector( 0*cm, 0*cm, -55.25*mm);
    Pos_RecTrk[3] = G4ThreeVector( 0*cm, 0*cm, -40.25*mm);
    Pos_RecTrk[4] = G4ThreeVector( 0*cm, 0*cm,  -4.25*mm);
    Pos_RecTrk[5] = G4ThreeVector( 0*cm, 0*cm,  86.25*mm);

    Size_RecRegion = G4ThreeVector( Size_RecTrk[5].x(), Size_RecTrk[5].y(), 17.25*cm + 0.1*mm );
    Pos_RecRegion  = G4ThreeVector(  0*cm,  0*cm, 0.5*Size_RecRegion.z() + 7.5*mm  + 0.5*350*um );

    /////////////////////////
    //  ECAL
    /////////////////////////
    ECALRegion_Mat = G4Material::GetMaterial("CarbonFiber");
    ECAL_Center_Mat = G4Material::GetMaterial("LYSO");
    ECAL_Outer_Mat = G4Material::GetMaterial("PWO4"); 
    ECAL_Wrap_Mat  = G4Material::GetMaterial("G4_Al");
    
    ECAL_Center_Wrap_Size = G4ThreeVector( 0.3*mm, 0.3*mm, 0.3*mm );
    ECAL_Center_Size      = G4ThreeVector( 1*cm, 1*cm, 36*cm + 35*ECAL_Center_Wrap_Size.z() );
    ECAL_Center_Module_No = G4ThreeVector( 6, 6, 1 );

    ECAL_Outer_Wrap_Size = ECAL_Center_Wrap_Size;
    ECAL_Outer_Size_Dir = G4ThreeVector( 20*cm+ 19*ECAL_Outer_Wrap_Size.x(), 1*cm, 1*cm );
    ECAL_Outer_Mod_No_Dir = G4ThreeVector( 1, 20, 36 );
    ECAL_Outer_Module_No = G4ThreeVector( 2, 2, 1 );
    ECAL_Module_Gap = 0.5*mm;

    Size_ECALRegion = G4ThreeVector(0,0,0);
    Size_ECALRegion.setX( ECAL_Outer_Module_No.x() * (ECAL_Outer_Size_Dir.x() + ECAL_Outer_Wrap_Size.x() ) + ECAL_Module_Gap );
    Size_ECALRegion.setY( ECAL_Outer_Module_No.y() * ECAL_Outer_Mod_No_Dir.y() * (ECAL_Outer_Size_Dir.y() + ECAL_Outer_Wrap_Size.y() ) + ECAL_Module_Gap );
    Size_ECALRegion.setZ( ECAL_Outer_Mod_No_Dir.z()* (ECAL_Outer_Size_Dir.z() + ECAL_Outer_Wrap_Size.z() ) + ECAL_Module_Gap );
    
    Pos_ECALRegion = G4ThreeVector( 0, 0, 0.5*Size_ECALRegion.z() + Pos_RecRegion.z() + 0.5*Size_RecRegion.z() + 1.0*mm );

    G4cout<<" ==> ECAL starts from "<< Pos_ECALRegion.z() - Size_ECALRegion.z()/2 <<G4endl;

    
    /////////////////////////
    //  HCAL
    /////////////////////////
    HCAL_Absorber_Mat  = G4Material::GetMaterial("G4_Fe");
    HCALRegion_Mat = G4Material::GetMaterial("CarbonFiber");
    HCAL_Mat = G4Material::GetMaterial("Polystyrene"); 
    HCAL_Wrap_Mat  = G4Material::GetMaterial("G4_Al");
    
    HCAL_Absorber_Thickness = 3*cm;
    HCAL_Wrap_Size = G4ThreeVector( 0.3*mm, 0.3*mm, 0.3*mm );
    HCAL_Size_Dir = G4ThreeVector( 100*cm+ 19*HCAL_Wrap_Size.x(), 5*cm, 1*cm );
    //HCAL_Mod_No_Dir = G4ThreeVector( 1, 20, 2 );
    HCAL_Mod_No_Dir = G4ThreeVector( 1, 20, 120 );
    HCAL_Module_No = G4ThreeVector( 3, 3, 1 );
    HCAL_Module_Gap = 0.5*mm;

    Size_HCALRegion = G4ThreeVector(0,0,0);
    Size_HCALRegion.setX( HCAL_Module_No.x() * (HCAL_Size_Dir.x() + HCAL_Wrap_Size.x() ) + HCAL_Module_Gap * (HCAL_Module_No.x()-1) );
    Size_HCALRegion.setY( HCAL_Module_No.y() * HCAL_Mod_No_Dir.y() * (HCAL_Size_Dir.y() + HCAL_Wrap_Size.y() ) + HCAL_Module_Gap * (HCAL_Module_No.y()-1) );
    Size_HCALRegion.setZ( HCAL_Mod_No_Dir.z()/2*(2*(HCAL_Size_Dir.z() + HCAL_Wrap_Size.z()) + HCAL_Absorber_Thickness) + HCAL_Module_Gap * (HCAL_Module_No.z()-1) );
    
    Pos_HCALRegion = G4ThreeVector( 0, 0, 0.5*Size_HCALRegion.z() + Pos_RecRegion.z() + 0.5*Size_RecRegion.z() + 1.0*mm + Size_ECALRegion.z() );

    G4cout<<" ==> HCAL starts from "<< Pos_HCALRegion.z() - Size_HCALRegion.z()/2 <<G4endl;

    /////////////////////////
    //  World
    /////////////////////////
    World_Mat = G4Material::GetMaterial("vacuum"); 
    auto l = 2.0*( Pos_HCALRegion.z() + Size_HCALRegion.x() );
    Size_World = G4ThreeVector( l, l, l);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{

    DefineWorld();      // Build World
    if ( build_Target ) DefineTarget();     // Build Target
    if ( build_TagTrk ) DefineTagTracker(); // Build Tagging Tracker
    if ( build_RecTrk ) DefineRecTracker(); // Build Recoil Tracker
    if ( build_ECAL   ) DefineECAL();       // Build ECal
    if ( build_HCAL   ) DefineHCAL();       // Build HCal

    // Set User Limit 
    G4double maxStep = 10*mm;
    fStepLimit = new G4UserLimits(maxStep, DBL_MAX, 200*s);
    
    // Book RootMng
    fRootMng->book();
    G4cout<<"[Root Manager] ==> Root Manager initialized ..." <<G4endl;     
    G4cout<<"[Root Manager] ==> Output File"<<fRootMng->GetOutFileName()<<" created ..." <<G4endl;     

    //SetBiasLayer();

    return World_PV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void DetectorConstruction::DefineWorld()
{
    ////////////////////////////////////////////////////////////
    //
    //  World
    //
    ////////////////////////////////////////////////////////////

    G4GeometryManager::GetInstance()->SetWorldMaximumExtent( Size_World.z() );
    
    G4cout << "Computed tolerance = "
           << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
           << " mm" << G4endl;
    
    auto World_Box = new G4Box("World_Box", Size_World.x()/2, Size_World.y()/2, Size_World.z()/2);
    World_LV  = new G4LogicalVolume( World_Box, World_Mat, "World_LV");
    World_PV  = new G4PVPlacement(nullptr, G4ThreeVector(), World_LV, "World", nullptr, false, 0, fCheckOverlaps);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineTarget()
{
    ////////////////////////////////////////////////////////////
    //
    //  Target
    //
    ////////////////////////////////////////////////////////////

    auto Target = new TrkConstruct("Target", World_LV, 0, fCheckOverlaps);
    Target->SetSizeXYZ( Target_Size );
    Target->SetPosXYZ( Target_Pos );
    Target->SetTrkMaterial( Target_Mat );
    Target->SetVis(new G4VisAttributes(G4Colour(0,1.0,0)));
    Target->Construct();
    
    Target_LV = Target->GetTrkLVVector();

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineTagTracker()
{
    ////////////////////////////////////////////////////////////
    //
    //  Tagging Tracker
    //
    ////////////////////////////////////////////////////////////

    auto TagRegion_Box = new G4Box("TagTrk", Size_TagRegion.x()/2., Size_TagRegion.y()/2., Size_TagRegion.z()/2. );
    TagRegion_LV  = new G4LogicalVolume( TagRegion_Box, TagRegion_Mat, "TAGTrK",nullptr,nullptr,nullptr);
    new G4PVPlacement(nullptr, Pos_TagRegion, TagRegion_LV, "TAGTRK", World_LV, false, 0, fCheckOverlaps);
    TagRegion_LV->SetVisAttributes(G4VisAttributes::GetInvisible());
    
    auto TagTrk1 = new TrkConstruct("TagTrk1", TagRegion_LV, 0, fCheckOverlaps);
    TagTrk1->SetRotation( 0. );
    TagTrk1->SetTrkMaterial( TagTrk_Mat );
    TagTrk1->SetVis(new G4VisAttributes(G4Colour(0.5,0.5,.0)));
    TagTrk1->LinearPlacement( No_TagTrk, Size_TagTrk, Pos_TagTrk);
    
    TagTrk_LV1 = TagTrk1->GetTrkLVVector();
    
    auto TagTrk2 = new TrkConstruct("TagTrk2", TagRegion_LV, 0, fCheckOverlaps);
    TagTrk2->SetZMove( Size_TagTrk[0].z() );
    TagTrk2->SetRotation( Tag_Angle );
    TagTrk2->SetTrkMaterial( TagTrk_Mat );
    TagTrk2->SetVis(new G4VisAttributes(G4Colour(0.5,0.5,.0)));
    TagTrk2->LinearPlacement( No_TagTrk, Size_TagTrk, Pos_TagTrk);
    
    TagTrk_LV2 = TagTrk2->GetTrkLVVector();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineRecTracker()
{
    ////////////////////////////////////////////////////////////
    //
    //  Recoil Tracker
    //
    ////////////////////////////////////////////////////////////

    auto RecRegion_Box = new G4Box("RecTrk", Size_RecRegion.x()/2., Size_RecRegion.y()/2., Size_RecRegion.z()/2. );
    RecRegion_LV  = new G4LogicalVolume( RecRegion_Box, RecRegion_Mat, "RECTrK",nullptr,nullptr,nullptr);
    new G4PVPlacement(nullptr, Pos_RecRegion, RecRegion_LV, "RECTRK", World_LV, false, 0, fCheckOverlaps);
    RecRegion_LV->SetVisAttributes(G4VisAttributes::GetInvisible());
    
    auto RecTrk1 = new TrkConstruct("RecTrk1", RecRegion_LV, 0, fCheckOverlaps);
    RecTrk1->SetRotation( 0 );
    RecTrk1->SetTrkMaterial( RecTrk_Mat );
    RecTrk1->SetVis(new G4VisAttributes(G4Colour(0.5,0.5,.0)));
    RecTrk1->LinearPlacement( No_RecTrk, Size_RecTrk, Pos_RecTrk);
    
    RecTrk_LV1 = RecTrk1->GetTrkLVVector();

    auto RecTrk2 = new TrkConstruct("RecTrk2", RecRegion_LV, 0, fCheckOverlaps);
    RecTrk2->SetZMove( Size_RecTrk[0].z() );
    RecTrk2->SetRotation( Rec_Angle );
    RecTrk2->SetTrkMaterial( RecTrk_Mat );
    RecTrk2->SetVis(new G4VisAttributes(G4Colour(0.5,0.5,.0)));
    RecTrk2->LinearPlacement( No_RecTrk, Size_RecTrk, Pos_RecTrk);
    
    RecTrk_LV2 = RecTrk2->GetTrkLVVector();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineECAL()
{
    ////////////////////////////////////////////////////////////
    //
    //  ECAL
    //
    ////////////////////////////////////////////////////////////

    auto ECAL_Box = new G4Box("ecal", Size_ECALRegion.x()/2, Size_ECALRegion.y()/2, Size_ECALRegion.z()/2);
    auto ECal_LV = new G4LogicalVolume( ECAL_Box, ECALRegion_Mat, "ECAL",nullptr,nullptr,nullptr);
    new G4PVPlacement(nullptr, Pos_ECALRegion, ECal_LV, "ECAL", World_LV, false, 0, fCheckOverlaps);
    ECal_LV->SetVisAttributes(G4VisAttributes::GetInvisible());
    
    /* Building Center Calorimeter with LYSO
     * Cell Size: 1*1*36 cm^3
     * Module No: 6*6 
     */
    auto ECAL_Center = new CALConstruct("ECAL_Center", ECal_LV, 0, true, true, fRootMng->GetOptical(), fCheckOverlaps);
    ECAL_Center->SetSizeXYZ( ECAL_Center_Size.x()/2., ECAL_Center_Size.y()/2., ECAL_Center_Size.z()/2. );
    ECAL_Center->SetWrapSizeXYZ( ECAL_Center_Wrap_Size.x()/2., ECAL_Center_Wrap_Size.y()/2., ECAL_Center_Wrap_Size.z()/2. );
    ECAL_Center->SetCALMaterial(ECAL_Center_Mat);
    ECAL_Center->SetWrapMaterial( ECAL_Wrap_Mat );
    ECAL_Center->SetVis(new G4VisAttributes(G4Colour(0.5,0.5,.0)));
    ECAL_Center->SetAPDSize(APD_Size, Glue_Size);
    ECAL_Center->SetAPDMat(APD_Mat, Glue_Mat);
    ECAL_Center->MatrixPlacement(ECAL_Center_Module_No.x(),ECAL_Center_Module_No.y(),ECAL_Center_Module_No.z(),G4ThreeVector(0,0,0));
    
    ECAL_Center_LV = ECAL_Center->GetCaloLVVector();

    /* Building Surrounding Calorimeter with PWO4
     * Cell Size: 1*20*1 cm^3 or 20*1*1 cm^3
     * Totally 4 modules
     */
    int nECAL_Outer = ECAL_Outer_Module_No.x()*ECAL_Outer_Module_No.y()*ECAL_Outer_Module_No.z();
    for (int ip = 1; ip <= nECAL_Outer; ip++)
    {
        double w1 = pow(-1,(ip%2))   * Size_ECALRegion.x()/4. ;
        double w2 = pow(-1,(ip-1)/2) * Size_ECALRegion.x()/4. ;
        auto ECAL_Outer = new CALConstruct("ECAL_Outer_"+std::to_string(ip), ECal_LV, 0, true, true, fRootMng->GetOptical(), fCheckOverlaps);
        ECAL_Outer->SetSizeXYZ( ECAL_Outer_Size_Dir.x()/2., ECAL_Outer_Size_Dir.y()/2., ECAL_Outer_Size_Dir.z()/2. );
        ECAL_Outer->SetWrapSizeXYZ( ECAL_Outer_Wrap_Size.x()/2., ECAL_Outer_Wrap_Size.y()/2., ECAL_Outer_Wrap_Size.z()/2. );
        ECAL_Outer->SetCALMaterial(ECAL_Outer_Mat);
        ECAL_Outer->SetWrapMaterial( ECAL_Wrap_Mat );
        ECAL_Outer->SetVis(new G4VisAttributes(G4Colour(0.4,0.57,0.6)));
        ECAL_Outer->SetAPDSize(APD_Size, Glue_Size);
        ECAL_Outer->SetAPDMat(APD_Mat, Glue_Mat);
        ECAL_Outer->MatrixPlacementXYRemoved(ECAL_Outer_Mod_No_Dir.x(),ECAL_Outer_Mod_No_Dir.y(),ECAL_Outer_Mod_No_Dir.z(),G4ThreeVector( w1, w2 ,0), 3, ip);

        auto tmp_LV = ECAL_Outer->GetCaloLVVector();
        //ECAL_Outer_LV.insert( ECAL_Outer_LV.end(), tmp_LV.begin(), tmp_LV.end() );
        ECAL_Outer_LV[ip-1] = ECAL_Outer->GetCaloLVVector();
    }


    // Production Cut
    //G4Region* emCal = new G4Region("EMCal");
    //emCal->AddRootLogicalVolume(ECal_LV);
    //G4ProductionCuts* cuts = new G4ProductionCuts;
    //cuts->SetProductionCut(1.0*nm,G4ProductionCuts::GetIndex("gamma"));
    //cuts->SetProductionCut(1.0*nm,G4ProductionCuts::GetIndex("opticalphoton"));
    //cuts->SetProductionCut(1.0*nm,G4ProductionCuts::GetIndex("e-"));
    //cuts->SetProductionCut(1.0*nm,G4ProductionCuts::GetIndex("e+"));

    //emCal->SetProductionCuts(cuts);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineHCAL()
{
    ////////////////////////////////////////////////////////////
    //
    //  HCAL
    //
    ////////////////////////////////////////////////////////////

    auto HCAL_Box = new G4Box("hcal", Size_HCALRegion.x()/2, Size_HCALRegion.y()/2, Size_HCALRegion.z()/2);
    auto HCAL_LV = new G4LogicalVolume( HCAL_Box, HCALRegion_Mat, "HCAL",nullptr,nullptr,nullptr);
    new G4PVPlacement(nullptr, Pos_HCALRegion, HCAL_LV, "HCAL", World_LV, false, 0, fCheckOverlaps);
    //HCAL_LV->SetVisAttributes(G4VisAttributes::GetInvisible());
    
    /* Building Surrounding Calorimeter with Scintillator
     * Cell Size: 2*100*1 cm^3 or 100*2*1 cm^3
     * Totally 3*3*1 modules
     */
    
    for (int iy = 0; iy < HCAL_Module_No.y(); iy++ ) {
        for (int ix = 0; ix < HCAL_Module_No.x(); ix++ ) {
            double wx = -Size_HCALRegion.x()*0.5 + (Size_HCALRegion.x()/HCAL_Module_No.x()*(0.5+ix) );
            double wy = -Size_HCALRegion.y()*0.5 + (Size_HCALRegion.y()/HCAL_Module_No.y()*(0.5+iy) ); 

            auto HCAL = new CALConstruct("HCAL_"+std::to_string( (int)(ix + iy*HCAL_Module_No.x()) ), HCAL_LV, 0, true, true, fRootMng->GetOptical(), fCheckOverlaps);
            HCAL->SetSizeXYZ( HCAL_Size_Dir.x()/2., HCAL_Size_Dir.y()/2., HCAL_Size_Dir.z()/2. );
            HCAL->SetWrapSizeXYZ( HCAL_Wrap_Size.x()/2., HCAL_Wrap_Size.y()/2., HCAL_Wrap_Size.z()/2. );
            HCAL->SetCALMaterial(HCAL_Mat);
            HCAL->SetWrapMaterial( HCAL_Wrap_Mat );
            HCAL->SetVis(new G4VisAttributes(G4Colour(0.2,0.37,0.8)));
            HCAL->SetAPDSize(APD_Size, Glue_Size);
            HCAL->SetAPDMat(APD_Mat, Glue_Mat);
            HCAL->MatrixPlacementXYwithAbsorber(HCAL_Mod_No_Dir.x(),HCAL_Mod_No_Dir.y(),HCAL_Mod_No_Dir.z(), G4ThreeVector( wx, wy ,0), HCAL_Absorber_Thickness, HCAL_Absorber_Mat);

            HCAL_SD_LV[(int)(ix + iy*HCAL_Module_No.x())] = HCAL->GetCaloLVVector();
        }
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
    
    /*                          */
    /* Construct Magnetic Field */
    /*                          */

    G4bool allLocal = true;
    // Tagging Tracker
    if ( build_TagTrk ) {
        G4MagneticField* TagTrkMagField = new G4UniformMagField(G4ThreeVector( TagTrk_MagField_x, 0., 0.));
        auto* TagTrkFieldMng = new G4FieldManager();
        TagTrkFieldMng->SetDetectorField(TagTrkMagField);
        TagTrkFieldMng->CreateChordFinder(TagTrkMagField);

        TagRegion_LV->SetFieldManager( TagTrkFieldMng, allLocal );
    }
    // Recoil Tracker
    if ( build_RecTrk ) {
        G4MagneticField* RecTrkMagField = new G4UniformMagField(G4ThreeVector( RecTrk_MagField_x, 0., 0.));
        auto* RecTrkFieldMng = new G4FieldManager();
        RecTrkFieldMng->SetDetectorField(RecTrkMagField);
        RecTrkFieldMng->CreateChordFinder(RecTrkMagField);

        RecRegion_LV->SetFieldManager( RecTrkFieldMng, allLocal );
    }
    /*                              */
    /* Construct Sensitive Detector */
    /*                              */

    if ( build_TagTrk ) {
        auto* TagTrkSD1 = new DetectorSD( 0, "TagTrk1", G4ThreeVector( 1, 1, No_TagTrk ), fRootMng);
        G4SDManager::GetSDMpointer()->AddNewDetector( TagTrkSD1 );
        for ( itr_LV = TagTrk_LV1.begin(); itr_LV != TagTrk_LV1.end(); itr_LV++ )
            (*itr_LV)->SetSensitiveDetector( TagTrkSD1 );

        auto* TagTrkSD2 = new DetectorSD( 0, "TagTrk2", G4ThreeVector( 1, 1, No_TagTrk ), fRootMng);
        G4SDManager::GetSDMpointer()->AddNewDetector( TagTrkSD2 );
        for ( itr_LV = TagTrk_LV1.begin(); itr_LV != TagTrk_LV1.end(); itr_LV++ )
            (*itr_LV)->SetSensitiveDetector( TagTrkSD2 );
    }

    if ( build_RecTrk ) {
        auto* RecTrkSD1 = new DetectorSD( 0, "RecTrk1", G4ThreeVector( 1, 1, No_RecTrk ), fRootMng);
        G4SDManager::GetSDMpointer()->AddNewDetector( RecTrkSD1 );
        for ( itr_LV = RecTrk_LV1.begin(); itr_LV != RecTrk_LV1.end(); itr_LV++ )
            (*itr_LV)->SetSensitiveDetector( RecTrkSD1 );

        auto* RecTrkSD2 = new DetectorSD( 0, "RecTrk2", G4ThreeVector( 1, 1, No_RecTrk ), fRootMng);
        G4SDManager::GetSDMpointer()->AddNewDetector( RecTrkSD2 );
        for ( itr_LV = RecTrk_LV1.begin(); itr_LV != RecTrk_LV1.end(); itr_LV++ )
            (*itr_LV)->SetSensitiveDetector( RecTrkSD2 );
    }

    if ( build_ECAL ) {
        auto* ECalSD = new DetectorSD( 1, "ECAL_Center", ECAL_Center_Module_No , fRootMng);
        G4SDManager::GetSDMpointer()->AddNewDetector( ECalSD );
        for ( itr_LV = ECAL_Center_LV.begin(); itr_LV != ECAL_Center_LV.end(); itr_LV++ )
            (*itr_LV)->SetSensitiveDetector( ECalSD );

        const int nECAL_Outer = ECAL_Outer_Module_No.x()*ECAL_Outer_Module_No.y()*ECAL_Outer_Module_No.z();
        DetectorSD* ECalOutSD[nECAL_Outer];
        for (int i=1; i<=nECAL_Outer; i++)
        {
            ECalOutSD[i-1] = new DetectorSD( 2, "ECAL_Outer_"+std::to_string(i), ECAL_Outer_Mod_No_Dir, fRootMng);
            G4SDManager::GetSDMpointer()->AddNewDetector( ECalOutSD[i-1] );
            for ( itr_LV = (ECAL_Outer_LV[i-1]).begin(); itr_LV != (ECAL_Outer_LV[i-1]).end(); itr_LV++ )
                (*itr_LV)->SetSensitiveDetector( ECalOutSD[i-1] );
        }
    }

    if ( build_HCAL ) {
        DetectorSD* HCalSD[9];
        for (int iy = 0; iy < HCAL_Module_No.y(); iy++ ) {
            for (int ix = 0; ix < HCAL_Module_No.x(); ix++ ) {
                int index = (int)(ix + iy*HCAL_Module_No.x());
                HCalSD[index] = new DetectorSD( 2, "HCAL_"+std::to_string( index ), HCAL_Mod_No_Dir , fRootMng);
                G4SDManager::GetSDMpointer()->AddNewDetector( HCalSD[index] );
                for ( itr_LV = HCAL_SD_LV[index].begin(); itr_LV != HCAL_SD_LV[index].end(); itr_LV++ )
                    (*itr_LV)->SetSensitiveDetector( HCalSD[index] );
            }
        }
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetifBias(G4bool in) 
{
    fRootMng->SetifBias(in);
    if (in) SetBiasLayer();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetBiasLayer()
{
    G4cout<<"[Bias Setting] ==> Setting bias layers ..."<<G4endl;
    G4cout<<"[Bias Setting] ==> Setting bias process: "<<fRootMng->GetBiasProcess()<<G4endl;
    G4cout<<"[Bias Setting] ==> Setting bias factor:  "<<fRootMng->GetBiasFactor()<<G4endl;
    G4cout<<"[Bias Setting] ==> Setting min bias E:   "<<fRootMng->GetBiasEmin()<<G4endl;
    G4cout<<"[Bias Setting] ==> Biasing Layer Target: "<<fRootMng->GetifBiasTarget()<<G4endl;
    G4cout<<"[Bias Setting] ==> Biasing Layer ECAL:   "<<fRootMng->GetifBiasECAL()<<G4endl;
    
    /*                           */
    /* define biasing parameters */
    /*                           */

    G4String BiasProcess = fRootMng->GetBiasProcess();
    G4double BiasFactor  = fRootMng->GetBiasFactor();
    G4double BiasEmin    = fRootMng->GetBiasEmin();

    auto bias = new BOptrMultiParticleChangeCrossSection(BiasProcess);
    bias->AddParticle("e-",BiasFactor,BiasEmin,BiasProcess);
    bias->AddParticle("gamma",BiasFactor,BiasEmin,BiasProcess);

    if ( fRootMng->GetifBiasTarget() ) {
        for ( itr_LV = Target_LV.begin(); itr_LV != Target_LV.end(); itr_LV++ )
            bias->AttachTo(*itr_LV);
    }

    if ( fRootMng->GetifBiasECAL() ) {
        for ( itr_LV = ECAL_Center_LV.begin(); itr_LV != ECAL_Center_LV.end(); itr_LV++ )
            bias->AttachTo(*itr_LV);

        for (int i=1; i<=4; i++)
        {
            for ( itr_LV = (ECAL_Outer_LV[i-1]).begin(); itr_LV != (ECAL_Outer_LV[i-1]).end(); itr_LV++ )
                bias->AttachTo(*itr_LV);
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetTagTrkMagField(G4double in) 
{
    TagTrk_MagField_x = in; 

    G4bool allLocal = false;
    // Tagging Tracker
    G4MagneticField* TagTrkMagField = new G4UniformMagField(G4ThreeVector( 0., TagTrk_MagField_x, 0.));
    auto* TagTrkFieldMng = new G4FieldManager();
    TagTrkFieldMng->SetDetectorField(TagTrkMagField);
    TagTrkFieldMng->CreateChordFinder(TagTrkMagField);

    TagRegion_LV->SetFieldManager( TagTrkFieldMng, allLocal );

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetRecTrkMagField(G4double in) 
{
    RecTrk_MagField_x = in; 

    G4bool allLocal = false;
    // Recging Tracker
    G4MagneticField* RecTrkMagField = new G4UniformMagField(G4ThreeVector( 0., RecTrk_MagField_x, 0.));
    auto* RecTrkFieldMng = new G4FieldManager();
    RecTrkFieldMng->SetDetectorField(RecTrkMagField);
    RecTrkFieldMng->CreateChordFinder(RecTrkMagField);

    RecRegion_LV->SetFieldManager( RecTrkFieldMng, allLocal );

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void DetectorConstruction::SetOptical(G4bool in)
{
    fRootMng->SetOptical(in);

}
