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
/// \file DetectorMessenger.hh
/// \brief Definition of the DetectorMessenger class

#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;

class G4UIdirectory;

class G4UIcmdWithAString;

class G4UIcmdWithAnInteger;

class G4UIcmdWithADouble;

class G4UIcmdWithADoubleAndUnit;

class G4UIcmdWithABool;

class G4UIcmdWithADoubleAndUnit;

class G4UIcmdWithoutParameter;

class G4UIcmdWith3Vector;

class G4UIcmdWith3VectorAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// Messenger class that defines commands for DetectorConstruction.
///
/// It implements commands:
/// - //det/setTargetMaterial name
/// - //det/setChamberMaterial name
/// - //det/setField value unit
/// - //det/stepMax value unit

class DetectorMessenger : public G4UImessenger {
public:
    DetectorMessenger(DetectorConstruction *);

    virtual ~DetectorMessenger();

    void SetNewValue(G4UIcommand *, G4String) override;

private:
    DetectorConstruction *fDetectorConstruction;

    G4UIdirectory *fDirectory;
    G4UIdirectory *fDetDirectory;
    G4UIdirectory *fTargetDirectory;
    G4UIdirectory *fTagTrkDirectory;
    G4UIdirectory *fRecTrkDirectory;
    G4UIdirectory *fECALDirectory;
    G4UIdirectory *fHCALDirecotry;

    G4UIcmdWithABool *fSetifBiasCmd;

    G4UIcmdWithABool *fifSaveGeomCmd;

    G4UIcmdWithAnInteger *fECAL_SelectionCmd;
    G4UIcmdWithABool *fSetifTarget;

    G4UIcmdWithABool *fSetifECAL;
    G4UIcmdWithABool *fSetifHCAL;
    G4UIcmdWithoutParameter *fSetonlyTracker;
    G4UIcmdWithoutParameter *fSetonlyECAL;
    G4UIcmdWithoutParameter *fSetonlyHCAL;
    G4UIcmdWithoutParameter *fReConstruct;

    /// Tracker Setter

    G4UIcmdWithADoubleAndUnit *fSetTrkTarDis;

    /// TagTrk Setter

    G4UIcmdWithABool *fSetifTagTrk;
    G4UIcmdWith3VectorAndUnit *fAddNewTagTrkSize;
    G4UIcmdWith3VectorAndUnit *fAddNewTagTrkPos;
    G4UIcmdWithABool *fDelTagTrk;
    G4UIcmdWithADoubleAndUnit *fSetTagTrk1Rotation;
    G4UIcmdWithADoubleAndUnit *fSetTagTrk2Rotation;
    G4UIcmdWith3Vector *fSetTagTrk1Color;
    G4UIcmdWith3Vector *fSetTagTrk2Color;
    G4UIcmdWithADoubleAndUnit *fSetTBxCmd;

    /// RecTrk Setter

    G4UIcmdWithABool *fSetifRecTrk;
    G4UIcmdWith3VectorAndUnit *fAddNewRecTrkSize;
    G4UIcmdWith3VectorAndUnit *fAddNewRecTrkPos;
    G4UIcmdWithABool *fDelRecTrk;
    G4UIcmdWithADoubleAndUnit *fSetRecTrk1Rotation;
    G4UIcmdWithADoubleAndUnit *fSetRecTrk2Rotation;
    G4UIcmdWith3Vector *fSetRecTrk1Color;
    G4UIcmdWith3Vector *fSetRecTrk2Color;
    G4UIcmdWithADoubleAndUnit *fSetRBxCmd;

    /// ECAL Setter

    G4UIcmdWith3VectorAndUnit *fSetECALCenterWrapSize;
    G4UIcmdWith3VectorAndUnit *fSetECALCenterSize;
    G4UIcmdWith3Vector *fSetECALCenterModuleNo;

    /// HCAL Setter

    G4UIcmdWith3VectorAndUnit *fSetHCALWrapSize;
    G4UIcmdWith3VectorAndUnit *fSetHCALSizeDir;
    G4UIcmdWith3Vector *fSetHCALModNoDir;
    G4UIcmdWith3Vector *fSetHCALModuleNo;
    G4UIcmdWithADoubleAndUnit *fSetHCALModuleGap;
    G4UIcmdWithADoubleAndUnit *fSetHCALAbsorberThickness;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
