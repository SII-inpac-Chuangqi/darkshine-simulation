#ifndef FILTER_PROCESS_H
#define FILTER_PROCESS_H

#include "G4Step.hh"
#include "G4VProcess.hh"

#include <vector>

/// \brief Class Description:
///  This class will be put in a vector container in class FilterManager.
class FilterProcess {
public:
    FilterProcess(G4String processName, G4double risingEnergyEdge, G4double fallingEnergyEdge,
                  G4double risingScanEdge, G4double fallingScanEdge, G4bool flag);

    virtual ~FilterProcess() = default;

    static G4bool Square_Filter(G4double val, G4double risingEdge, G4double fallingEdge);

    G4bool Filter(const G4Step *aStep);

    /// Getter
    G4bool GetFlag() const { return Flag; };

private:
    G4bool ifFilter{};
    G4StepPoint *prev{};
    G4StepPoint *post{};
    G4double post_distance{};
    G4double deltaE{};
    G4double Energy_Edge_Rising;
    G4double Energy_Edge_Falling;

    G4double ScanDistance_Edge_Rising;
    G4double ScanDistance_Edge_Falling;

    G4String Process_Name{};
    G4String pname;
    G4bool res = false;
    G4bool Flag;
};

#endif