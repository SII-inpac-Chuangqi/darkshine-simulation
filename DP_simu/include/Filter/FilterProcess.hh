#ifndef FILTER_PROCESS_H
#define FILTER_PROCESS_H

#include "G4Step.hh"
#include "G4VProcess.hh"

#include <vector>

/// \brief Class Description:
///  This class will be put in a vector container in class FilterManager.
class FilterProcess {
public:
    FilterProcess(G4String processName, G4double risingEdge, G4double fallingEdge,
                  G4double minDistance, G4double maxDistance);
    virtual ~FilterProcess() = default;
    G4bool Energy_Filter(G4double in);
    G4bool Filter(const G4Step* aStep);
private:
    G4bool ifFilter;
    G4StepPoint *prev;
    G4StepPoint *post;
    G4double post_distance;
    G4double deltaE;
    G4double Edge_Rising;
    G4double Edge_Falling;

    G4double ScanDistance_Min;
    G4double ScanDistance_Max;
    
    G4String Process_Name{};
    G4String pname;
    G4bool res = false;
};

#endif