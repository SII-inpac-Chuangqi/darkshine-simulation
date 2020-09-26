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
/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class

#include "DP_simu/SteppingAction.hh"
#include "DP_simu/RootManager.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4StackManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalPhoton.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(RootManager * rootMng)
 : G4UserSteppingAction()
{
    froot = rootMng;
    G4cout<<"Stepping Initialized!!"<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{
    G4cout<<"Stepping deleted..."<<G4endl;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{ 
    if ( froot->GetifFilter_Process() ) froot->Filter_Process(aStep);
    if ( (froot->GetifFilter_HardBrem() && !froot->Filter_HardBrem(aStep)) ) { 
        G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->SetEventAborted();
        G4EventManager::GetEventManager()->AbortCurrentEvent();
    }

    G4StepPoint* prev = aStep->GetPreStepPoint();
    G4StepPoint* post = aStep->GetPostStepPoint();
    auto * p = new McParticle();

    // Get Detector Region
    if( post && post->GetPhysicalVolume() ) {
        auto Region_name = post->GetPhysicalVolume()->GetName();
        froot->FillEleak( aStep , Region_name );
    }

    if ( aStep->GetTrack()->GetTrackID()==1 ) {

        p->setPdg( aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding() );
        p->setId( aStep->GetTrack()->GetTrackID());
        p->setEnergy( post->GetTotalEnergy() );
        p->setPx(post->GetMomentum()[0]);
        p->setPy(post->GetMomentum()[1]);
        p->setPz(post->GetMomentum()[2]);
        p->setVertexX( post->GetPosition()[0] );
        p->setVertexY( post->GetPosition()[1] );
        p->setVertexZ( post->GetPosition()[2] );

        /* Record all steps for certain particle */
        if ( froot->GetRecordStep() )
            froot->FillParticleStep(aStep);
    }

    if ( aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding() == 22 ) {
            
        G4double deltaE = fabs( prev->GetKineticEnergy() - post->GetKineticEnergy() );
        
        // Photo-Nuclear Reaction
        if (   post->GetProcessDefinedStep()->GetProcessType() == 4
            && post->GetProcessDefinedStep()->GetProcessSubType() == 121) {
            
            // Target
            if( post->GetPosition()[2] <= 180.*mm )
                PNEnergyTar = deltaE;
            // ECal
            if( post->GetPosition()[2]  > 180.*mm )
                PNEnergyECAL = deltaE;

            froot->FillPNE(PNEnergyTar, PNEnergyECAL);
        }
    }

    /* Optical Photon Detection: APD region */
    if ( froot->GetOptical() && aStep->GetTrack()->GetParticleDefinition()->GetParticleName() == "opticalphoton" ) {
        if( post && post->GetPhysicalVolume() ) {
            auto Region_name = post->GetPhysicalVolume()->GetName();
            auto kill_flag = froot->FillOptical( aStep , Region_name );

            if (kill_flag) aStep->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries );
        }

    }

    delete p;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


