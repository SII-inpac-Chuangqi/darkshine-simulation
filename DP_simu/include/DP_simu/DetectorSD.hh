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
/// \file DetectorSD.hh
/// \brief Definition of the DetectorSD class

#ifndef DetectorSD_h
#define DetectorSD_h 1

#include "G4VSensitiveDetector.hh"
#include "RootManager.hh"

#include "Object/SimulatedHit.h"

#include <vector>
#include <iterator>

class G4Step;

// class RootManager;

class G4HCofThisEvent;

/// Calorimeter sensitive detector class
///
/// In Initialize(), it creates one hit for each calorimeter layer and one more
/// hit for accounting the total quantities in all layers.
///
/// The values are accounted in hits in ProcessHits() function which is called
/// by Geant4 kernel at each step.

class DetectorSD : public G4VSensitiveDetector {
public:
    DetectorSD(G4int,
               const G4String &name,
               const G4ThreeVector &);

    ~DetectorSD() override;

    // methods from base class
    void Initialize(G4HCofThisEvent *hitCollection) override;

    G4bool ProcessHits(G4Step *step, G4TouchableHistory *history) override;

    void EndOfEvent(G4HCofThisEvent *hitCollection) override;

private:
    G4ThreeVector fCellID;
    G4int fType; // 0: Tracker 1: ECAL_Center 2: ECAL_Outer
    G4String fname;
    G4int reNumber1; // replical number of PV

    std::vector<SimulatedHit *> fSimHitVec;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

