//
// Created by Zhang Yulei on 12/27/20.
//

#include <utility>

#include "Bias_Filter/FilterProcess.hh"
#include "Control/Control.h"

FilterProcess::FilterProcess(G4String processName,
                             G4double minEnergy,
                             G4double maxEnergy,
                             G4double minScanDistance,
                             G4double maxScanDistance,
                             G4bool flag,
                             G4bool fStage0,
                             G4bool fStage1) {
    Process_Name = std::move(processName);
    Energy_Min = minEnergy;
    Energy_Max = maxEnergy;
    ScanDistance_Min = minScanDistance;
    ScanDistance_Max = maxScanDistance;
    Flag = flag;
    ifStage0 = fStage0;
    ifStage1 = fStage1;
    if (maxEnergy < 0) infty_maxE = true;
}

///// \brief judge if val is in the range.
///// lowerBound < upperBound: band-pass;
///// lowerBound > upperBound : band-stop;
///// lowerBound = upperBound : all-pass.
///// \return true - in the range, false - out of range.
//G4bool FilterProcess::In_Range(G4double val, G4double lowerBound, G4double upperBound) {
//    if (upperBound <= lowerBound) {
//        if (val >= lowerBound || val < upperBound)
//            return true;
//        else
//            return false;
//    }
//    else { // lowerBound < upperBound
//        if (val >= lowerBound && val < upperBound)
//            return true;
//        else
//            return false;
//    }
//}

/// \brief Check if one particular process is in particular energy range and distance range.
/// \return true - in the range,
/// fasle - out of range.
G4bool FilterProcess::In_Filter(const G4Step *aStep) {
    if (dControl->fStage == 1 && !ifStage1) return false;
    if (dControl->fStage == 0 && !ifStage0) return false;
    prev = aStep->GetPreStepPoint();
    prev_E = prev->GetKineticEnergy();
    if (prev_E < Energy_Min) return false;

    post = aStep->GetPostStepPoint();
    deltaE = fabs(prev_E - post->GetKineticEnergy());
    pname = post->GetProcessDefinedStep()->GetProcessName();
    post_distance = post->GetPosition()[2];

    if (deltaE >= Energy_Min
        && (infty_maxE || deltaE < Energy_Max)
        && post_distance >= ScanDistance_Min
        && post_distance < ScanDistance_Max
        && pname.contains(Process_Name)) {
        Found_Result = true;
        return true;
    } else
        return false;
}
