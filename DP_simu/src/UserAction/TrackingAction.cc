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
/// \file TrackingAction.cc
/// \brief Implementation of the TrackingAction class

#include "DP_simu/TrackingAction.hh"
#include "Control.h"

#include "G4Track.hh"
#include "G4TrackingManager.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "DP_simu/RootManager.hh"
#include "G4SystemOfUnits.hh"

class MCParticle;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction(RootManager *rootMng) : G4UserTrackingAction() {
    froot = rootMng;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::~TrackingAction() = default;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PreUserTrackingAction(const G4Track *aTrack) {
    /* Initialize Filter */

    auto pdg = aTrack->GetParticleDefinition()->GetPDGEncoding();
    auto energy = aTrack->GetKineticEnergy();

    G4double pm = sqrt(aTrack->GetMomentum()[0] * aTrack->GetMomentum()[0] +
                       aTrack->GetMomentum()[1] * aTrack->GetMomentum()[1] +
                       aTrack->GetMomentum()[2] * aTrack->GetMomentum()[2]);

    if (dControl->save_all_mcp || (aTrack->GetTrackID() == 1
                                   || pm >= 1. * GeV
                                   || (energy >= 1. * GeV && energy <= 8. * GeV)
                                   || abs(pdg) == 13   // Muon
                                   || abs(pdg) == 111  // Pion0
                                   || abs(pdg) == 211  // Pion+-
                                   || abs(pdg) == 321  // Kaon+-
                                   || abs(pdg) == 2212 // proton
                                   || abs(pdg) == 2112 // neutron
                                   || abs(pdg) == 14   // muon neutrino
                                   || abs(pdg) == 12   // electron neutrino
    )) {
        fMC = new McParticle();
        fMC->setPdg(pdg);
        fMC->setId(aTrack->GetTrackID());
        fMC->setEnergy(energy);
        fMC->setPx(aTrack->GetMomentum()[0]);
        fMC->setPy(aTrack->GetMomentum()[1]);
        fMC->setPz(aTrack->GetMomentum()[2]);
        fMC->setVertexX(aTrack->GetPosition()[0]);
        fMC->setVertexY(aTrack->GetPosition()[1]);
        fMC->setVertexZ(aTrack->GetPosition()[2]);

        if (aTrack->GetCreatorProcess())
            fMC->setCreateProcess(aTrack->GetCreatorProcess()->GetProcessName());

        froot->FillMC(fMC, aTrack->GetParentID());

    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PostUserTrackingAction(const G4Track *aTrack) {

    // Find MC in collection
    auto MCCols = froot->GetEvt()->getMcParticleCollection_Old().at("RawMCParticle");
    auto p = McParticle::SearchID(MCCols, aTrack->GetTrackID());
    if (p) {
        p->setERemain(aTrack->GetKineticEnergy());
        p->setEndPointX(aTrack->GetStep()->GetPreStepPoint()->GetPosition()[0]);
        p->setEndPointY(aTrack->GetStep()->GetPreStepPoint()->GetPosition()[1]);
        p->setEndPointZ(aTrack->GetStep()->GetPreStepPoint()->GetPosition()[2]);
    }

    delete fMC;
    fMC = nullptr;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
