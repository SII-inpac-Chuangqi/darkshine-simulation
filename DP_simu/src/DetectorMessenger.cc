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

    // ------------- Detector Setting ---------------
    fECAL_SelectionCmd = new G4UIcmdWithAnInteger("/DP/det/selectECAL",this);
    fECAL_SelectionCmd->SetGuidance("Select the build-in ECAL Configuration.");
    fECAL_SelectionCmd->SetParameterName("ECAL_Selection",false);
    fECAL_SelectionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

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

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger() {
    delete fDirectory;
    delete fDetDirectory;
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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand *command, G4String newValue) {
    if (command == fSetTBxCmd) {
        fDetectorConstruction
                ->SetTagTrkMagField(fSetTBxCmd->GetNewDoubleValue(newValue));
    }

    if (command == fSetRBxCmd) {
        fDetectorConstruction
                ->SetRecTrkMagField(fSetRBxCmd->GetNewDoubleValue(newValue));
    }

    if (command == fSetifBiasCmd) {
        fDetectorConstruction
                ->SetifBias(fSetifBiasCmd->GetNewBoolValue(newValue));
    }

    if (command == fECAL_SelectionCmd) {
        fDetectorConstruction
                ->SetECALSelection(fECAL_SelectionCmd->GetNewIntValue(newValue));
        fDetectorConstruction->RebuildGeometry();
    }

    if (command == fSetifTarget) {
        fDetectorConstruction
                ->SetifTarget(fSetifTarget->GetNewBoolValue(newValue));
        fDetectorConstruction->RebuildGeometry();
    }

    if (command == fSetifTagTrk) {
        fDetectorConstruction
                ->SetifTagTrk(fSetifTagTrk->GetNewBoolValue(newValue));
        fDetectorConstruction->RebuildGeometry();
    }

    if (command == fSetifRecTrk) {
        fDetectorConstruction
                ->SetifRecTrk(fSetifRecTrk->GetNewBoolValue(newValue));
        fDetectorConstruction->RebuildGeometry();
    }

    if (command == fSetifECAL) {
        fDetectorConstruction
                ->SetifECAL(fSetifECAL->GetNewBoolValue(newValue));
        fDetectorConstruction->RebuildGeometry();
    }

    if (command == fSetifHCAL) {
        fDetectorConstruction
                ->SetifHCAL(fSetifHCAL->GetNewBoolValue(newValue));
        fDetectorConstruction->RebuildGeometry();
    }

    if (command == fSetonlyTracker) {
        fDetectorConstruction->SetifTarget(false);
        fDetectorConstruction->SetifTagTrk(true);
        fDetectorConstruction->SetifRecTrk(true);
        fDetectorConstruction->SetifECAL(false);
        fDetectorConstruction->SetifHCAL(false);
        fDetectorConstruction->RebuildGeometry();
    }

    if (command == fSetonlyECAL) {
        fDetectorConstruction->SetifTarget(false);
        fDetectorConstruction->SetifTagTrk(false);
        fDetectorConstruction->SetifRecTrk(false);
        fDetectorConstruction->SetifECAL(true);
        fDetectorConstruction->SetifHCAL(false);
        fDetectorConstruction->RebuildGeometry();
    }

    if (command == fSetonlyHCAL) {
        fDetectorConstruction->SetifTarget(false);
        fDetectorConstruction->SetifTagTrk(false);
        fDetectorConstruction->SetifRecTrk(false);
        fDetectorConstruction->SetifECAL(false);
        fDetectorConstruction->SetifHCAL(true);
        fDetectorConstruction->RebuildGeometry();
    }

    if (command == fifSaveGeomCmd) {
        if (fifSaveGeomCmd->GetNewBoolValue(newValue))
            fDetectorConstruction->SaveGeometry();
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
