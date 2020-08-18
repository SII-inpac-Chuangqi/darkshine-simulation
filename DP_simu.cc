//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id$
//
/// \file example.cc
/// \brief Main program of the  example

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "TrackingAction.hh"
#include "SteppingAction.hh"
#include "RootManager.hh"

//#include "G4StepLimiterBuilder.hh"  // Geant4.9
#include "G4StepLimiterPhysics.hh"  // Geant4.10
#include "G4GenericBiasingPhysics.hh"
#include "GammaPhysics.h"
#include "OpticalPhysics.h"
#include "G4OpticalPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "FTFP_BERT.hh"

#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " factory [-m macro ] [-o OpticalMacro] [-t nThreads]" << G4endl;
    G4cerr << "   note: -t option is available only for multi-threaded mode." << G4endl;
    G4cerr << G4endl;
  }
}

int main(int argc,char** argv)
{

    /* Evaluate Arguments */

  if ( argc > 9 ) {
    PrintUsage();
    return 1;
  }
  
  G4String macro;
  G4String OpticalMacro;
  G4String gdmlFileName;
#ifdef G4MULTITHREADED
  G4int nofThreads = 0;
#endif
  for ( G4int i=1; i<argc; i=i+2 ) {
    if      ( G4String(argv[i]) == "-m" ) macro = argv[i+1];
    else if ( G4String(argv[i]) == "-o" ) OpticalMacro = argv[i+1];
#ifdef G4MULTITHREADED
    else if ( G4String(argv[i]) == "-t" ) {
      nofThreads = G4UIcommand::ConvertToInt(argv[i+1]);
    }
#endif
    else {
      PrintUsage();
      //return 1;
    }
  }  
    
  G4cout<<macro<<", "<<OpticalMacro<<G4endl;

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Choose the Random engine

  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  
  // Construct the root manager
  
  RootManager * rootMng = new RootManager;

  // Construct the default run manager
  
  G4RunManager * runManager = new G4RunManager;

  // Set mandatory initialization classes

  runManager->SetUserInitialization(new DetectorConstruction(rootMng));

  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  physicsList->RegisterPhysics(new G4StepLimiterPhysics());
  physicsList->RegisterPhysics( new GammaPhysics() );
  G4GenericBiasingPhysics* biasingPhysics = new G4GenericBiasingPhysics();
  biasingPhysics->Bias("e-");
  biasingPhysics->Bias("gamma");
  physicsList->RegisterPhysics(biasingPhysics);
    
  //physicsList->ReplacePhysics(new G4EmStandardPhysics_option4());    
  //G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  //physicsList->RegisterPhysics(opticalPhysics);

  physicsList->RegisterPhysics( new OpticalPhysics( rootMng ) );

  runManager->SetUserInitialization(physicsList);
    
  if ( OpticalMacro.size() )   // batch mode
    {
      G4String command = "/control/execute ";
      UImanager->ApplyCommand(command+OpticalMacro);
    }

  // Set user action classes
  RunAction* run_action = new RunAction(rootMng);
  runManager->SetUserAction( run_action );

  EventAction* event_action = new EventAction(rootMng);
  runManager->SetUserAction(event_action);

  runManager->SetUserAction(new PrimaryGeneratorAction());
  runManager->SetUserAction(new TrackingAction(rootMng));
  SteppingAction* stepping_action = new SteppingAction(rootMng);
  runManager->SetUserAction(stepping_action);
  
  // Initialize G4 kernel
  runManager->Initialize();

#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  if ( macro.size() )   // batch mode
    {
      G4String command = "/control/execute ";
      UImanager->ApplyCommand(command+macro);
    }
  else if ( !OpticalMacro.size() )
    {  // interactive mode : define UI session
#ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
        UImanager->ApplyCommand("/control/execute init_vis.mac");
#else
        UImanager->ApplyCommand("/control/execute init.mac");
#endif
      if (ui->IsGUI())
         UImanager->ApplyCommand("/control/execute gui.mac");
      ui->SessionStart();
      delete ui;
#endif
    }


  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete rootMng;
  delete runManager;

  return 0;
}

/* test on CLion */
/* test on Git */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
