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

RootMessenger::RootMessenger()
	 : G4UImessenger()
{
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
  delete fEngDirectory;
  delete fSetRndEngCmd;

}

void RootMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
    if( command == fSetRndEngCmd )
        CLHEP::HepRandom::getTheEngine()->setSeed(G4UIcmdWithAnInteger::GetNewIntValue(newValue),0);
}

