//
// Created by Zhang Yulei on 12/27/20.
//

#include "Bias_Filter/FilterParticle.hh"

FilterParticle::FilterParticle(G4int pdg,
                               G4double minEnergy,
                               G4double maxEnergy,
                               G4double minScanDistance,
                               G4double maxScanDistance,
                               G4bool flag) {
    PDG = pdg;
    Energy_Min = minEnergy;
    Energy_Max = maxEnergy;
    ScanDistance_Min = minScanDistance;
    ScanDistance_Max = maxScanDistance;
    Flag = flag;
}

/// \brief judge if val is in the range.
/// lowerBound < upperBound: band-pass;
/// lowerBound > upperBound : band-stop;
/// lowerBound = upperBound : all-pass.
/// \return true - in the range, false - out of range.
G4bool FilterParticle::In_Range(G4double val, G4double lowerBound, G4double upperBound) {
    if (upperBound <= lowerBound) {
        if (val >= lowerBound || val < upperBound)
            return true;
        else
            return false;
    }
    else { // lowerBound < upperBound
        if (val >= lowerBound && val < upperBound)
            return true;
        else
            return false;
    }
}

/// \brief Check if one particular Particle is in particular energy range and distance range.
/// \return true - in the range,
/// false - out of range.
G4bool FilterParticle::In_Filter(const G4Step* aStep) {
    post = aStep->GetPostStepPoint();
    post_distance = post->GetPosition()[2];
    if (In_Range(post_distance, ScanDistance_Min, ScanDistance_Max)) {
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
            if(In_Range(energy, Energy_Min, Energy_Max) ) {
                Found_Result = true;
                return true;
            }
        }
        return false;
    }
    else
        return false;
}
