#ifndef FILTER_PARTICLE_H
#define FILTER_PARITCLE_H

#include "G4Step.hh"

#include <vector>

/// \brief Class Description:
///   This class will be put in a vector container in class FilterManager.
class FilterParticle {
public:
    FilterParticle(G4int pdg, G4double risingEdge, G4double fallingEdge,
                   G4double minDistance, G4double maxDistance);
    virtual ~FilterParticle() = default;

    G4bool Energy_Filter(G4double in);
    G4bool Filter(const G4Step* aStep);
    /// Getter
    G4int GetPDGID() { return PDG; };
private:
    G4bool ifFilter;
    G4StepPoint *prev;
    G4StepPoint *post;
    G4double post_distance;
    G4double deltaE;
    G4double Edge_Rising;
    G4double Edge_Falling;

    G4double Filter_Result = false;
    G4double ScanDistance_Min;
    G4double ScanDistance_Max;

    G4int TrackID = 1;
    const std::vector< const G4Track* > *secondary;
    std::vector< const G4Track* >::const_iterator sec_itr;
    std::vector< const G4Track* >::const_iterator sec_end;
    const G4Track *aTrack;
    G4int PDG;
    G4double energy;
};
#endif