#include "Filter/FilterManager.hh"

/// Including class FilterManager, FilterParticle, FilterProcess.

/// class FilterManager

/// \brief Filter Particle method. Scan every FilterParticle->Filter().
/// \return true - aboart,
/// false - keep.
G4bool FilterManager::Filter_Particle(const G4Step* aStep) {
    particle_end = Filter_Particle_List.end();
    for (particle_itr = Filter_Particle_List.begin(); particle_itr != particle_end; particle_itr++) {
        std::shared_ptr<FilterParticle> fFilterParticle = *particle_itr;
        if (fFilterParticle->Filter(aStep)) {
            Filter_Particle_Result = true;
            return true;
        }
    }
    Filter_Particle_Result = false;
    return false; 
}

/// \brief Filter Process method. Scan every FilterProcess->Filter().
/// Store result in Filter_Process_Result.
void FilterManager::Filter_Process(const G4Step* aStep) {
    process_end = Filter_Process_List.end();
    for (process_itr = Filter_Process_List.begin(); process_itr != process_end; process_itr++) {
        std::shared_ptr<FilterProcess> fFilterProcess = *process_itr;
        if (fFilterProcess->Filter(aStep)) Filter_Process_Result = true;
    }
    Filter_Process_Result = false;
}

void FilterManager::Filter_Track_Initialize() {

}

void FilterManager::Filter_Event_Initialize() {
    Filter_Particle_Result = false;
    Filter_Process_Result = false;
}

void FilterManager::SetNew_Particle_Filter(G4int pdg, G4double risingEdge, G4double fallingEdge,
                                          G4double minDistance, G4double maxDistance)
{
    ifFilter_Particle = true;
    Filter_Particle_List.emplace_back(std::make_shared<FilterParticle>(pdg, risingEdge, fallingEdge,
                                                                       minDistance, maxDistance));
}

void FilterManager::SetNew_Process_Filter(G4String processName, G4double risingEdge, G4double fallingEdge ,G4double minDistance, G4double maxDistance) {
    ifFilter_Process = false;
    Filter_Process_List.emplace_back(std::make_shared<FilterProcess>(processName, risingEdge, fallingEdge, minDistance, maxDistance));
}

/// class FilterParticle

FilterParticle::FilterParticle(G4int pdg, G4double risingEdge, G4double fallingEdge,
                               G4double minDistance, G4double maxDistance) {
    PDG = pdg;
    Edge_Rising = risingEdge;
    Edge_Falling = fallingEdge;
    ScanDistance_Min = minDistance;
    ScanDistance_Max = maxDistance;
}

/// \brief simple filter 
G4bool FilterParticle::Energy_Filter(G4double in) {
    // special case: Edge_Falling=Edge_Rising, return true
    if (Edge_Falling <= Edge_Rising) {
        if (in >= Edge_Rising || in < Edge_Falling) {
            return true;
        }
        else {
            return false;
        }
    }
    else { // Edge_Rising < Edge_Falling
        if (Edge_Rising <= in && in < Edge_Falling) {
            return true;
        }
        else {
            return false;
        }
    }
}

/// \brief Check if one particular Particle should be filtered. 
/// \return true - filter,
/// false - keep.
G4bool FilterParticle::Filter(const G4Step* aStep) {
    post = aStep->GetPostStepPoint();
    post_distance = post->GetPosition()[2];
    if (ScanDistance_Min <= post_distance && post_distance <= ScanDistance_Max) {
        prev = aStep->GetPreStepPoint();
        // search for all the secondary particles produced in this step
        secondary = aStep->GetSecondaryInCurrentStep();
        sec_end = (*secondary).end();
        for ( sec_itr = (*secondary).begin(); sec_itr != sec_end; sec_itr++ ) {
            aTrack = (*sec_itr);
            // Select particle
            if ( PDG != aTrack->GetParticleDefinition()->GetPDGEncoding() ) continue;
            // Energy of secondaries reqirement
            energy = aTrack->GetTotalEnergy();
            if(Energy_Filter(energy)) {
                return true;
            }
        }
    }
    return false;
}

/// class FilterProcess

FilterProcess::FilterProcess(G4String processName, G4double risingEdge, G4double fallingEdge,
                             G4double minDistance, G4double maxDistance) {
    Process_Name = processName;
    Edge_Rising = risingEdge;
    Edge_Falling = fallingEdge;
    ScanDistance_Min = minDistance;
    ScanDistance_Max = maxDistance;
    
}

G4bool FilterProcess::Energy_Filter(G4double in) {
    // special case: Edge_Falling=Edge_Rising, return true
    if (Edge_Falling <= Edge_Rising) {
        if (in >= Edge_Rising || in < Edge_Falling) {
            return true;
        }
        else {
            return false;
        }
    }
    else { // Edge_Rising < Edge_Falling
        if (Edge_Rising <= in && in < Edge_Falling) {
            return true;
        }
        else {
            return false;
        }
    }
}

/// \brief Check if one particular process should be filtered.
/// \return true - filter,
/// fasle - keep.
G4bool FilterProcess::Filter(const G4Step* aStep) {
    prev = aStep->GetPreStepPoint();
    post = aStep->GetPostStepPoint();
    deltaE = fabs( prev->GetKineticEnergy() - post->GetKineticEnergy() );
    pname = post->GetProcessDefinedStep()->GetProcessName();
    post_distance = post->GetPosition()[2];

    if  (ScanDistance_Min <= post_distance && post_distance <= ScanDistance_Max) {
        if (Energy_Filter(deltaE)) {
            res = pname.contains( Process_Name );
            if (res) return true;
        }
    }
    return false;
}
