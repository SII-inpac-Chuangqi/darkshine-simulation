#ifndef FILTER_PROCESS_H
#define FILTER_PROCESS_H

#include "G4Step.hh"
#include "G4VProcess.hh"

#include <vector>

/// \brief Class Description:
///  This class will be put in a vector container in class FilterManager.
class FilterProcess {
public:
    FilterProcess(G4String processName,
                  G4double minEnergy,
                  G4double maxEnergy,
                  G4double minScanDistance,
                  G4double maxScanDistance,
                  G4bool flag,
                  G4bool fStage0,
                  G4bool fStage1);

    virtual ~FilterProcess() = default;

    //static G4bool In_Range(G4double val, G4double lowerBound, G4double upperBound);

    G4bool In_Filter(const G4Step *aStep);

    /// Setter

    void SetFoundResult(G4bool in) { Found_Result = in; };

    /// Getter
    [[nodiscard]] G4bool GetFlag() const { return Flag; };

    [[nodiscard]] G4bool GetFoundResult() const { return Found_Result; };

protected:
    G4StepPoint *prev{};
    G4StepPoint *post{};
    G4double post_distance{};
    G4double deltaE{};
    G4double prev_E{};
    G4double Energy_Min;
    G4double Energy_Max;
    G4bool infty_maxE = false;

    G4double ScanDistance_Min;
    G4double ScanDistance_Max;

    G4String Process_Name{};
    G4String pname;
    G4bool res = false;
    G4bool Flag;
    G4bool Found_Result = false; // true if found this process in the range.
    G4bool ifStage0;
    G4bool ifStage1;
};

#endif