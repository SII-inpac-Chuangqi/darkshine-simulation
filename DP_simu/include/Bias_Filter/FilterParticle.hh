#ifndef FILTER_PARTICLE_H
#define FILTER_PARTICLE_H

#include "G4Step.hh"

#include <vector>

/// \brief Class Description:
///   This class will be put in a vector container in class FilterManager.
class FilterParticle {
public:
    FilterParticle(G4int pdg, G4double risingEnergyEdge, G4double fallingEnergyEdge,
                   G4double risingScanEdge, G4double fallingScanEdge, G4bool flag);

    virtual ~FilterParticle() = default;

    static  G4bool Square_Filter(G4double val, G4double risingEdge, G4double fallingEdge);

    G4bool Filter(const G4Step *aStep);

    /// Setter

    void SetFoundResult(G4bool in) { Found_Result = in; };

    /// Getter
    [[nodiscard]] G4int GetPDGID() const { return PDG; };

    [[nodiscard]] G4bool GetFlag() const { return Flag; };

    [[nodiscard]] G4bool GetFoundResult() const { return Found_Result; };
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

    G4int TrackID = 1;
    const std::vector<const G4Track *> *secondary{};
    std::vector<const G4Track *>::const_iterator sec_itr;
    std::vector<const G4Track *>::const_iterator sec_end;
    const G4Track *aTrack{};
    G4int PDG;
    G4double energy{};
    G4bool Flag;
    G4bool Found_Result = false; // true if found this particle in the range of filter.
};

#endif