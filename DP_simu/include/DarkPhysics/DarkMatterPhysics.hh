#ifndef DarkMatterPhysics_h
#define DarkMatterPhysics_h 1

#pragma once

#include "G4VPhysicsConstructor.hh"

class DarkMatter;

class DarkPhotons;

class DarkMatterPhysics : public G4VPhysicsConstructor {
public:
    DarkMatterPhysics();

    ~DarkMatterPhysics();

    // Should call initial constructor of particle singletons
    void ConstructParticle() override;

    void ConstructProcess() override;

    DarkPhotons *GetDarkMatterPointer() { return myDarkMatter; }

    G4double GetBiasSigmaFactor() { return BiasSigmaFactor; }

private:
    DarkMatterPhysics(const DarkMatterPhysics &) = delete;

    DarkMatterPhysics &operator=(const DarkMatterPhysics &) = delete;

private:
    DarkPhotons *myDarkMatter;
    G4double BiasSigmaFactor;
};

#endif