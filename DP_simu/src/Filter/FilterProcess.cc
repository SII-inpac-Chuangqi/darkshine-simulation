//
// Created by Zhang Yulei on 12/27/20.
//

#include <utility>

#include "Filter/FilterProcess.hh"

FilterProcess::FilterProcess(G4String processName, G4double risingEnergyEdge, G4double fallingEnergyEdge,
                             G4double risingScanEdge, G4double fallingScanEdge, G4bool flag) {
    Process_Name = std::move(processName);
    Energy_Edge_Rising = risingEnergyEdge;
    Energy_Edge_Falling = fallingEnergyEdge;
    ScanDistance_Edge_Rising = risingScanEdge;
    ScanDistance_Edge_Falling = fallingScanEdge;
    Flag = flag;
}

/// \brief simple filter
/// \return true - pass, false - stop.
G4bool FilterProcess::Square_Filter(G4double val, G4double risingEdge, G4double fallingEdge) {
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

/// \brief Check if one particular process is in particular energy range and distance range.
/// \return true - in the range,
/// fasle - out of range.
G4bool FilterProcess::Filter(const G4Step* aStep) {
    prev = aStep->GetPreStepPoint();
    post = aStep->GetPostStepPoint();
    deltaE = fabs( prev->GetKineticEnergy() - post->GetKineticEnergy() );
    pname = post->GetProcessDefinedStep()->GetProcessName();
    post_distance = post->GetPosition()[2];

    if  ( Square_Filter(post_distance, ScanDistance_Edge_Rising, ScanDistance_Edge_Falling) ) {
        if ( Square_Filter(deltaE, Energy_Edge_Rising, Energy_Edge_Falling)) {
            res = pname.contains( Process_Name );
            if (res) return true;
        }
        else
            return false;
    }
    else
        return false;

    return false;
}
