#include "DP_simu/BiasingMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcommand.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//

BiasingMessenger::BiasingMessenger(BOptrChangeCrossSection* BOptr)
	 : G4UImessenger(), fBiasingOptr(BOptr)
{
  //fBiasingDirectory = new G4UIdirectory("/DP/Biasing/");
  //fBiasingDirectory->SetGuidance("Biasing operations");

  //fBiasFactorCmd = new G4UIcmdWithADouble("/DP/Biasing/Factor",this);
  //fBiasFactorCmd->SetGuidance("The biasing factor for the certain process.");
  //fBiasFactorCmd->SetParameterName("factor",false);

  //fEminCmd = new G4UIcmdWithADoubleAndUnit("/DP/Biasing/Emin",this);
  //fEminCmd->SetGuidance("Set the minimal Energy for biasing.");
  //fEminCmd->SetParameterName("Emin",false);

  //fBiasProcessCmd = new G4UIcmdWithAString("/DP/Biasing/Process",this);
  //fBiasProcessCmd->SetGuidance("Set process for biasing.");
  //fBiasProcessCmd->SetParameterName("BiasProcess",false);
  //fBiasProcessCmd->SetCandidates("GammaToMuPair photonNuclear electronNuclear");
}

BiasingMessenger::~BiasingMessenger()
{
  delete fBiasFactorCmd;
  delete fEminCmd;
  delete fBiasProcessCmd;
}

void BiasingMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == fBiasFactorCmd )
  { 
	fBiasingOptr->SetBiasFactor(fBiasFactorCmd->GetNewDoubleValue(newValue));
  }

  if( command == fEminCmd )
  { 
	fBiasingOptr->SetEmin(fEminCmd->GetNewDoubleValue(newValue));
  }

  if( command == fBiasProcessCmd )
  { 
	fBiasingOptr->SetBiasProcess(newValue);
  }
}

