#include "DP_simu/RootMessenger.hh"
#include "DP_simu/RootManager.hh"
#include "Randomize.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcommand.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//

RootMessenger::RootMessenger(RootManager* rootMng)
	 : G4UImessenger(), froot(rootMng)
{
    // Event Setting
    fEventDirectory = new G4UIdirectory("/DP/Event/");
    fEventDirectory->SetGuidance("Event operations");
    
    fEvtJobCmd = new G4UIcmdWithAnInteger("/DP/Event/StartID",this);
    fEvtJobCmd->SetGuidance("Set the Evt Start ID for this job.");
    fEvtJobCmd->SetParameterName("startid",false);
    fEvtJobCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fJobNbCmd = new G4UIcmdWithAnInteger("/DP/Event/NbEvent",this);
    fJobNbCmd->SetGuidance("Set the Evt Number for this job.");
    fJobNbCmd->SetParameterName("evtNb",false);
    fJobNbCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fCleanCmd = new G4UIcmdWithABool("/DP/Event/ifClean",this);
    fCleanCmd->SetGuidance("whether to clean the event.");
    fCleanCmd->SetParameterName("if_clean",false);
    fCleanCmd->SetDefaultValue(false);
    fCleanCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fFilterCmd = new G4UIcmdWithABool("/DP/Event/ifFilter",this);
    fFilterCmd->SetGuidance("whether to filter the event.");
    fFilterCmd->SetParameterName("if_filter",false);
    fFilterCmd->SetDefaultValue(false);
    fFilterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    // Bias Setting
    fBiasDirectory = new G4UIdirectory("/DP/Bias/");
    fBiasDirectory->SetGuidance("Event operations");

    fBiasProcessCmd = new G4UIcmdWithAString("/DP/Bias/Process",this);
    fBiasProcessCmd->SetGuidance("which process to be biased");
    fBiasProcessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fBiasFactorCmd = new G4UIcmdWithADouble("/DP/Bias/Factor",this);
    fBiasFactorCmd->SetGuidance("Bias Factor");
    fBiasFactorCmd->SetDefaultValue(1.0);
    fBiasFactorCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fBiasEminCmd = new G4UIcmdWithADoubleAndUnit("/DP/Bias/Emin",this);
    fBiasEminCmd->SetGuidance("Min Energy to be biased");
    fBiasEminCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fifBiasTargetCmd = new G4UIcmdWithABool("/DP/Bias/if_bias_target",this);
    fifBiasTargetCmd->SetGuidance("if bias target");
    fifBiasTargetCmd->SetDefaultValue(false);
    fifBiasTargetCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fifBiasECALCmd = new G4UIcmdWithABool("/DP/Bias/if_bias_ecal",this);
    fifBiasECALCmd->SetGuidance("if bias ecal");
    fifBiasECALCmd->SetDefaultValue(false);
    fifBiasECALCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    // Filter Setting
    fFilterDirectory = new G4UIdirectory("/DP/Filter/");
    fFilterDirectory->SetGuidance("Simulation Filter control");

    fSetNewParticleFilter = new G4UIcmdWithAString("/DP/Filter/particle", this);
    fSetNewParticleFilter->SetGuidance("Add New Particle Filter.");
    fSetNewParticleFilter->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSetNewProcessFilter = new G4UIcmdWithAString("/DP/Filter/process", this);
    fSetNewProcessFilter->SetGuidance("Add New Process Filter");
    fSetNewProcessFilter->AvailableForStates(G4State_PreInit, G4State_Idle);

    // Optical Photon
    fifOpticalCmd = new G4UIcmdWithABool("/DP/if_Optical_Photon",this);
    fifOpticalCmd->SetGuidance("if simulate optical photon process");
    fifOpticalCmd->SetDefaultValue(false);
    fifOpticalCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

    // Rndm Setting
    fEngDirectory = new G4UIdirectory("/DP/Engine/");
    fEngDirectory->SetGuidance("Simulation Engine control");
    
    fSetRndEngCmd = new G4UIcmdWithAnInteger("/DP/Engine/randomSeed",this);
    fSetRndEngCmd->SetGuidance("Set the random seed for Rnd Engine.");
    fSetRndEngCmd->SetParameterName("rndseed",false);
    fSetRndEngCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
}

RootMessenger::~RootMessenger()
{
  delete fEvtJobCmd;
  delete fJobNbCmd;
  delete fCleanCmd;
  delete fFilterCmd;
  delete fEngDirectory;
  delete fSetRndEngCmd;
  delete fBiasDirectory;
  delete fEventDirectory;
  delete fBiasProcessCmd;
  delete fBiasFactorCmd;
  delete fifBiasTargetCmd;
  delete fifBiasECALCmd;
  delete fBiasEminCmd;
  delete fFilterDirectory;
  delete fSetNewParticleFilter;
  delete fSetNewProcessFilter;
  delete fifOpticalCmd;
}

void RootMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
    if( command == fEvtJobCmd )
        froot->SetStartID(fEvtJobCmd->GetNewIntValue(newValue));
    
    if( command == fJobNbCmd )
        froot->SetNbEvent(fJobNbCmd->GetNewIntValue(newValue));
    
    if( command == fCleanCmd )
        froot->SetClean(fCleanCmd->GetNewBoolValue(newValue));
    
    if( command == fFilterCmd )
        froot->SetFilter(fFilterCmd->GetNewBoolValue(newValue));
    
    if( command == fBiasProcessCmd )
        froot->SetBiasProcess(newValue);
     
    if( command == fBiasFactorCmd )
        froot->SetBiasFactor(fBiasFactorCmd->GetNewDoubleValue(newValue));

    if( command == fBiasEminCmd )
        froot->SetBiasEmin(fBiasEminCmd->GetNewDoubleValue(newValue));

    if( command == fifBiasTargetCmd)
        froot->SetifBiasTarget(fifBiasTargetCmd->GetNewBoolValue(newValue));

    if( command == fifBiasECALCmd)
        froot->SetifBiasECAL(fifBiasECALCmd->GetNewBoolValue(newValue));

    if( command == fSetNewParticleFilter) {
        std::vector<G4String> res; // storing splitted string.
        G4String result; // temporary splitted stirng.
        std::stringstream input(newValue);
        while(input>>result)
            res.emplace_back(result);
        assert(res.size() == 5);
        froot->SetNew_Particle_Filter(
            std::stoi(res[0]), // pdf
            std::stod(res[1]), // Energy risingEdge
            std::stod(res[2]), // Energy fallingEdge
            std::stod(res[3]), // minDistance
            std::stod(res[4])  // maxDistance
        );
    }

    if( command == fSetNewProcessFilter) {
        std::vector<G4String> res;
        G4String result;
        std::stringstream input(newValue);
        while(input>>result)
            res.emplace_back(result);
        assert(res.size() == 5);
        froot->SetNew_Process_Filter(
            res[0],            // processName
            std::stod(res[1]), // Energy risingEdge
            std::stod(res[2]), // Energy fallingEdge
            std::stod(res[3]), // minDistance
            std::stod(res[4])  // maxDistance
        );
    }

    if( command == fifOpticalCmd ) {
        froot->SetOptical(fifOpticalCmd->GetNewBoolValue(newValue));
    }

    if( command == fSetRndEngCmd )
        CLHEP::HepRandom::getTheEngine()->setSeed(fSetRndEngCmd->GetNewIntValue(newValue),0);
}

