
#ifndef RootMessenger_h
#define RootMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIcommand.hh"
#include "RootManager.hh"

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
class RootManager;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RootMessenger: public G4UImessenger
{
  public:

	  RootMessenger(RootManager* rootMng);
	  virtual ~RootMessenger();
	  virtual void SetNewValue(G4UIcommand*, G4String);

  private:

	  RootManager*             froot;

	  G4UIdirectory*           fEventDirectory;
	  G4UIdirectory*           fBiasDirectory;
	  G4UIdirectory*           fFilterDirectory;
      G4UIdirectory*           fEngDirectory;

      G4UIcmdWithAnInteger*    fSetRndEngCmd;
      G4UIcmdWithAnInteger*    fEvtJobCmd;
      G4UIcmdWithAnInteger*    fJobNbCmd;
      G4UIcmdWithABool*        fCleanCmd;
      G4UIcmdWithABool*        fFilterCmd;

      G4UIcmdWithAString*      fBiasProcessCmd;
      G4UIcmdWithADouble*      fBiasFactorCmd;
      G4UIcmdWithABool*        fifBiasTargetCmd;
      G4UIcmdWithABool*        fifBiasECALCmd;
      G4UIcmdWithADoubleAndUnit* fBiasEminCmd;

      G4UIcmdWithABool*        fifFilter_HardBremCmd;
      G4UIcmdWithADoubleAndUnit* fFilterGammaEminCmd;
      G4UIcmdWithADoubleAndUnit* fFilterHardBrem_ScanDistanceCmd;

      G4UIcmdWithABool*        fifFilter_ProcessCmd;
      G4UIcmdWithAString*      fFilterProcessNameCmd;
      G4UIcmdWithADoubleAndUnit* fFilterProcessEminCmd;
      G4UIcmdWithADoubleAndUnit* fFilterProcess_MinScanDistanceCmd;
      G4UIcmdWithADoubleAndUnit* fFilterProcess_MaxScanDistanceCmd;

      G4UIcmdWithABool*         fifOpticalCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


