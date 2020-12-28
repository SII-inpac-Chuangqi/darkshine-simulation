//
// Created by Zhang Yulei on 12/27/20.
//

#include "Bias_Filter/FilterParticle.hh"

FilterParticle::FilterParticle(G4int pdg, G4double risingEnergyEdge, G4double fallingEnergyEdge,
                               G4double risingScanEdge, G4double fallingScanEdge, G4bool flag) {
    PDG = pdg;
    Energy_Edge_Rising = risingEnergyEdge;
    Energy_Edge_Falling = fallingEnergyEdge;
    ScanDistance_Edge_Rising = risingScanEdge;
    ScanDistance_Edge_Falling = fallingScanEdge;
    Flag = flag;
}

/// \brief simple filter
/// \return true - pass, false - stop.
G4bool FilterParticle::Square_Filter(G4double val, G4double risingEdge, G4double fallingEdge) {
    // All-pass case: fallingEdge=risingEdge, return true
    if ( fallingEdge <= risingEdge) {
        if (val >= risingEdge || val < fallingEdge)
            return true;
        else
            return false;
    }
    else { // risingEdge < fallingEdge
        if (val >= risingEdge && val < fallingEdge)
            return true;
        else
            return false;
    }
}

/// \brief Check if one particular Particle is in particular energy range and distance range.
/// \return true - in the range,
/// false - out of range.
G4bool FilterParticle::Filter(const G4Step* aStep) {
    post = aStep->GetPostStepPoint();
    post_distance = post->GetPosition()[2];
    if (Square_Filter(post_distance, ScanDistance_Edge_Rising, ScanDistance_Edge_Falling)) {
        prev = aStep->GetPreStepPoint();
        // search for all the secondary particles produced in this step
        secondary = aStep->GetSecondaryInCurrentStep();
        sec_end = (*secondary).end();
        for ( sec_itr = (*secondary).begin(); sec_itr != sec_end; sec_itr++ ) {
            aTrack = (*sec_itr);
            // Select particle
            if ( PDG != aTrack->GetParticleDefinition()->GetPDGEncoding() ) continue;
            // Energy of secondaries reqirement
            energy = aTrack->GetTotalEnergy();
            if( Square_Filter(energy, Energy_Edge_Rising, Energy_Edge_Falling) ) {
                return true;
            }
        }
        return false;
    }
    else
        return false;
}
