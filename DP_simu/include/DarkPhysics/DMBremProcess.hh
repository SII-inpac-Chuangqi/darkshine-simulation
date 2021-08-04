
#ifndef DMBremProcess_h
#define DMBremProcess_h 1

#pragma once

#include <G4VDiscreteProcess.hh>
//here is the additional headers
#include "TH2.h"
#include "TRandom.h"
#include "TFile.h"
#include "Control/Control.h"


class DarkMatter;

class G4ParticleDefinition;

#include "DarkPhotons.hh"

class DMBremProcess : public G4VDiscreteProcess {
public:

    G4ParticleDefinition *theDMParticlePtr;

public:

    DMBremProcess(DarkPhotons *DarkMatterPointer, G4ParticleDefinition *theDMParticlePtrIn, G4double BiasSigmaFactor);

    // Implements final state parameters when the process won.
    G4VParticleChange *PostStepDoIt(const G4Track &, const G4Step &) override;

    G4double GetMeanFreePath(const G4Track &aTrack,
                             G4double previousStepSize,
                             G4ForceCondition *condition) override;

    G4bool IsApplicable(const G4ParticleDefinition &) override;

    void LUTRandom();

private:

    DarkPhotons *myDarkMatter;
    G4double BiasSigmaFactor;

    //here is the additional variables -- by Xiang
    TFile *f = nullptr;
    TH2F *Lut_hist = nullptr;
    double var_LUT[2]{};
    double energy = 0;
    double recoil_E = 0;


};

#endif