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
#include "DP_simu/RootManager.hh"
#include "G4SystemOfUnits.hh"

class MCParticle;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction(RootManager* rootMng)
 : G4UserTrackingAction() {
    froot = rootMng;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::~TrackingAction()
{
    //delete fMC;
    //delete froot;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{ 
    /* Initialize Filter */
    //froot->Filter_Initialize();
    fMC = new McParticle();
    fMC->setPdg( aTrack->GetParticleDefinition()->GetPDGEncoding());
    fMC->setId( aTrack->GetTrackID() );
    fMC->setEnergy( aTrack->GetTotalEnergy() );
    fMC->setPx(aTrack->GetMomentum()[0]);
    fMC->setPy(aTrack->GetMomentum()[1]);
    fMC->setPz(aTrack->GetMomentum()[2]);
    fMC->setVertexX( aTrack->GetPosition()[0] );
    fMC->setVertexY( aTrack->GetPosition()[1] );
    fMC->setVertexZ( aTrack->GetPosition()[2] );

    if ( aTrack->GetCreatorProcess() )
        fMC->setCreateProcess( aTrack->GetCreatorProcess()->GetProcessName() );

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
    fMC->setEndPointX( aTrack->GetStep()->GetPreStepPoint()->GetPosition()[0] );
    fMC->setEndPointY( aTrack->GetStep()->GetPreStepPoint()->GetPosition()[1] );
    fMC->setEndPointZ( aTrack->GetStep()->GetPreStepPoint()->GetPosition()[2] );

    fMC->setERemain( aTrack->GetKineticEnergy());

    G4double pm = sqrt(fMC->getPx()*fMC->getPx()+
                       fMC->getPy()*fMC->getPy()+
                       fMC->getPz()*fMC->getPz());

    auto MC = new McParticle(*fMC);

    if (  fMC->getId() == 1
       || pm >= 1.*GeV
       || (fMC->getEnergy() >= 1.*GeV && fMC->getEnergy() <= 8.*GeV)
       || abs(fMC->getPdg()) == 13   // Muon
       || abs(fMC->getPdg()) == 111  // Pion0
       || abs(fMC->getPdg()) == 211  // Pion+-
       || abs(fMC->getPdg()) == 321  // Kaon+-
       || abs(fMC->getPdg()) == 2212 // proton
       || abs(fMC->getPdg()) == 2112 // neutron
       || abs(fMC->getPdg()) == 14   // muon neutrino
       || abs(fMC->getPdg()) == 12   // electron neutrino
       ) froot->FillMC( MC, aTrack->GetParentID());

    delete fMC;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
