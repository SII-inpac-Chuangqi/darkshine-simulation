
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

#include <sstream>

class RootManager;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// class description:
//  

class RootMessenger: public G4UImessenger
{
  public:

	  RootMessenger(RootManager* rootMng);
	  virtual ~RootMessenger();
	  virtual void SetNewValue(G4UIcommand*, G4String);

  private:

      G4UIdirectory*           fEngDirectory;

      G4UIcmdWithAnInteger*    fSetRndEngCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


