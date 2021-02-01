
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
private:
    FilterManager();

    virtual ~FilterManager() = default;

public:

    FilterManager(const FilterManager &) = delete;

    FilterManager &operator=(FilterManager const &) = delete;

    static FilterManager *CreateInstance();

    [[maybe_unused]] void Filter_Track_Initialize();

    void Filter_Event_Initialize();

    /// Filter method

    G4bool Filter_Particle(const G4Step *aStep);

    void Filter_Process(const G4Step *aStep);

    [[nodiscard]] G4bool Filter_Particle_Found_Result(); /// check whether found must-have particle.

    [[nodiscard]] G4bool Filter_Process_Found_Result(); /// check whether found must-have process.


    /// Getter
    [[nodiscard]] G4bool GetifFilter_Particle() const { return ifFilter_Particle; };

    [[nodiscard]] G4bool GetifFilter_Process() const { return ifFilter_Process; };

    [[nodiscard]] G4bool GetFilter_Process_Result() const { return Filter_Process_Result; };

private:
    G4bool ifFilter_Particle = false;
    G4bool ifFilter_Process = false;
    std::vector<std::shared_ptr<FilterParticle>> Filter_Particle_List{};
    std::vector<std::shared_ptr<FilterProcess>> Filter_Process_List{};
//    std::vector<std::shared_ptr<FilterHighEParticle>> Particle_HighE_Filter_List{};
//    std::vector<std::shared_ptr<FilterHighEProcess>> Process_HighE_Filter_List{};

    G4bool Filter_Particle_Result{};
    G4bool Filter_Process_Result{};
};

extern FilterManager *dFilterManager;
#endif // FILTER_MANAGER_H
