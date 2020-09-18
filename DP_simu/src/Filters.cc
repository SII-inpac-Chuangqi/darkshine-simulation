#include "DP_simu/RootManager.hh"





//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::Filter_Track_Initialize()
{

}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......
//
void RootManager::Filter_Event_Initialize()
{
    Filter_HardBrem_Result = false;
    Filter_Process_Result = false;

}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

G4bool RootManager::Filter_HardBrem(const G4Step* aStep)
{
    G4StepPoint* prev = aStep->GetPreStepPoint();
    G4StepPoint* post = aStep->GetPostStepPoint();
    G4double     deltaE = fabs( prev->GetKineticEnergy() - post->GetKineticEnergy() );
    
    if ( aStep->GetTrack()->GetTrackID()==1 ) {
        if ( deltaE >= GammaEmin && post->GetPosition()[2] < HardBrem_ScanDistance ) {
            // search for all the secondary particles produced in this step
            const std::vector< const G4Track* > * secondary = aStep->GetSecondaryInCurrentStep();
            std::vector< const G4Track* >::const_iterator itr_sec;

            for( itr_sec = (*secondary).begin(); itr_sec != (*secondary).end(); itr_sec++ ) {
                const G4Track* aTrack = (*itr_sec);

                auto pdg = aTrack->GetParticleDefinition()->GetPDGEncoding();
                auto energy = aTrack->GetTotalEnergy();

                /* Only Select Gamma */
                if ( pdg != 22 ) continue;

                /* Energy of secondaries requirement */
                if ( energy >= GammaEmin ) {
                    Filter_HardBrem_Result = true;
                    return true;
                }
            }
        }
        else if ( deltaE < GammaEmin && post->GetPosition()[2] < HardBrem_ScanDistance )
            return true;
    }

    return Filter_HardBrem_Result;
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::Filter_Process(const G4Step* aStep) 
{
    G4StepPoint* prev = aStep->GetPreStepPoint();
    G4StepPoint* post = aStep->GetPostStepPoint();
    G4double     deltaE = fabs( prev->GetKineticEnergy() - post->GetKineticEnergy() );
    G4String     pname = post->GetProcessDefinedStep()->GetProcessName();

    if ( deltaE >= ProcessEmin ) {
        if ( post->GetPosition()[2] >= Process_MinScanDistance 
          && post->GetPosition()[2] <= Process_MaxScanDistance ) {
            auto res = pname.contains( ProcessName );
            if (res) Filter_Process_Result = true; 
        }
    }

}
