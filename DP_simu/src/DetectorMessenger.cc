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
/// \file DetectorMessenger.cc
/// \brief Implementation of the DetectorMessenger class

#include "DP_simu/DetectorMessenger.hh"
#include "DP_simu/DetectorConstruction.hh"
#include "Randomize.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction *Det)
        : G4UImessenger(),
          fDetectorConstruction(Det) {
    fDirectory = new G4UIdirectory("/DP/");
    fDirectory->SetGuidance("UI commands specific to this example.");

    fifSaveGeomCmd = new G4UIcmdWithABool("/DP/ifSaveGeometry", this);
    fifSaveGeomCmd->SetGuidance("whether to Save Geometry to the Event.");
    fifSaveGeomCmd->SetParameterName("if_SaveGeometry", false);
    fifSaveGeomCmd->SetDefaultValue(false);
    fifSaveGeomCmd->AvailableForStates(G4State_PreInit, G4State_Init,G4State_Idle);

    fDetDirectory = new G4UIdirectory("/DP/det/");
    fDetDirectory->SetGuidance("Detector construction control");

    fTargetDirectory = new G4UIdirectory("/DP/det/target/");
    fTargetDirectory->SetGuidance("Target construction control");

    fTagTrkDirectory = new G4UIdirectory("/DP/det/tagTrk/");
    fTagTrkDirectory->SetGuidance("Tagging tracker construction control");

    fRecTrkDirectory = new G4UIdirectory("/DP/det/recTrk/");
    fRecTrkDirectory->SetGuidance("Recoil Tracker construction control");

    fECALDirectory = new G4UIdirectory("/DP/det/ECAL/");
    fECALDirectory->SetGuidance("ECAL construction control");

    fHCALDirecotry = new G4UIdirectory("/DP/det/HCAL/");
    fHCALDirecotry->SetGuidance("HCAL construction control");

    fSetTBxCmd = new G4UIcmdWithADoubleAndUnit("/DP/det/setTByField", this);
    fSetTBxCmd->SetGuidance("Define tagging tracker y-direction magnetic field.");
    fSetTBxCmd->SetGuidance("Magnetic field will be in Y direction.");
    fSetTBxCmd->SetParameterName("tBx", false);
    fSetTBxCmd->SetUnitCategory("Magnetic flux density");
    fSetTBxCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetRBxCmd = new G4UIcmdWithADoubleAndUnit("/DP/det/setRByField", this);
    fSetRBxCmd->SetGuidance("Define recoil tracker y-direction magnetic field.");
    fSetRBxCmd->SetGuidance("Magnetic field will be in y direction.");
    fSetRBxCmd->SetParameterName("rBy", false);
    fSetRBxCmd->SetUnitCategory("Magnetic flux density");
    fSetRBxCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetifBiasCmd = new G4UIcmdWithABool("/DP/setifBias", this);
    fSetifBiasCmd->SetGuidance("Set if define the biasing.");
    fSetifBiasCmd->SetParameterName("if_Bias", false);
    fSetifBiasCmd->SetDefaultValue(false);

    /// ------------- Detector Settings ---------------
    fECAL_SelectionCmd = new G4UIcmdWithAnInteger("/DP/det/selectECAL",this);
    fECAL_SelectionCmd->SetGuidance("Select the build-in ECAL Configuration.");
    fECAL_SelectionCmd->SetParameterName("ECAL_Selection",false);
    fECAL_SelectionCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetifTarget = new G4UIcmdWithABool("/DP/det/ifTarget", this);
    fSetifTarget->SetGuidance("Whether to build Target.");
    fSetifTarget->SetParameterName("build_Target",true);
    fSetifTarget->SetDefaultValue(true);
    fSetifTarget->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetifTagTrk = new G4UIcmdWithABool("/DP/det/ifTagTracker", this);
    fSetifTagTrk->SetGuidance("Wether to build Tagging Tracker.");
    fSetifTagTrk->SetParameterName("build_TagTrk", true);
    fSetifTagTrk->SetDefaultValue(true);
    fSetifTarget->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetifRecTrk = new G4UIcmdWithABool("/DP/det/ifRecTracker", this);
    fSetifRecTrk->SetGuidance("whether to build Recoil Tracker.");
    fSetifRecTrk->SetParameterName("build_RecTrk", true);
    fSetifRecTrk->SetDefaultValue(true);
    fSetifRecTrk->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetifECAL = new G4UIcmdWithABool("/DP/det/ifECAL", this);
    fSetifECAL->SetGuidance("Whether to build ECAL");
    fSetifECAL->SetParameterName("build_ECAL", true);
    fSetifECAL->SetDefaultValue(true);
    fSetifECAL->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetifHCAL = new G4UIcmdWithABool("/DP/det/ifHCAL", this);
    fSetifHCAL->SetGuidance("Whether to build HCAL");
    fSetifHCAL->SetParameterName("build HCAL", true);
    fSetifHCAL->SetDefaultValue(true);
    fSetifHCAL->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetonlyTracker = new G4UIcmdWithoutParameter("/DP/det/onlyTracker", this);
    fSetonlyTracker->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetonlyECAL = new G4UIcmdWithoutParameter("/DP/det/onlyECAL", this);
    fSetonlyECAL->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetonlyHCAL = new G4UIcmdWithoutParameter("/DP/det/onlyHCAL", this);
    fSetonlyHCAL->AvailableForStates(G4State_PreInit, G4State_Idle);

    fReConstruct = new G4UIcmdWithoutParameter("/DP/det/reConstruct", this);
    fReConstruct->SetGuidance("Called after parameter settin is done.");
    fReConstruct->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetTrkTarDis = new G4UIcmdWithADoubleAndUnit("/DP/det/trkTarDis", this);
    fSetTrkTarDis->SetGuidance("Distance between Tracker and Target.");
    fSetTrkTarDis->SetParameterName("TrkTarDis", false);
    fSetTrkTarDis->SetDefaultValue(7.5 * mm);
    fSetTrkTarDis->SetUnitCategory("Length");
    fSetTrkTarDis->AvailableForStates(G4State_PreInit, G4State_Idle);

    // -----------Tracker Setting-------------

    /// /DP/det/tagTrk

    fAddNewTagTrkSize = new G4UIcmdWith3VectorAndUnit("/DP/det/tagTrk/addNewSize", this);
    fAddNewTagTrkSize->SetGuidance("Add New Tagging Tracker Size");
    fAddNewTagTrkSize->SetParameterName("X", "Y", "Z", true);
    fAddNewTagTrkSize->SetDefaultValue(G4ThreeVector(10*cm , 20*cm, 0.01*cm));
    //fAddNewTagTrkSize->SetDefaultUnit("mm");
    fAddNewTagTrkSize->SetUnitCategory("Length");
    fAddNewTagTrkSize->AvailableForStates(G4State_PreInit, G4State_Idle);
    
    fAddNewTagTrkPos = new G4UIcmdWith3VectorAndUnit("/DP/det/tagTrk/addNewPos", this);
    fAddNewTagTrkPos->SetGuidance("Add New Tagging Tracker Position");
    fAddNewTagTrkPos->SetParameterName("X", "Y", "Z", true);
    fAddNewTagTrkPos->SetDefaultValue(G4ThreeVector(0 * cm, 0 * cm, 0 * cm));
    //fAddNewTagTrkPos->SetDefaultUnit("mm");
    fAddNewTagTrkPos->SetUnitCategory("Length");
    fAddNewTagTrkPos->AvailableForStates(G4State_PreInit, G4State_Idle);

    fDelTagTrk = new G4UIcmdWithABool("/DP/det/tagTrk/del", this);
    fDelTagTrk->SetGuidance("Delete all previous Tagging Tracker");
    fDelTagTrk->SetParameterName("DelTagTrk", true);
    fDelTagTrk->SetDefaultValue(true);
    fDelTagTrk->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetTagTrk1Rotation = new G4UIcmdWithADoubleAndUnit("/DP/det/tagTrk/rot1", this);
    fSetTagTrk1Rotation->SetGuidance("Angle of TagTrk 1");
    fSetTagTrk1Rotation->SetParameterName("TagTrk1Rotation", false);
    fSetTagTrk1Rotation->SetDefaultValue(0. * radian);
    fSetTagTrk1Rotation->SetUnitCategory("Angle");
    fSetTagTrk1Rotation->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetTagTrk2Rotation = new G4UIcmdWithADoubleAndUnit("/DP/det/tagTrk/rot2", this);
    fSetTagTrk2Rotation->SetGuidance("Angle of TagTrk 2");
    fSetTagTrk2Rotation->SetParameterName("TagTrk2Rotation", false);
    fSetTagTrk2Rotation->SetDefaultValue(0.1 * radian);
    fSetTagTrk2Rotation->SetUnitCategory("Angle");
    fSetTagTrk2Rotation->AvailableForStates(G4State_PreInit, G4State_Idle);
    
    fSetTagTrk1Color = new G4UIcmdWith3Vector("/DP/det/tagTrk/color1", this);
    fSetTagTrk1Color->SetGuidance("Color of TagTrk 1");
    fSetTagTrk1Color->SetParameterName("R", "G", "B", false);
    fSetTagTrk1Color->SetDefaultValue(G4ThreeVector(0.5, 0.5, 0.));
    fSetTagTrk1Color->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetTagTrk2Color = new G4UIcmdWith3Vector("/DP/det/tagTrk/color2", this);
    fSetTagTrk2Color->SetGuidance("Color of TagTrk 2");
    fSetTagTrk2Color->SetParameterName("R", "G", "B", false);
    fSetTagTrk2Color->SetDefaultValue(G4ThreeVector(0.5, 0.5, 0.));
    fSetTagTrk2Color->AvailableForStates(G4State_PreInit, G4State_Idle);

    /// /DP/det/recTrk

    fAddNewRecTrkSize = new G4UIcmdWith3VectorAndUnit("/DP/det/recTrk/addNewSize", this);
    fAddNewRecTrkSize->SetGuidance("Add New Recoil Tracker Size");
    fAddNewRecTrkSize->SetParameterName("X", "Y", "Z", false);
    fAddNewRecTrkSize->SetUnitCategory("Length");
    fAddNewRecTrkSize->AvailableForStates(G4State_PreInit, G4State_Idle);

    fAddNewRecTrkPos = new G4UIcmdWith3VectorAndUnit("/DP/det/recTrk/addNewPos", this);
    fAddNewRecTrkPos->SetGuidance("Add New Recoil Tracker Postion");
    fAddNewRecTrkPos->SetParameterName("X", "Y", "Z", false);
    fAddNewRecTrkPos->SetUnitCategory("Length");
    fAddNewRecTrkPos->AvailableForStates(G4State_PreInit, G4State_Idle);

    fDelRecTrk = new G4UIcmdWithABool("/DP/det/recTrk/del", this);
    fDelRecTrk->SetGuidance("Delete all Recoil Tracker");
    fDelRecTrk->SetParameterName("DelRecTrk", true);
    fDelRecTrk->SetDefaultValue(true);
    fDelRecTrk->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetRecTrk1Rotation = new G4UIcmdWithADoubleAndUnit("/DP/det/recTrk/rot1", this);
    fSetRecTrk1Rotation->SetGuidance("Angle of RecTrk 1");
    fSetRecTrk1Rotation->SetParameterName("RecTrk1Rotation", false);
    fSetRecTrk1Rotation->SetDefaultValue(0. * radian);
    fSetRecTrk1Rotation->SetUnitCategory("Angle");
    fSetRecTrk1Rotation->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetRecTrk2Rotation = new G4UIcmdWithADoubleAndUnit("/DP/det/recTrk/rot2", this);
    fSetRecTrk2Rotation->SetGuidance("Angle of RecTrk 2");
    fSetRecTrk2Rotation->SetParameterName("RecTrk2Rotation", false);
    fSetRecTrk2Rotation->SetDefaultValue(0.1 * radian);
    fSetRecTrk2Rotation->SetUnitCategory("Angle");
    fSetRecTrk2Rotation->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetRecTrk1Color = new G4UIcmdWith3Vector("/DP/det/recTrk/color1", this);
    fSetRecTrk1Color->SetGuidance("Color of RecTrk 1");
    fSetRecTrk1Color->SetParameterName("R", "G", "B", false);
    fSetRecTrk1Color->SetDefaultValue(G4ThreeVector(0.5, 0.5, 0));
    fSetRecTrk1Color->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetRecTrk2Color = new G4UIcmdWith3Vector("/DP/det/recTrk/color2", this);
    fSetRecTrk2Color->SetGuidance("Color of RecTrk 2");
    fSetRecTrk2Color->SetParameterName("R", "G", "B", false);
    fSetRecTrk2Color->SetDefaultValue(G4ThreeVector(0.5, 0.5, 0));
    fSetRecTrk2Color->AvailableForStates(G4State_PreInit, G4State_Idle);

    /// /DP/det/ECAL

    fSetECALCenterWrapSize = new G4UIcmdWith3VectorAndUnit("/DP/det/ECAL/centerWrapSize", this);
    fSetECALCenterWrapSize->SetGuidance("ECAL Center Wrap Size");
    fSetECALCenterWrapSize->SetParameterName("X", "Y", "Z", false);
    fSetECALCenterWrapSize->SetDefaultValue(G4ThreeVector(0.3 * mm, 0.3 * mm, 0.3 * mm));
    fSetECALCenterWrapSize->SetUnitCategory("Length");
    fSetECALCenterWrapSize->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetECALCenterSize = new G4UIcmdWith3VectorAndUnit("/DP/det/ECAL/centerSize", this);
    fSetECALCenterSize->SetGuidance("ECAL Center Size");
    fSetECALCenterSize->SetParameterName("X", "Y", "Z", false);
    fSetECALCenterSize->SetDefaultValue(G4ThreeVector(2.5 * cm, 2.5 * cm, 2.0 * cm));
    fSetECALCenterSize->SetUnitCategory("Length");
    fSetECALCenterSize->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetECALCenterModuleNo = new G4UIcmdWith3Vector("/DP/det/ECAL/centerModuleNo", this);
    fSetECALCenterModuleNo->SetGuidance("ECAL Center Module Nomber");
    fSetECALCenterModuleNo->SetParameterName("nX", "nY", "nZ", false);
    fSetECALCenterModuleNo->SetDefaultValue(G4ThreeVector(20, 20, 1));
    fSetECALCenterModuleNo->AvailableForStates(G4State_PreInit, G4State_Idle);

    /// /DP/det/HCAL

    fSetHCALWrapSize = new G4UIcmdWith3VectorAndUnit("/DP/det/HCAL/wrapSize", this);
    fSetHCALWrapSize->SetGuidance("HCAL Wrap Size");
    fSetHCALWrapSize->SetParameterName("X", "Y","Z", false);
    fSetHCALWrapSize->SetDefaultValue(G4ThreeVector(0.3 * mm, 0.3 * mm, 0.3 * mm));
    fSetHCALWrapSize->SetUnitCategory("Length");
    fSetHCALWrapSize->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetHCALSizeDir = new G4UIcmdWith3VectorAndUnit("/DP/det/HCAL/sizeDir", this);
    fSetHCALSizeDir->SetGuidance("HCALSizeDir");
    fSetHCALSizeDir->SetParameterName("X", "Y","Z", false);
    fSetHCALSizeDir->SetDefaultValue(G4ThreeVector(100 * cm + 19 * 0.3 * mm , 5 * cm, 1 * cm));
    fSetHCALSizeDir->SetUnitCategory("Length");
    fSetHCALSizeDir->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetHCALModNoDir = new G4UIcmdWith3Vector("/DP/det/HCAL/moduleNoDir", this);
    fSetHCALModNoDir->SetGuidance("HCALModNoDir");
    fSetHCALModNoDir->SetParameterName("X", "Y","Z", false);
    fSetHCALModNoDir->SetDefaultValue(G4ThreeVector(1, 20, 120));
    fSetHCALModNoDir->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetHCALModuleNo = new G4UIcmdWith3Vector("/DP/det/HCAL/moduleNo", this);
    fSetHCALModuleNo->SetGuidance("HCALModuleNo");
    fSetHCALModuleNo->SetParameterName("X", "Y","Z", false);
    fSetHCALModuleNo->SetDefaultValue(G4ThreeVector(3, 3, 1));
    fSetHCALModuleNo->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetHCALModuleGap = new G4UIcmdWithADoubleAndUnit("/DP/det/HCAL/moduleGap", this);
    fSetHCALModuleGap->SetGuidance("HCALModuleGap");
    fSetHCALModuleGap->SetParameterName("HCALModuleGap", false);
    fSetHCALModuleGap->SetDefaultValue(0.5 * mm);
    fSetHCALModuleGap->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetHCALAbsorberThickness = new G4UIcmdWithADoubleAndUnit("/DP/det/HCAL/absorberThickness", this);
    fSetHCALAbsorberThickness->SetGuidance("HCALAbsorberThickness");
    fSetHCALAbsorberThickness->SetParameterName("HCALAbsorberThickness", false);
    fSetHCALAbsorberThickness->SetDefaultValue(3 * cm);
    fSetHCALAbsorberThickness->AvailableForStates(G4State_PreInit, G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger() {
    delete fDirectory;
    delete fDetDirectory;
    delete fTargetDirectory;
    delete fTagTrkDirectory;
    delete fRecTrkDirectory;
    delete fECALDirectory;
    delete fHCALDirecotry;
    delete fSetTBxCmd;
    delete fSetRBxCmd;
    delete fSetifBiasCmd;
    delete fifSaveGeomCmd;
    delete fECAL_SelectionCmd;
    delete fSetifTarget;
    delete fSetifTagTrk;
    delete fSetifRecTrk;
    delete fSetifECAL;
    delete fSetifHCAL;
    delete fSetonlyTracker;
    delete fSetonlyHCAL;
    delete fReConstruct;
    delete fSetTrkTarDis;
    delete fAddNewTagTrkSize;
    delete fAddNewTagTrkPos;
    delete fDelTagTrk;
    delete fSetTagTrk1Rotation;
    delete fSetTagTrk2Rotation;
    delete fSetTagTrk1Color;
    delete fSetTagTrk2Color;
    delete fAddNewRecTrkSize;
    delete fAddNewRecTrkPos;
    delete fDelRecTrk;
    delete fSetRecTrk1Rotation;
    delete fSetRecTrk2Rotation;
    delete fSetRecTrk1Color;
    delete fSetRecTrk2Color;
    delete fSetECALCenterWrapSize;
    delete fSetECALCenterSize;
    delete fSetECALCenterModuleNo;
    delete fSetHCALWrapSize;
    delete fSetHCALSizeDir;
    delete fSetHCALModNoDir;
    delete fSetHCALModuleNo;
    delete fSetHCALModuleGap;
    delete fSetHCALAbsorberThickness;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand *command, G4String newValue) {

    if (command == fSetifBiasCmd) {
        fDetectorConstruction
                ->SetifBias(fSetifBiasCmd->GetNewBoolValue(newValue));
    }

    if (command == fECAL_SelectionCmd) {
        fDetectorConstruction
                ->SetECALSelection(fECAL_SelectionCmd->GetNewIntValue(newValue));
    }

    if (command == fSetifTarget) {
        fDetectorConstruction
                ->SetifTarget(fSetifTarget->GetNewBoolValue(newValue));
    }

    /// Tracker

    if (command == fSetTrkTarDis) {
       fDetectorConstruction->SetTrkTarDis(fSetTrkTarDis->GetNewDoubleValue(newValue));
    }

    if (command == fSetifTagTrk) {
        fDetectorConstruction
                ->SetifTagTrk(fSetifTagTrk->GetNewBoolValue(newValue));
    }

    /// TagTrk

    if (command == fAddNewTagTrkSize) {
        fDetectorConstruction->AddNewTagTrkSize(fAddNewTagTrkSize->GetNew3VectorValue(newValue));
    }

    if (command == fAddNewTagTrkPos) {
        fDetectorConstruction->AddNewTagTrkPos(fAddNewTagTrkPos->GetNew3VectorValue(newValue));
    }

    if (command == fDelTagTrk) {
       fDetectorConstruction->DelTagTrk(fDelTagTrk->GetNewBoolValue(newValue));
    }

    if (command == fSetTagTrk1Rotation) {
       fDetectorConstruction->SetTagTrk1Rotation(fSetTagTrk1Rotation->GetNewDoubleValue(newValue));
    }

    if (command == fSetTagTrk2Rotation) {
       fDetectorConstruction->SetTagTrk2Rotation(fSetTagTrk2Rotation->GetNewDoubleValue(newValue));
    }

    if (command == fSetTagTrk1Color) {
       fDetectorConstruction->SetTagTrk1Color(fSetTagTrk1Color->GetNew3VectorValue(newValue));
    }

    if (command == fSetTagTrk2Color) {
       fDetectorConstruction->SetTagTrk2Color(fSetTagTrk2Color->GetNew3VectorValue(newValue));
    }

    if (command == fSetTBxCmd) {
        fDetectorConstruction
                ->SetTagTrkMagField(fSetTBxCmd->GetNewDoubleValue(newValue));
    }

    /// RecTrk

    if (command == fSetifRecTrk) {
        fDetectorConstruction
                ->SetifRecTrk(fSetifRecTrk->GetNewBoolValue(newValue));
    }

    if (command == fAddNewRecTrkSize) {
       fDetectorConstruction->AddNewRecTrkSize(fAddNewRecTrkSize->GetNew3VectorValue(newValue));
    }

    if (command == fAddNewRecTrkPos) {
       fDetectorConstruction->AddNewRecTrkPos(fAddNewRecTrkPos->GetNew3VectorValue(newValue));
    }

    if (command == fDelRecTrk) {
       fDetectorConstruction->DelRecTrk(fDelRecTrk->GetNewBoolValue(newValue));
    }

    if (command == fSetRecTrk1Rotation) {
       fDetectorConstruction->SetRecTrk1Rotation(fSetRecTrk1Rotation->GetNewDoubleValue(newValue));
    }

    if (command == fSetRecTrk2Rotation) {
       fDetectorConstruction->SetRecTrk2Rotation(fSetRecTrk2Rotation->GetNewDoubleValue(newValue));
    }

    if (command == fSetRecTrk1Color) {
       fDetectorConstruction->SetRecTrk1Color(fSetRecTrk1Color->GetNew3VectorValue(newValue));
    }

    if (command == fSetRecTrk2Color) {
       fDetectorConstruction->SetRecTrk2Color(fSetRecTrk2Color->GetNew3VectorValue(newValue));
    }

    if (command == fSetRBxCmd) {
        fDetectorConstruction
                ->SetRecTrkMagField(fSetRBxCmd->GetNewDoubleValue(newValue));
    }

    /// ECAL

    if (command == fSetifECAL) {
        fDetectorConstruction
                ->SetifECAL(fSetifECAL->GetNewBoolValue(newValue));
    }
    
    if (command == fSetECALCenterWrapSize) {
       fDetectorConstruction->SetECALCenterWrapSize(fSetECALCenterWrapSize->GetNew3VectorValue(newValue));
    }

    if (command == fSetECALCenterSize) {
       fDetectorConstruction->SetECALCenterSize(fSetECALCenterSize->GetNew3VectorValue(newValue));
    }

    if (command == fSetECALCenterModuleNo) {
       fDetectorConstruction->SetECALCenterModuleNo(fSetECALCenterModuleNo->GetNew3VectorValue(newValue));
    }

    /// HCAL

    if (command == fSetifHCAL) {
        fDetectorConstruction
                ->SetifHCAL(fSetifHCAL->GetNewBoolValue(newValue));
    }

    if (command == fSetHCALWrapSize) {
       fDetectorConstruction->SetHCALWrapSize(fSetHCALWrapSize->GetNew3VectorValue(newValue));
    }

    if (command == fSetHCALSizeDir) {
       fDetectorConstruction->SetHCALSizeDir(fSetHCALSizeDir->GetNew3VectorValue(newValue));
    }

    if (command == fSetHCALModNoDir) {
       fDetectorConstruction->SetHCALModNoDir(fSetHCALModNoDir->GetNew3VectorValue(newValue));
    }

    if (command == fSetHCALModuleNo) {
       fDetectorConstruction->SetHCALModuleNo(fSetHCALModuleNo->GetNew3VectorValue(newValue));
    }

    if (command == fSetHCALModuleGap) {
       fDetectorConstruction->SetHCALModuleGap(fSetHCALModuleGap->GetNewDoubleValue(newValue));
    }

    if (command == fSetHCALAbsorberThickness) {
       fDetectorConstruction->SetHCALAbsorberThickness(fSetHCALAbsorberThickness->GetNewDoubleValue(newValue));
    }

    /// Set Only

    if (command == fSetonlyTracker) {
        fDetectorConstruction->SetifTarget(false);
        fDetectorConstruction->SetifTagTrk(true);
        fDetectorConstruction->SetifRecTrk(true);
        fDetectorConstruction->SetifECAL(false);
        fDetectorConstruction->SetifHCAL(false);
    }

    if (command == fSetonlyECAL) {
        fDetectorConstruction->SetifTarget(false);
        fDetectorConstruction->SetifTagTrk(false);
        fDetectorConstruction->SetifRecTrk(false);
        fDetectorConstruction->SetifECAL(true);
        fDetectorConstruction->SetifHCAL(false);
    }

    if (command == fSetonlyHCAL) {
        fDetectorConstruction->SetifTarget(false);
        fDetectorConstruction->SetifTagTrk(false);
        fDetectorConstruction->SetifRecTrk(false);
        fDetectorConstruction->SetifECAL(false);
        fDetectorConstruction->SetifHCAL(true);
    }

    /// ReConstruct

    if (command == fReConstruct) {
        fDetectorConstruction->ReConstruct();
    }

    /// Save

    if (command == fifSaveGeomCmd) {
        if (fifSaveGeomCmd->GetNewBoolValue(newValue))
            fDetectorConstruction->SaveGeometry();
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
