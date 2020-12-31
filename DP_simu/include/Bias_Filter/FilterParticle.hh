#ifndef FILTER_PARTICLE_H
#define FILTER_PARTICLE_H

#include "G4Step.hh"

#include <vector>

/// \brief Class Description:
///   This class will be put in a vector container in class FilterManager.
class FilterParticle {
public:
    FilterParticle(G4int pdg,
                   G4double minEnergy,
                   G4double maxEnergy,
                   G4double minScanDistance,
                   G4double maxScanDistance,
                   G4bool flag);

    virtual ~FilterParticle() = default;

    static  G4bool In_Range(G4double val, G4double lowerBound, G4double upperBound);

    G4bool In_Filter(const G4Step *aStep);

    /// Setter

    void SetFoundResult(G4bool in) { Found_Result = in; };
    // Found_Result record whether have found th particle in
    // particular range. Set to false at the beginning of the event.

    /// Getter
    [[nodiscard]] G4int GetPDGID() const { return PDG; };

    [[nodiscard]] G4bool GetFlag() const { return Flag; };

    [[nodiscard]] G4bool GetFoundResult() const { return Found_Result; };
private:
    G4StepPoint *prev{};
    G4StepPoint *post{};
    G4double post_distance{};
    G4double deltaE{};
    G4double Energy_Min;
    G4double Energy_Max;

    G4double ScanDistance_Min;
    G4double ScanDistance_Max;

    G4int TrackID = 1;
    const std::vector<const G4Track *> *secondary{};
    std::vector<const G4Track *>::const_iterator sec_itr;
    std::vector<const G4Track *>::const_iterator sec_end;
    const G4Track *aTrack{};
    G4int PDG;
    G4double energy{};
    G4bool Flag;
    G4bool Found_Result = false; // true if found this particle in the range.
};

#endif