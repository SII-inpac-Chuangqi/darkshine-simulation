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
#include "Animation/AnimationData.h"

#include "G4Version.hh"
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
        record_step_z = dControl->Pos_HCALRegion.z() + 0.5 * dControl->Size_HCALRegion.z();
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

    if (dControl->if_filter) {
        if (prev->GetPhysicalVolume()->GetName()[0] == 'E') {
            if (dControl->veto_missP && dControl->fStage == 0
                && prev->GetPosition().z() <= dControl->ECAL_Front_Z
                && (dRootMng->GetEvt()->getStepCollection().at(dControl->InitialParticleStepCollection_Name)->at(0)->getE() - prev->GetTotalEnergy()) < dControl->veto_missP_leq_E) {
                dRootMng->AddFEvtNbKilledByTruthFilter();
                G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->SetEventAborted();
                G4EventManager::GetEventManager()->AbortCurrentEvent();
            }
            dFilterManager->AddEstimateInECALEnergy(aStep->GetTotalEnergyDeposit());
            if (dControl->veto_ECAL && dFilterManager->GetEstimateInECALEnergy() > dControl->veto_ECAL_geq_E) {
                dRootMng->AddFEvtNbKilledByTruthFilter();
                G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->SetEventAborted();
                G4EventManager::GetEventManager()->AbortCurrentEvent();
            }
        }
        // For default hardbrem filter
        // Requirement: gamma from initial electron with energy larger than 4 GeV in tracker region
        // N.B. initial particle starts from 1 and parent==0
        if (aStep->GetTrack()->GetParentID()==0 && dControl->if_HardBrem) {
            // If out of selection region, check event status
            if (prev->GetTotalEnergy() < dControl->BiasEmin || prev->GetPosition()[2] >= 180 * mm) {
                if (!dFilterManager->GetHardbremFound()) {
                    dRootMng->AddFEvtNbKilledByFilter();
                    G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->SetEventAborted();
                    G4EventManager::GetEventManager()->AbortCurrentEvent();
                }
            } else if (fabs(prev->GetKineticEnergy() - post->GetKineticEnergy()) >= dControl->BiasEmin) {
                // Search for all secondaries in current step
                for (auto sec: *(aStep->GetSecondaryInCurrentStep())) {
                    if (sec->GetParticleDefinition()->GetPDGEncoding() == 22
                        && sec->GetTotalEnergy() >= dControl->BiasEmin) {
                        dFilterManager->SetHardbremFound(true);
                    }
                }
            }
        }

        if (dControl->fStage < dFilterManager->GetCheckIncludeStage()) { // check excluding filters
            if ((dFilterManager->GetifFilter_Process() && !dFilterManager->Filter_Process(aStep)) // Process filters
                || (dFilterManager->GetifFilter_Particle() &&
                    !dFilterManager->Filter_Particle(aStep))) { // Particle filters
                dRootMng->AddFEvtNbKilledByFilter();
                G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->SetEventAborted();
                G4EventManager::GetEventManager()->AbortCurrentEvent();
            }
        } else if (dFilterManager->GetifCheckIncludeResult()
                   && dControl->fStage == dFilterManager->GetCheckIncludeStage()) { // check including filters result
            dFilterManager->SetifCheckIncludeResult(false);
            if ((dFilterManager->GetifFilter_Process() && !dFilterManager->Filter_Process_Found_Result())
                || (dFilterManager->GetifFilter_Particle() && !dFilterManager->Filter_Particle_Found_Result())) {
                dRootMng->AddFEvtNbKilledByFilter();
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
        && aStep->GetTrack()->GetParentID()==0 
        && aStep->GetPreStepPoint()->GetPosition().z() <= record_step_z) {
        /* Record all steps for certain particle */
        dRootMng->FillParticleStep(aStep);
    }
    if (aStep->GetTrack()->GetParentID()==0) { // initial particle
        if (
# if G4VERSION_NUMBER >= 1100
             G4StrUtil::contains(post->GetProcessDefinedStep()->GetProcessName(), "electronNuclear")
# else
             post->GetProcessDefinedStep()->GetProcessName().contains("electronNuclear")
# endif
           )
        {
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


    // For new truth classification
    UpdateTruthStatesInCalo(aStep);
    if (auto p = dTMgr->getTruthParticle(); p->E_kin >= dControl->E_kin_min_step)
        UpdateTruthInfo(p, aStep);


    // Animation
    if (pAniData->if_first_step(aStep->GetTrack()->GetTrackID())) {
        pAniData->add_particle_step(
                aStep->GetTrack()->GetTrackID(),
                prev->GetPosition()[0],
                prev->GetPosition()[1],
                prev->GetPosition()[2]
        );
    }

    pAniData->add_particle_step(
            aStep->GetTrack()->GetTrackID(),
            post->GetPosition()[0],
            post->GetPosition()[1],
            post->GetPosition()[2]
    );

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

    fMCPH->setPx((float) post->GetMomentum().x());
    fMCPH->setPy((float) post->GetMomentum().y());
    fMCPH->setPz((float) post->GetMomentum().z());

    fMCPH->setMass((float) aStep->GetTrack()->GetParticleDefinition()->GetPDGMass());
    fMCPH->setIsIncoming(true);
}

void SteppingAction::UpdateTruthInfo(DTruthParticle *tp, const G4Step *aStep) {
    if (post && post->GetPhysicalVolume()) {
        auto Region_name = post->GetPhysicalVolume()->GetName();

        // Only Energy deposit in the detector
        auto e_deposit = static_cast<float>(aStep->GetTotalEnergyDeposit());
        // Including generating secondaries
        auto e_change = static_cast<float>(prev->GetKineticEnergy() - post->GetKineticEnergy());

        // reject too small energy deposit
        if (e_change < 1e-10) return;

        auto E_sec = e_change - e_deposit;

        tp->E_secondaries += E_sec;

        if (dTMgr->checkInterest(Region_name))
            tp->E_leak += e_deposit;

        auto region_idx = dTMgr->getDetPVIndex(Region_name);
        auto process_idx = dTMgr->getProcessIndex(post->GetProcessDefinedStep()->GetProcessName());

        if (auto change = tp->E_deposit_details.find({region_idx, process_idx}); change !=
                                                                                 tp->E_deposit_details.end()) {
            change->second += e_change;
        } else
            tp->E_deposit_details.insert({{region_idx, process_idx}, e_change});


        // Analyze the truth process
        if (auto n = aStep->GetNumberOfSecondariesInCurrentStep(); n > 0) {

            // Record this process if the production energy is larger than ratio * E_kin_current
            if (E_sec >= dControl->E_min_process || E_sec >= dControl->E_process_ratio * prev->GetKineticEnergy()) {

                auto process = new DTruthProcess();
                tp->sec_process_link.push_back(process);

                process->vertex[0] = static_cast<float>(post->GetPosition().x());
                process->vertex[1] = static_cast<float>(post->GetPosition().y());
                process->vertex[2] = static_cast<float>(post->GetPosition().z());

                process->E = E_sec;
                process->index = process_idx;

                process->in_p = tp;
            }
        }
    }
}

void SteppingAction::UpdateTruthStatesInCalo(const G4Step *aStep) {
    if (post && post->GetPhysicalVolume()) {
        auto Region_name = post->GetPhysicalVolume()->GetName();

        // Analyze the tracker region
        auto assignV3 = [](DTruthState *dState, G4StepPoint *step) {
            dState->vertex[0] = static_cast<float>(step->GetPosition().x() / mm);
            dState->vertex[1] = static_cast<float>(step->GetPosition().y() / mm);
            dState->vertex[2] = static_cast<float>(step->GetPosition().z() / mm);

            dState->momentum[0] = static_cast<float>(step->GetMomentum().x() / MeV);
            dState->momentum[1] = static_cast<float>(step->GetMomentum().y() / MeV);
            dState->momentum[2] = static_cast<float>(step->GetMomentum().z() / MeV);

            dState->E = static_cast<float>(step->GetKineticEnergy() / MeV);
        };
        if (prev->GetPhysicalVolume() && dTMgr->getTruthState()) {
            auto Region_name_prev = prev->GetPhysicalVolume()->GetName();

            bool entry_state = Region_name_prev == "World" && Region_name == "TAGTrk";
            bool end_state = Region_name_prev == "RECTrk" && Region_name == "World";
            bool middle_state = (Region_name == "TAGTrk" || Region_name == "RECTrk") &&
                                !(entry_state || end_state);

            if (entry_state || middle_state || end_state ||
                (Region_name == "Target_PV" && !dTMgr->getTruthState()->empty())) {
                // Add a start status
                if (dTMgr->getTruthState()->empty()) {
                    auto dState = new DTruthState();
                    assignV3(dState, prev);
                    dTMgr->getTruthState()->push_back(dState);
                }
                // Calculate the process index
                auto dState = new DTruthState();
                assignV3(dState, post);
                dState->process_index = dTMgr->getProcessIndex(post->GetProcessDefinedStep()->GetProcessName());

                dTMgr->getTruthState()->push_back(dState);
            }
        }
        // Analyze the Calorimeter region
        {
            auto Region_name_prev = prev->GetPhysicalVolume()->GetName();
            bool ecal_state = Region_name_prev == "World" && Region_name == "ECAL";
            bool hcal_state = Region_name_prev != "HCAL" && Region_name == "HCAL";

            if ((ecal_state || hcal_state) && post->GetKineticEnergy() > 0.01 * MeV) {
                auto dState_prev = new DTruthState();
                assignV3(dState_prev, prev);
                auto dState_post = new DTruthState();
                assignV3(dState_post, post);
                dRootMng->GetEvt()->getTruthInfo()->UpdateTruthDetTrack(
                        {aStep->GetTrack()->GetTrackID(), aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding()},
                        {dState_prev, dState_post});
            }
        }
    }
}
