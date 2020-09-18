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

#include "G4Track.hh"
#include "G4TrackingManager.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "DP_simu/MCParticle.hh"
#include "DP_simu/RootManager.hh"
#include "G4SystemOfUnits.hh"

class MCParticle;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction(RootManager* rootMng)
 : G4UserTrackingAction()
{
    froot = rootMng;
    fMC = new MCParticle(); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::~TrackingAction()
{
    delete fMC;
    //delete froot;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{ 
    /* Initialize Filter */
    //froot->Filter_Initialize();

    fMC->SetPDG( aTrack->GetParticleDefinition()->GetPDGEncoding());
    fMC->Setid( aTrack->GetTrackID() );
    fMC->SetParentID( aTrack->GetParentID() );
    fMC->SetEnergy( aTrack->GetTotalEnergy() );
    fMC->SetMomentum( aTrack->GetMomentum() );
    fMC->SetVPos( aTrack->GetVertexPosition() );
    if ( aTrack->GetCreatorProcess() ) {
        fMC->SetProcess( aTrack->GetCreatorProcess()->GetProcessName() );
        fMC->SetProcessType( aTrack->GetCreatorProcess()->GetProcessType() );
        fMC->SetProcessSubType( aTrack->GetCreatorProcess()->GetProcessSubType() );
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
    fMC->SetEPos( aTrack->GetStep()->GetPreStepPoint()->GetPosition() );

    G4double Eremain = aTrack->GetKineticEnergy();

    G4double pm = sqrt(fMC->GetMomentum()[0]*fMC->GetMomentum()[0]+
                       fMC->GetMomentum()[1]*fMC->GetMomentum()[1]+
                       fMC->GetMomentum()[2]*fMC->GetMomentum()[2]);

    if (  fMC->Getid() == 1 
       || pm >= 1.*GeV
       || (fMC->GetEnergy() >= 1.*GeV && fMC->GetEnergy() <= 8.*GeV)
       || abs(fMC->GetPDG()) == 13   // Muon
       || abs(fMC->GetPDG()) == 111  // Pion0
       || abs(fMC->GetPDG()) == 211  // Pion+-
       || abs(fMC->GetPDG()) == 321  // Kaon+-
       || abs(fMC->GetPDG()) == 2212 // proton
       || abs(fMC->GetPDG()) == 2112 // neutron
       || abs(fMC->GetPDG()) == 14   // muon neutrino
       || abs(fMC->GetPDG()) == 12   // electron neutrino
       ) froot->FillMC( fMC , Eremain );

    fMC->Initialize();
    if(aTrack->GetTrackID() == 1) {
        //G4cout<< " " << G4endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
