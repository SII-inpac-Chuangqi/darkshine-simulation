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
#include "Control/Control.h"
#include "Bias_Filter/FilterManager.hh"

#include "G4Step.hh"
#include "G4EventManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalPhoton.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction()
        : G4UserSteppingAction() {
    G4cout << "Stepping Initialized!!" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction() {
    G4cout << "Stepping deleted..." << G4endl;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step *aStep) {
    G4StepPoint *prev = aStep->GetPreStepPoint();
    G4StepPoint *post = aStep->GetPostStepPoint();

    // For default hardbrem filter
    // Requirement: gamma from initial electron with energy larger than 4 GeV in tracker region
    if (aStep->GetTrack()->GetTrackID() == 1 && dControl->if_HardBrem && dControl->if_filter) {
        // If out of selection region, check event status
        if (prev->GetTotalEnergy() < 4 * GeV || prev->GetPosition()[2] >= 180 * mm) {
            if (!dFilterManager->GetHardbremFound()) {
                G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->SetEventAborted();
                G4EventManager::GetEventManager()->AbortCurrentEvent();
            }
        } else if(fabs(prev->GetKineticEnergy() - post->GetKineticEnergy()) >= 4 *GeV) {
            // Search for all secondaries in current step
            for (auto sec : *(aStep->GetSecondaryInCurrentStep())) {
                if (sec->GetParticleDefinition()->GetPDGEncoding() == 22
                    && sec->GetTotalEnergy() >= 4 * GeV){
                    dFilterManager->SetHardbremFound(true);
                }
            }
        }
    }

    if (dControl->if_filter) {
        if (dControl->fStage < dFilterManager->GetCheckIncludeStage()) { // check excluding filters
            if ((dFilterManager->GetifFilter_Process() && !dFilterManager->Filter_Process(aStep)) // Process filters
                || (dFilterManager->GetifFilter_Particle() &&
                    !dFilterManager->Filter_Particle(aStep))) { // Particle filters
                G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->SetEventAborted();
                G4EventManager::GetEventManager()->AbortCurrentEvent();
            }
        } else if (dFilterManager->GetifCheckIncludeResult()
                   && dControl->fStage == dFilterManager->GetCheckIncludeStage()) { // check including filters result
            dFilterManager->SetifCheckIncludeResult(false);
            if ((dFilterManager->GetifFilter_Process() && !dFilterManager->Filter_Process_Found_Result())
                || (dFilterManager->GetifFilter_Particle() && !dFilterManager->Filter_Particle_Found_Result())) {
                G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->SetEventAborted();
                G4EventManager::GetEventManager()->AbortCurrentEvent();
            }
        }
    }

    // Get Detector Region
    if (post && post->GetPhysicalVolume()) {
        auto Region_name = post->GetPhysicalVolume()->GetName();
        dRootMng->FillEleak(aStep, Region_name);
    }
    if (!post) return;
    if (dControl->save_initial_particle_step
        && aStep->GetTrack()->GetTrackID() == 1) {

        /* Record all steps for certain particle */
        dRootMng->FillParticleStep(aStep);
    }
    if (aStep->GetTrack()->GetTrackID() == 1) {
        if (post->GetProcessDefinedStep()->GetProcessName().contains("electronNuclear") ) {
//            || post->GetProcessDefinedStep()->GetProcessName() == "biasWrapper(electronNuclear)" ) {
            G4double deltaE = fabs(prev->GetKineticEnergy() - post->GetKineticEnergy());
            if (post->GetPosition()[2] <= 100. * mm) {
                // Target
                ENEnergyTar = deltaE;
            } else {
                // ECal
                ENEnergyECAL = deltaE;
            }
            dRootMng->FillENE(ENEnergyTar, ENEnergyECAL, post->GetPosition()[2]);
        }
    } else {
        if (aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding() == 22) {
            // Photo-Nuclear Reaction
            if (post->GetProcessDefinedStep()->GetProcessType() == 4 // 4=Nulear
                && post->GetProcessDefinedStep()->GetProcessSubType() == 121) { // 121=HadInElastic
                G4double deltaE = fabs(prev->GetKineticEnergy() - post->GetKineticEnergy());
                if (post->GetPosition()[2] <= 100. * mm) {
                    // Target
                    PNEnergyTar = deltaE;
                } else {
                    // ECal
                    PNEnergyECAL = deltaE;
                }

                dRootMng->FillPNE(PNEnergyTar, PNEnergyECAL, post->GetPosition()[2]);
            }
        }
    }

    // /* Optical Photon Detection: APD region */
    //no need for real photon, since LUT is default
    // if (dControl->if_optical && aStep->GetTrack()->GetParticleDefinition()->GetParticleName() == "opticalphoton") {
    //     if (post->GetPhysicalVolume()) {
    //         auto Region_name = post->GetPhysicalVolume()->GetName();
    //         auto kill_flag = froot->FillOptical(aStep, Region_name);

    //         if (kill_flag) aStep->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
    //     }

    // }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


