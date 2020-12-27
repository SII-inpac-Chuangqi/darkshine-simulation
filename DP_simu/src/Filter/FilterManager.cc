//
// Created by Zhang Yulei on 12/27/20.
//

#include "Filter/FilterManager.hh"

/// \brief Filter Particle method. Scan every FilterParticle->Filter().
/// \return true - keep event,
/// false - abort evet.
G4bool FilterManager::Filter_Particle(const G4Step* aStep) {
    particle_end = Filter_Particle_List.end();
    for (particle_itr = Filter_Particle_List.begin(); particle_itr != particle_end; particle_itr++) {
        std::shared_ptr<FilterParticle> fFilterParticle = *particle_itr;
        if ( fFilterParticle->Filter(aStep) ) { // found particle in particular range.
            if ( !fFilterParticle->GetFlag() ) { // don't want this particle.
                Filter_Particle_Result = false;
                return false;
            }
            // else,
        }
        else { // not found this particle in particular range.
            if (fFilterParticle->GetFlag()) { // but this particle is must-have.
                Filter_Particle_Result = false;
                return false;
            }
        }
    }
    Filter_Particle_Result = true; // found all particle we want to see, and no particle we don't want.
    return Filter_Particle_Result;
}

/// \brief Filter Process method. Scan every FilterProcess->Filter().
/// Store result in Filter_Process_Result.
void FilterManager::Filter_Process(const G4Step* aStep) {
    process_end = Filter_Process_List.end();
    for (process_itr = Filter_Process_List.begin(); process_itr != process_end; process_itr++) {
        std::shared_ptr<FilterProcess> fFilterProcess = *process_itr;
        if ( fFilterProcess->Filter(aStep) ) { // found process in particular range.
            if ( !fFilterProcess->GetFlag() ) { // don't want this process
                Filter_Process_Result = false;
                return;
            }
        }
        else { // not found process in particular range
            if ( fFilterProcess->GetFlag() ) { // but must have this process
                Filter_Process_Result = false;
                return;
            }
        }
    }
    Filter_Process_Result = true;
}

void FilterManager::Filter_Track_Initialize() {

}

void FilterManager::Filter_Event_Initialize() {
    Filter_Particle_Result = true;
    Filter_Process_Result = true;
}

void FilterManager::SetNew_Particle_Filter(G4int pdg, G4double risingEnergyEdge, G4double fallingEnergyEdge,
                                           G4double risingScanEdge, G4double fallingScanEdge, G4bool flag)
{
    ifFilter_Particle = true;
    Filter_Particle_List.emplace_back(std::make_shared<FilterParticle>(pdg, risingEnergyEdge, fallingEnergyEdge,
                                                                       risingScanEdge, fallingScanEdge, flag));
}

void FilterManager::SetNew_Process_Filter(G4String processName, G4double risingEnergyEdge, G4double fallingEnergyEdge,
                                          G4double risingScanEdge, G4double fallingScanEdge, G4bool flag)
{
    ifFilter_Process = false;
    Filter_Process_List.emplace_back(std::make_shared<FilterProcess>(processName, risingEnergyEdge, fallingEnergyEdge,
                                                                     risingScanEdge, fallingScanEdge, flag));
}
