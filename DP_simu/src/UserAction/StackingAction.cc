//
// Created by zhuxu on 2021/1/31.
//

#include "DP_simu/StackingAction.hh"
#include "Control/Control.h"

#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

#include <vector>
#include <tuple>

StackingAction::StackingAction()
 : G4UserStackingAction() {
    dControl->fStage = 0;
}

StackingAction::~StackingAction() {
}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track *aTrack) {
    G4ClassificationOfNewTrack classification = fWaiting;
    switch(dControl->fStage)
    {
        case 0: // Stage 0 : Tracks in RoI only
            if(InsideRoI(aTrack)) {
                classification = fUrgent;
            }
            break;
        default: //Stage 1 : Accept all primaries
                 // Accept all secondaries
            classification = fUrgent;
            break;
    }
    return classification;
}

G4bool StackingAction::InsideRoI(const G4Track *aTrack) {
    const G4double trPos = aTrack->GetPosition()[2];
    for (const auto& roi : dControl->region_of_interest) {
        const G4double minDist = std::get<0>(roi);
        const G4double maxDist = std::get<1>(roi);
        if(minDist < trPos && trPos < maxDist) {
            return true;
        }
    }
    return false;
}

void StackingAction::NewStage() {
    dControl->fStage++;
    stackManager->ReClassify();
}

void StackingAction::PrepareNewEvent() {
    dControl->fStage = 0;
}