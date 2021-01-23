#ifndef DMParticleAPrime_h
#define DMParticleAPrime_h 1

#pragma once

#include <G4ParticleDefinition.hh>

class DMParticleAPrime : public G4ParticleDefinition {
private:
    static DMParticleAPrime *theInstance;

    DMParticleAPrime() = default;

    ~DMParticleAPrime() override = default;

public:
    static DMParticleAPrime *Definition();
};

#endif