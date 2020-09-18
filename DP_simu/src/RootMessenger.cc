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
    /* Event Setting */
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

    /* Bias Setting */
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

    /* Filter Setting */
    fFilterDirectory = new G4UIdirectory("/DP/Filter/");
    fFilterDirectory->SetGuidance("Simulation Filter control");

    // Hard Brem
    fifFilter_HardBremCmd = new G4UIcmdWithABool("/DP/Filter/if_filter_HardBrem",this);
    fifFilter_HardBremCmd->SetGuidance("if filter hardbrem event");
    fifFilter_HardBremCmd->SetDefaultValue(false);
    fifFilter_HardBremCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fFilterGammaEminCmd = new G4UIcmdWithADoubleAndUnit("/DP/Filter/HardBrem_GammaEmin",this);
    fFilterGammaEminCmd->SetGuidance("Min Energy for Hard Brem");
    fFilterGammaEminCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fFilterHardBrem_ScanDistanceCmd = new G4UIcmdWithADoubleAndUnit("/DP/Filter/HardBrem_ScanDistance",this);
    fFilterHardBrem_ScanDistanceCmd->SetGuidance("Max Scan Distance for Hard Brem");
    fFilterHardBrem_ScanDistanceCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    // Process
    fifFilter_ProcessCmd = new G4UIcmdWithABool("/DP/Filter/if_filter_Process",this);
    fifFilter_ProcessCmd->SetGuidance("if filter Process event");
    fifFilter_ProcessCmd->SetDefaultValue(false);
    fifFilter_ProcessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fFilterProcessNameCmd = new G4UIcmdWithAString("/DP/Filter/Process_Name",this);
    fFilterProcessNameCmd->SetGuidance("which process to be filtered");
    fFilterProcessNameCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fFilterProcessEminCmd = new G4UIcmdWithADoubleAndUnit("/DP/Filter/Process_Emin",this);
    fFilterProcessEminCmd->SetGuidance("Min Energy for certain process");
    fFilterProcessEminCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fFilterProcess_MinScanDistanceCmd = new G4UIcmdWithADoubleAndUnit("/DP/Filter/Process_MinScanDistance",this);
    fFilterProcess_MinScanDistanceCmd->SetGuidance("Max Scan Distance for certain process");
    fFilterProcess_MinScanDistanceCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fFilterProcess_MaxScanDistanceCmd = new G4UIcmdWithADoubleAndUnit("/DP/Filter/Process_MaxScanDistance",this);
    fFilterProcess_MaxScanDistanceCmd->SetGuidance("Max Scan Distance for certain process");
    fFilterProcess_MaxScanDistanceCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    /* Optical Photon */
    fifOpticalCmd = new G4UIcmdWithABool("/DP/if_Optical_Photon",this);
    fifOpticalCmd->SetGuidance("if simulate optical photon process");
    fifOpticalCmd->SetDefaultValue(false);
    fifOpticalCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

    /* Rndm Setting */
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
  delete fifFilter_HardBremCmd;
  delete fFilterGammaEminCmd;
  delete fFilterHardBrem_ScanDistanceCmd;
  delete fifFilter_ProcessCmd;
  delete fFilterProcessNameCmd;
  delete fFilterProcessEminCmd;
  delete fFilterProcess_MinScanDistanceCmd;
  delete fFilterProcess_MaxScanDistanceCmd;
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

    if( command == fifFilter_HardBremCmd)
        froot->SetifFilter_HardBrem(fifFilter_HardBremCmd->GetNewBoolValue(newValue));

    if( command == fFilterGammaEminCmd )
        froot->SetGammaEmin(fFilterGammaEminCmd->GetNewDoubleValue(newValue));

    if( command == fFilterHardBrem_ScanDistanceCmd )
        froot->SetHardBrem_ScanDistance(fFilterHardBrem_ScanDistanceCmd->GetNewDoubleValue(newValue));

    if( command == fifFilter_ProcessCmd)
        froot->SetifFilter_Process(fifFilter_ProcessCmd->GetNewBoolValue(newValue));

    if( command == fFilterProcessNameCmd )
        froot->SetProcessName(newValue);

    if( command == fFilterProcessEminCmd )
        froot->SetProcessEmin(fFilterProcessEminCmd->GetNewDoubleValue(newValue));

    if( command == fFilterProcess_MinScanDistanceCmd )
        froot->SetProcess_MinScanDistance(fFilterProcess_MinScanDistanceCmd->GetNewDoubleValue(newValue));

    if( command == fFilterProcess_MaxScanDistanceCmd )
        froot->SetProcess_MaxScanDistance(fFilterProcess_MaxScanDistanceCmd->GetNewDoubleValue(newValue));

    if( command == fifOpticalCmd ) {
        froot->SetOptical(fifOpticalCmd->GetNewBoolValue(newValue));
    }

    if( command == fSetRndEngCmd )
        CLHEP::HepRandom::getTheEngine()->setSeed(fSetRndEngCmd->GetNewIntValue(newValue),0);
}

