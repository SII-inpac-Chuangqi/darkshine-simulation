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

#include "SteppingAction.hh"
#include "RootManager.hh"
#include "MCParticle.hh"

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
 : G4UserSteppingAction(),
   if_select(false), if_kill(true), if_biased(false)
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
    MCParticle* p = new MCParticle();

    // Get Detector Region
    if( post && post->GetPhysicalVolume() ) {
        auto Region_name = post->GetPhysicalVolume()->GetName();
        froot->FillEleak( aStep , Region_name );
    }

    if ( aStep->GetTrack()->GetTrackID()==1 ) {

        p->SetPDG( aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding() );
        p->Setid( aStep->GetTrack()->GetTrackID());
        p->SetEnergy( post->GetTotalEnergy() );
        p->SetMomentum( post->GetMomentum() );
        p->SetVPos( post->GetPosition() );

        ///*
        if ( fabs(p->GetVPos()[2] - 0.175*mm) <= 0.05*mm ) {
            p1 = new MCParticle();
            p1->SetMomentum( post->GetMomentum() );
            p1->SetVPos( post->GetPosition() );

            froot->FillE1( p1 );
            p1->Initialize();
        }

        if ( fabs(p->GetVPos()[2] - 181.275*mm) <= 0.05*mm ) {
                p2 = p;
                froot->FillE2( p2 );
                p2->Initialize();
        }


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
                EMax1 = deltaE; 
            // ECal
            if( post->GetPosition()[2]  > 180.*mm ) 
                EMax2 = deltaE; 

            froot->FillPNE(EMax1, EMax2);
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


