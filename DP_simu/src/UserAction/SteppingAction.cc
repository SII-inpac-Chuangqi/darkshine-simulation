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
    if (dControl->build_ECAL)
        record_step_z = dControl->Pos_ECALRegion.z() - 0.5 * dControl->Size_ECALRegion.z();
    else if (dControl->build_HCAL)
        record_step_z = dControl->Pos_HCALRegion.z() - 0.5 * dControl->Size_HCALRegion.z();
    else if (dControl->build_rec_tracker)
        record_step_z = dControl->rec_Pos_TrackerRegion.z() + 0.5 * dControl->rec_Size_TrackerRegion.z();
    else
        record_step_z = 0;

    G4cout << "Stepping Initialized!!" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction() {
    G4cout << "Stepping deleted..." << G4endl;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step *aStep) {
    prev = aStep->GetPreStepPoint();
    post = aStep->GetPostStepPoint();

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
        && aStep->GetTrack()->GetTrackID() == 1
        && aStep->GetPreStepPoint()->GetPosition().z() <= record_step_z ) {
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

    if (dControl->save_mcp_helper) {
        if (prev->GetPhysicalVolume()->GetName() == "World") {
            if (post->GetPhysicalVolume() && post->GetPhysicalVolume()->GetName() == "ECAL") {
                SetMcPHelper(aStep, nECAL);
                dRootMng->FillMCPHelper(fMCPH, aStep->GetTrack()->GetTrackID());
            } else if (post->GetPhysicalVolume() && post->GetPhysicalVolume()->GetName() == "HCAL") {
                SetMcPHelper(aStep, nHCAL);
                dRootMng->FillMCPHelper(fMCPH, aStep->GetTrack()->GetTrackID());
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

void SteppingAction::SetMcPHelper(const G4Step *aStep, int detector) {
    fMCPH = new McPHelper();
//    auto *touchable = dynamic_cast<const G4TouchableHistory*> (aStep->GetPostStepPoint()->GetTouchable());
//    reNumber1 = touchable->GetReplicaNumber(1);

    fMCPH->setId(aStep->GetTrack()->GetTrackID());
    fMCPH->setDetector(detector);
//    fMCPH->setCellId(reNumber1 + 1);
//    if (detector == nTracker) {
//        fMCPH->setCellIdX(touchable->GetReplicaNumber(0) + 1);
//        fMCPH->setCellIdY(1);
//        fMCPH->setCellIdZ(reNumber1 + 1);
//    } else if (detector == nECAL) {
//        fMCPH->setCellIdX(reNumber1 % ((int)dControl->ECAL_Center_Module_No.x() * (int)dControl->ECAL_Center_Module_No.y()) % (int)dControl->ECAL_Center_Module_No.x() + 1);
//        fMCPH->setCellIdY(((reNumber1 % ((int)dControl->ECAL_Center_Module_No.x() * (int)dControl->ECAL_Center_Module_No.y())) / (int) dControl->ECAL_Center_Module_No.y()) + 1);
//        fMCPH->setCellIdZ(reNumber1 / ((int)dControl->ECAL_Center_Module_No.x() * (int)dControl->ECAL_Center_Module_No.y()) + 1);
//    }

    fMCPH->setX(post->GetPosition().x());
    fMCPH->setY(post->GetPosition().y());
    fMCPH->setZ(post->GetPosition().z());

    fMCPH->setT(post->GetKineticEnergy());
    fMCPH->setE(post->GetTotalEnergy());

    fMCPH->setPdg(aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding());

    fMCPH->setPx((float)post->GetMomentum().x());
    fMCPH->setPy((float)post->GetMomentum().y());
    fMCPH->setPz((float)post->GetMomentum().z());

    fMCPH->setMass((float)aStep->GetTrack()->GetParticleDefinition()->GetPDGMass());
    fMCPH->setIsIncoming(true);
}
