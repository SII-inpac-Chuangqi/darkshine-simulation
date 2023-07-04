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
#include "Control/Control.h"
#include "Animation/AnimationData.h"

#include "G4Track.hh"
#include "G4TrackingManager.hh"
#include "G4Event.hh"
#include "DP_simu/RootManager.hh"
#include "Utility/TruthManager.h"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction() : G4UserTrackingAction() {
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::~TrackingAction() = default;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PreUserTrackingAction(const G4Track *aTrack) {
    /* Initialize Filter */

    auto pdg = aTrack->GetParticleDefinition()->GetPDGEncoding();
    auto energy = aTrack->GetTotalEnergy();
    auto kin_energy = aTrack->GetKineticEnergy();


    G4double pm = sqrt(aTrack->GetMomentum()[0] * aTrack->GetMomentum()[0] +
                       aTrack->GetMomentum()[1] * aTrack->GetMomentum()[1] +
                       aTrack->GetMomentum()[2] * aTrack->GetMomentum()[2]);


    // For new truth classification
    dTMgr->setTruthParticle(new DTruthParticle());
    auto truth_particle = dTMgr->getTruthParticle();
    truth_particle->id = aTrack->GetTrackID();
    truth_particle->pdg = pdg;
    truth_particle->vertex[0] = static_cast<float>(aTrack->GetPosition().x());
    truth_particle->vertex[1] = static_cast<float>(aTrack->GetPosition().y());
    truth_particle->vertex[2] = static_cast<float>(aTrack->GetPosition().z());
    truth_particle->E_kin = static_cast<float>(kin_energy);
    truth_particle->E_remain = 0.;

    if (aTrack->GetCreatorProcess())
        truth_particle->prod_process = dTMgr->getProcessIndex(aTrack->GetCreatorProcess()->GetProcessName());
    else
        truth_particle->prod_process = 0;

    auto dTruth = dRootMng->GetEvt()->getTruthInfo();
    dTruth->linkParentProcess(truth_particle, aTrack->GetParentID());

    if (aTrack->GetPosition().z() <= 180 * mm && aTrack->GetParticleDefinition()->GetPDGCharge() != 0) {
        auto new_truth_track = dTruth->InitTruthTrack({aTrack->GetTrackID(), pdg});
        dTMgr->setTruthState(new_truth_track);
    }

    // For McParticle Collection
    if (dControl->save_MC) {
        if (dControl->save_all_mcp || (aTrack->GetParentID()==0
                                       || pm >= 1. * GeV
                                       || (kin_energy >= 1. * GeV && kin_energy <= 10. * GeV)
                                       || abs(pdg) == 13   // Muon
                                       || abs(pdg) == 111  // Pion0
                                       || abs(pdg) == 211  // Pion+-
                                       || abs(pdg) == 321  // Kaon+-
                                       || abs(pdg) == 2212 // proton
                                       || abs(pdg) == 2112 // neutron
                                       || abs(pdg) == 14   // muon neutrino
                                       || abs(pdg) == 12   // electron neutrino
                                       || (abs(pdg) >= 100 && abs(pdg) <= 10000) // inclusive hadrons

        )) {
            fMC = new McParticle();
            fMC->setPdg(pdg);
            fMC->setId(aTrack->GetTrackID());
            fMC->setMass(aTrack->GetParticleDefinition()->GetPDGMass());
            fMC->setEnergy(energy);
            fMC->setPx(aTrack->GetMomentum().x());
            fMC->setPy(aTrack->GetMomentum().y());
            fMC->setPz(aTrack->GetMomentum().z());
            fMC->setVertexX(aTrack->GetPosition().x());
            fMC->setVertexY(aTrack->GetPosition().y());
            fMC->setVertexZ(aTrack->GetPosition().z());

            if (aTrack->GetCreatorProcess())
                fMC->setCreateProcess(aTrack->GetCreatorProcess()->GetProcessName());

            auto mc = dRootMng->FillMC(fMC, aTrack->GetParentID());

            truth_particle->mc_link = mc;
        }
    }


    // Animation
    pAniData->add_particle(
            kin_energy,
            aTrack->GetTrackID(),
            pdg,
            aTrack->GetParentID(),
            aTrack->GetGlobalTime()
    );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PostUserTrackingAction(const G4Track *aTrack) {
    // save weight
    if (aTrack->GetWeight() != 1) {
        dRootMng->FillWeight(aTrack->GetWeight());
    }

    // Animation
    pAniData->update_particle_end_time(aTrack->GetTrackID(), aTrack->GetGlobalTime());

    // Find MC in collection
    if (dControl->save_MC) {
        auto MCCols = dRootMng->GetEvt()->getMcParticleCollection().at(dControl->RawMCCollection_Name);
        auto p = McParticle::SearchID(MCCols, aTrack->GetTrackID());
        if (p) {
            //p->setERemain(aTrack->GetKineticEnergy());
            p->setERemain(aTrack->GetTotalEnergy());
            p->setEndPointX(aTrack->GetStep()->GetPostStepPoint()->GetPosition()[0]);
            p->setEndPointY(aTrack->GetStep()->GetPostStepPoint()->GetPosition()[1]);
            p->setEndPointZ(aTrack->GetStep()->GetPostStepPoint()->GetPosition()[2]);
        }
    }

    // For new truth classification
    auto truth_particle = dTMgr->getTruthParticle();
    if (aTrack->GetStep()->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "Decay")
        truth_particle->E_remain = 0.;
    else
        truth_particle->E_remain = static_cast<float>(aTrack->GetKineticEnergy());

    // fill into DTruth
    if (
            (truth_particle->E_remain >= dControl->E_remain_min || truth_particle->E_leak >= dControl->E_leak_min) ||
            aTrack->GetParentID()==0 ||
            truth_particle->E_kin >= dControl->E_kin_min_record
            )
        dRootMng->GetEvt()->getTruthInfo()->UpdateTruthParticle(truth_particle);
    else {
        if (auto l = truth_particle->prod_process_link; l != nullptr) {
            if (auto pos = std::find(l->out_p.begin(), l->out_p.end(), truth_particle); pos != l->out_p.end())
                l->out_p.erase(pos);
        }
        dTMgr->Clean();
    }
    // Clean truth tracks if there is no fill in
    if (dTMgr->getTruthState() && dTMgr->getTruthState()->empty()) {
        dRootMng->GetEvt()->getTruthInfo()->removeTruthTrack(
                {aTrack->GetTrackID(), aTrack->GetParticleDefinition()->GetPDGEncoding()});
    }
    dTMgr->setTruthState(nullptr);

    delete fMC;
    fMC = nullptr;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
