//
// Created by Zhang Yulei on 12/27/20.
//

#include <utility>

#include "Bias_Filter/FilterProcess.hh"

FilterProcess::FilterProcess(G4String processName,
                             G4double minEnergy,
                             G4double maxEnergy,
                             G4double minScanDistance,
                             G4double maxScanDistance,
                             G4bool flag) {
    Process_Name = std::move(processName);
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
G4bool FilterProcess::In_Range(G4double val, G4double lowerBound, G4double upperBound) {
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

/// \brief Check if one particular process is in particular energy range and distance range.
/// \return true - in the range,
/// fasle - out of range.
G4bool FilterProcess::In_Filter(const G4Step* aStep) {
    prev = aStep->GetPreStepPoint();
    post = aStep->GetPostStepPoint();
    deltaE = fabs( prev->GetKineticEnergy() - post->GetKineticEnergy() );
    pname = post->GetProcessDefinedStep()->GetProcessName();
    post_distance = post->GetPosition()[2];

    if (pname.contains( Process_Name )) {
        if (In_Range(post_distance, ScanDistance_Min, ScanDistance_Max)) {
            if (In_Range(deltaE, Energy_Min, Energy_Max)) {
                    Found_Result = true;
                    return true;
            } else
                return false;
        } else
            return false;
    }
    return false;
}
