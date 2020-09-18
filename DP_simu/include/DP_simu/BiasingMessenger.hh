
#ifndef BiasingMessenger_h
#define BiasingMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "BOptrChangeCrossSection.hh"

class BOptrChangeCrossSection;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class BiasingMessenger: public G4UImessenger
{
  public:

	  BiasingMessenger(BOptrChangeCrossSection* );
	  virtual ~BiasingMessenger();
	  virtual void SetNewValue(G4UIcommand*, G4String);

  private:

      BOptrChangeCrossSection*      fBiasingOptr;
	  G4UIdirectory*                fBiasingDirectory;
      G4UIcmdWithADouble*           fBiasFactorCmd;
      G4UIcmdWithADoubleAndUnit*    fEminCmd;
      G4UIcmdWithAString*           fBiasProcessCmd;
	
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


