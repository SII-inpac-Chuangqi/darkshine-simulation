//
// Created by zhuxu on 2021/1/31.
//

#ifndef DSIMU_STACKINGACTION_HH
#define DSIMU_STACKINGACTION_HH

#include "globals.hh"
#include "G4UserStackingAction.hh"
#include "G4ThreeVector.hh"

class G4Track;

#include "DP_simu/SimHit.hh"
#include "Bias_Filter/FilterManager.hh"

class StackingAction : public G4UserStackingAction
{
public:
    StackingAction();
    virtual ~StackingAction();

public:
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
    virtual void NewStage();
    virtual void PrepareNewEvent();

private:
    G4bool InsideRoI(const G4Track* aTrack) { return dFilterManager->InsideRoI(aTrack); };

};

#endif //DSIMU_STACKINGACTION_HH
