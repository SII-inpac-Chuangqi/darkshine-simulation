
#ifndef DMBremProcess_h
#define DMBremProcess_h 1

#pragma once

#include <G4VDiscreteProcess.hh>

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

private:

    DarkPhotons *myDarkMatter;
    G4double BiasSigmaFactor;
};

#endif