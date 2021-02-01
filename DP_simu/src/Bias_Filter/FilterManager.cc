//
// Created by Zhang Yulei on 12/27/20.
//

#include "Bias_Filter/FilterManager.hh"
#include "Control/Control.h"

#include <algorithm>

// Required by Singleton
FilterManager *dFilterManager = nullptr;

// Get Instance Class
FilterManager *FilterManager::CreateInstance() {
    if (dFilterManager == nullptr)
        dFilterManager = new FilterManager();

    return dFilterManager;
}

FilterManager::FilterManager() {

    /// \brief Setup a new particle filter.
    /// \param pdg  PDG ID of secondary particle.
    /// \param flag  1: The Event to be computed must have this particle in particular range.
    ///              0: The Event to be computed must not have this particle in particular range.
    for (auto para : dControl->particle_filters_parameters) {
        ifFilter_Particle = true;
        Filter_Particle_List.emplace_back(std::make_shared<FilterParticle>(std::get<0>(para),
                                                                           std::get<1>(para),
                                                                           std::get<2>(para),
                                                                           std::get<3>(para),
                                                                           std::get<4>(para),
                                                                           std::get<5>(para)));
    }

    /// \brief Setup a new process filter.
    /// \param processName  process name of post step point.
    /// \param flag  1: The Event to be computed must have this process in particular range.
    ///              0: The Event to be computed must not have this process in particular range.
    for (auto para : dControl->process_filters_parameters) {
        ifFilter_Process = true;
        Filter_Process_List.emplace_back(std::make_shared<FilterProcess>(std::get<0>(para),
                                                                         std::get<1>(para),
                                                                         std::get<2>(para),
                                                                         std::get<3>(para),
                                                                         std::get<4>(para),
                                                                         std::get<5>(para)));
    }
}

/// \brief Filter Particle method. Scan every FilterParticle->Filter().
/// \return true - keep event,
/// false - abort evet.
G4bool FilterManager::Filter_Particle(const G4Step* aStep) {
    /// DEBUG
    G4cerr << "Filter_Particle called" << G4endl;
    for (const auto& particle_filter : Filter_Particle_List) {
        const std::shared_ptr<FilterParticle>& fFilterParticle = particle_filter;
        if (fFilterParticle->In_Filter(aStep) ) { // found particle in particular range.
            if ( !fFilterParticle->GetFlag() ) { // don't want this particle.
                Filter_Particle_Result = false;
                return false;
            }
            // else
        }
    }
    Filter_Particle_Result = true; // No particle we don't want.
    return Filter_Particle_Result;
}

G4bool FilterManager::Filter_Particle_Found_Result() {
    for (const auto& particle_filter : Filter_Particle_List) {
        const std::shared_ptr<FilterParticle>& fFilterParticle = particle_filter;
        if ( fFilterParticle->GetFlag() && !fFilterParticle->GetFoundResult() ) {
            return false; // user want this particle but not found, abort.
        }
    }

    // Another version using std::any_of and Lambda expression
//    if (std::any_of(Filter_Particle_List.cbegin(), Filter_Particle_List.cend(),
//                    [](auto x){return (x->GetFlag() && !x->GetFoundResult());}))
//        return false;

    return true; // keep
}

/// \brief Filter Process method. Scan every FilterProcess->Filter().
/// Store result in Filter_Process_Result.
void FilterManager::Filter_Process(const G4Step* aStep) {
    for (const auto& process_filter : Filter_Process_List) {
        const std::shared_ptr<FilterProcess>& fFilterProcess = process_filter;
        if (fFilterProcess->In_Filter(aStep) ) { // found process in particular range.
            if ( !fFilterProcess->GetFlag() ) { // don't want this process
                Filter_Process_Result = false;
                return;
            }
        }
    }
    Filter_Process_Result = true;
}

G4bool FilterManager::Filter_Process_Found_Result() {
    for (const auto& process_filter : Filter_Process_List) {
        const std::shared_ptr<FilterProcess>& fFilterProcess = process_filter;
        if ( fFilterProcess->GetFlag() && !fFilterProcess->GetFoundResult() ) {
            return false; // user want this process but not found, abort.
        }
    }
    return true; // keep
}

[[maybe_unused]] void FilterManager::Filter_Track_Initialize() {}

void FilterManager::Filter_Event_Initialize() {
    Filter_Particle_Result = true;
    Filter_Process_Result = true;
    // set Found_Result of particle to false
    for (const auto& process_particle : Filter_Particle_List) {
        process_particle->SetFoundResult(false);
    }
    // set Found_Result of process to false
    for (const auto& process_filter : Filter_Process_List) {
        process_filter->SetFoundResult(false);
    }
}

