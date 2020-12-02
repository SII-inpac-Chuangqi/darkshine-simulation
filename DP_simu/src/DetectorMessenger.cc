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

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger() {
    delete fDirectory;
    delete fDetDirectory;
    delete fSetTBxCmd;
    delete fSetRBxCmd;
    delete fSetifBiasCmd;
    delete fifSaveGeomCmd;
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

    if (command == fifSaveGeomCmd) {
        if (fifSaveGeomCmd->GetNewBoolValue(newValue))
            fDetectorConstruction->SaveGeometry();
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
