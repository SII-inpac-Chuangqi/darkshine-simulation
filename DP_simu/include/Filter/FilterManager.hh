
#ifndef FILTER_MANAGER_H
#define FILTER_MANAGER_H

#include "Filter/FilterParticle.hh"
#include "Filter/FilterProcess.hh"

/// \brief Class Description:
///  Take particle filter as an example. 
///  First, SetNewParticleFilter() will construct new class FilterParticle class,
///  initialize PDG, risingEnergyEdge, fallingEnergyEdge, etc.
///  The pointer to this class is stored in vector Filter_Particle_List.
///  Second, these class will be traversed in SteppingAction.
class FilterManager {
public:
    FilterManager() = default;

    virtual ~FilterManager() = default;

    void Filter_Track_Initialize();

    void Filter_Event_Initialize();

    /// Filter method

    G4bool Filter_Particle(const G4Step* aStep);
    
    void Filter_Process(const G4Step* aStep);

    /// Setter

    /// \brief Setup a new particle filter.
    /// \param pdg  PDG ID of secondary particle.
    /// \param risingEnergyEdge  The rising edge of Energy range.
    /// \param fallingEnergyEdge  The falling edge of Energy range.
    /// \param risingScanEdge  The rising edge of Scan Distance range.
    /// \param fallingScanEdge  The falling edge of Scan Distance range.
    /// \param flag  1: The Event to be computed must have this particle in particular range.
    ///              0: The Event to be computed must not have this particle in particular range.
    void SetNew_Particle_Filter(G4int pdg, G4double risingEnergyEdge, G4double fallingEnergyEdge,
                                G4double risingScanEdge, G4double fallingScanEdge, G4bool flag);
    /// \brief Setup a new process filter.
    /// \param processName  process name of post step point.
    /// \param risingEnergyEdge  The rising edge of Energy range.
    /// \param fallingEnergyEdge  The falling edge of Energy range.
    /// \param risingScanEdge  The rising edge of Scan Distance range.
    /// \param fallingScanEdge  The falling edge of Scan Distance range.
    /// \param flag  1: The Event to be computed must have this process in particular range.
    ///              0: The Event to be computed must not have this process in particular range.
    void SetNew_Process_Filter(G4String processName, G4double risingEnergyEdge, G4double fallingEnergyEdge,
                               G4double risingScanEdge, G4double fallingScanEdge, G4bool flag);

    /// Getter

    G4bool GetifFilter_Particle() { return ifFilter_Particle; };

    G4bool GetifFilter_Process() { return ifFilter_Process; };

    G4bool GetFilter_Process_Result() { return Filter_Process_Result; } ;

private:
    G4bool ifFilter_Particle = false;
    G4bool ifFilter_Process = false;
    std::vector<std::shared_ptr<FilterParticle>> Filter_Particle_List{};
    std::vector<std::shared_ptr<FilterProcess>> Filter_Process_List{};
    std::vector<std::shared_ptr<FilterParticle>>::iterator particle_itr;
    std::vector<std::shared_ptr<FilterParticle>>::iterator particle_end;
    std::vector<std::shared_ptr<FilterProcess>>::iterator process_itr;
    std::vector<std::shared_ptr<FilterProcess>>::iterator process_end;
    G4bool Filter_Particle_Result;
    G4bool Filter_Process_Result;
};
#endif // FILTER_MANAGER_H
