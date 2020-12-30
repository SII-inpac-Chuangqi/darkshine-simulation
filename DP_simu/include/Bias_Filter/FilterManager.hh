
#ifndef FILTER_MANAGER_H
#define FILTER_MANAGER_H

#include "Bias_Filter/FilterParticle.hh"
#include "Bias_Filter/FilterProcess.hh"

/// \brief Class Description:
///  Take particle filter as an example. 
///  First, SetNewParticleFilter() will construct new class FilterParticle class,
///  initialize PDG, Energy range, Scan Distance range, etc.
///  The pointer to this class is stored in vector Filter_Particle_List.
///  Second, these class will be traversed in SteppingAction.
class FilterManager {
public:
    FilterManager() = default;

    virtual ~FilterManager() = default;

    void Filter_Track_Initialize();

    void Filter_Event_Initialize();

    /// Filter method

    G4bool Filter_Particle(const G4Step *aStep);

    void Filter_Process(const G4Step *aStep);

    [[nodiscard]] G4bool Filter_Particle_Found_Result(); /// check whether found must-have particle.

    [[nodiscard]] G4bool Filter_Process_Found_Result(); /// check whether found must-have process.

    /// Setter

    /// \brief Setup a new particle filter.
    /// \param pdg  PDG ID of secondary particle.
    /// \param flag  1: The Event to be computed must have this particle in particular range.
    ///              0: The Event to be computed must not have this particle in particular range.
    void SetNew_Particle_Filter(G4int pdg,
                                G4double minEnergy,
                                G4double maxEnergy,
                                G4double minScanDistance,
                                G4double maxScanDistance,
                                G4bool flag);

    /// \brief Setup a new process filter.
    /// \param processName  process name of post step point.
    /// \param flag  1: The Event to be computed must have this process in particular range.
    ///              0: The Event to be computed must not have this process in particular range.
    void SetNew_Process_Filter(G4String processName,
                               G4double minEnergy,
                               G4double maxEnergy,
                               G4double minScanDistance,
                               G4double maxScanDistance,
                               G4bool flag);

    /// Getter
    [[nodiscard]] G4bool GetifFilter_Particle() const { return ifFilter_Particle; };

    [[nodiscard]] G4bool GetifFilter_Process() const { return ifFilter_Process; };

    [[nodiscard]] G4bool GetFilter_Process_Result() const { return Filter_Process_Result; };

private:
    G4bool ifFilter_Particle = false;
    G4bool ifFilter_Process = false;
    std::vector<std::shared_ptr<FilterParticle>> Filter_Particle_List{};
    std::vector<std::shared_ptr<FilterProcess>> Filter_Process_List{};
    std::vector<std::shared_ptr<FilterParticle>>::iterator particle_itr;
    std::vector<std::shared_ptr<FilterParticle>>::iterator particle_end;
    std::vector<std::shared_ptr<FilterProcess>>::iterator process_itr;
    std::vector<std::shared_ptr<FilterProcess>>::iterator process_end;
    G4bool Filter_Particle_Result{};
    G4bool Filter_Process_Result{};
};

#endif // FILTER_MANAGER_H
