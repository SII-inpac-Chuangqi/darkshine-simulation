#include "Filter/FilterManager.hh"

/// Including class FilterManager, FilterParticle, FilterProcess.

/// class FilterManager

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

/// class FilterParticle

FilterParticle::FilterParticle(G4int pdg, G4double risingEnergyEdge, G4double fallingEnergyEdge,
                               G4double risingScanEdge, G4double fallingScanEdge, G4bool flag) {
    PDG = pdg;
    Energy_Edge_Rising = risingEnergyEdge;
    Energy_Edge_Falling = fallingEnergyEdge;
    ScanDistance_Edge_Rising = risingScanEdge;
    ScanDistance_Edge_Falling = fallingScanEdge;
    Flag = flag;
}

/// \brief simple filter
/// \return true - pass, false - stop. 
G4bool FilterParticle::Square_Filter(G4double val, G4double risingEdge, G4double fallingEdge) {
    // All-pass case: fallingEdge=risingEdge, return true
    if ( fallingEdge <= risingEdge) {
        if (val >= risingEdge || val < fallingEdge)
            return true;
        else
            return false;
    }
    else { // risingEdge < fallingEdge 
        if (val >= risingEdge && val < fallingEdge)
            return true;
        else 
            return false;
    }
}

/// \brief Check if one particular Particle is in particular energy range and distance range. 
/// \return true - in the range,
/// false - out of range.
G4bool FilterParticle::Filter(const G4Step* aStep) {
    post = aStep->GetPostStepPoint();
    post_distance = post->GetPosition()[2];
    if (Square_Filter(post_distance, ScanDistance_Edge_Rising, ScanDistance_Edge_Falling)) {
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
            if( Square_Filter(energy, Energy_Edge_Rising, Energy_Edge_Falling) ) {
                return true;
            }
        }
        return false;
    }
    else
        return false;
}

/// class FilterProcess

FilterProcess::FilterProcess(G4String processName, G4double risingEnergyEdge, G4double fallingEnergyEdge,
                             G4double risingScanEdge, G4double fallingScanEdge, G4bool flag) {
    Process_Name = processName;
    Energy_Edge_Rising = risingEnergyEdge;
    Energy_Edge_Falling = fallingEnergyEdge;
    ScanDistance_Edge_Rising = risingScanEdge;
    ScanDistance_Edge_Falling = fallingScanEdge;
    Flag = flag;
}

/// \brief simple filter
/// \return true - pass, false - stop. 
G4bool FilterProcess::Square_Filter(G4double val, G4double risingEdge, G4double fallingEdge) {
    // All-pass case: fallingEdge=risingEdge, return true
    if ( fallingEdge <= risingEdge) {
        if (val >= risingEdge || val < fallingEdge)
            return true;
        else
            return false;
    }
    else { // risingEdge < fallingEdge 
        if (val >= risingEdge && val < fallingEdge)
            return true;
        else 
            return false;
    }
}

/// \brief Check if one particular process is in particular energy range and distance range.
/// \return true - in the range,
/// fasle - out of range.
G4bool FilterProcess::Filter(const G4Step* aStep) {
    prev = aStep->GetPreStepPoint();
    post = aStep->GetPostStepPoint();
    deltaE = fabs( prev->GetKineticEnergy() - post->GetKineticEnergy() );
    pname = post->GetProcessDefinedStep()->GetProcessName();
    post_distance = post->GetPosition()[2];

    if  ( Square_Filter(post_distance, ScanDistance_Edge_Rising, ScanDistance_Edge_Falling) ) {
        if ( Square_Filter(deltaE, Energy_Edge_Rising, Energy_Edge_Falling)) {
            res = pname.contains( Process_Name );
            if (res) return true;
        }
        else
            return false;
    }
    else 
        return false;
}
