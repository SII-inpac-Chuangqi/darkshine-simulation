//
// Created by zhuxu on 2021/1/31.
//

#include "DP_simu/StackingAction.hh"
#include "Control/Control.h"

#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4Track.hh"

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
        case 0: // Stage 0 : Only accept primary track
            if(aTrack->GetParentID()==0) {
                classification = fUrgent;
            }
            break;
        case 1: // Stage 1 : Only accept secondaries in region of interest
            if(InsideRoI(aTrack)) {
                classification = fUrgent;
            }
            break;
        default: // Stage 2 : Accept all primaries
                 //           Accept all secondaries
            classification = fUrgent;
            break;
    }
    return classification;
}

void StackingAction::NewStage() {
    dControl->fStage++;
    stackManager->ReClassify();
}

void StackingAction::PrepareNewEvent() {
    dControl->fStage = 0;
}