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

#include "DP_simu/DetectorConstruction.hh"
#include "DP_simu/PrimaryGeneratorAction.hh"
#include "DP_simu/RunAction.hh"
#include "DP_simu/EventAction.hh"
#include "DP_simu/TrackingAction.hh"
#include "DP_simu/StackingAction.hh"
#include "DP_simu/SteppingAction.hh"
#include "DP_simu/RootManager.hh"
#include "Control/Control.h"
#include "Bias_Filter/FilterManager.hh"
#include "Utility/TruthManager.h"

#include "G4StepLimiterPhysics.hh"  // Geant4.10
#include "G4GenericBiasingPhysics.hh"
#include "DP_simu/GammaPhysics.h"
#include "Optical/OpticalPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "DarkPhysics/DarkMatterPhysics.hh"
#include "G4OpticalPhysics.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "FTFP_BERT.hh"

#include "Randomize.hh"

//#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
//#endif

//#ifdef G4UI_USE
#include "G4UIExecutive.hh"
//#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace {
    bool if_introduction(true);

    void PrintIntroduction() {
        G4cerr << G4endl;
        G4cerr << "**************************************************************" << G4endl;
        G4cerr << " Copyright @DarkSHINE Project" << G4endl;
        G4cerr << "           By Master of C++," << G4endl;
        G4cerr << "              Master of Geant4," << G4endl;
        G4cerr << "              Master of PhD," << G4endl;
        G4cerr << "              Master of All Masters," << G4endl;
        G4cerr << "              Yulei Zhang" << G4endl;
        G4cerr << "              https://gitlab.com/yulei_zhang" << G4endl;
        G4cerr << "**************************************************************" << G4endl;
    }

    void PrintVersion() {
        G4cerr << "DSimu " << dControl->DSimu_version << G4endl; // date: 2021-07-28
    }

    void PrintUsage() {
        PrintIntroduction();
        if_introduction = false;
        PrintVersion();

        G4cerr << G4endl;
        G4cerr << " Usage: " << G4endl;
        G4cerr << " DSimu [-y default.yaml] [-m macro ] [-o OpticalMacro]" << G4endl;
        G4cerr << "   note: yaml file is necessary." << G4endl;
        G4cerr << " -g --gui                        Gui Mode" << G4endl;
        G4cerr << " -y --yaml default.yaml          Read YAML file" << G4endl;
        G4cerr << " -v --version                    Print version" << G4endl;
        G4cerr << "\n The following arguments overwrite the YAML settings." << G4endl;
        G4cerr << " -s --seed 42                    Set seed" << G4endl;
        G4cerr << "    --save_geometry 0            Set save_geometry" << G4endl;
        G4cerr << " -f --outfile_Name dp_out.root   Set output filename" << G4endl;
        G4cerr << " -n --Run_Number 0               Set the run number for this job (EventID = id + beam_on * Run_Number)" << G4endl;
        G4cerr << " -b --beam_on 1                  Set beam-on number" << G4endl;
        G4cerr << "**************************************************************" << G4endl;
        G4cerr << G4endl;
    }

}

int main(int argc, char **argv) {

    // Get the pointer to the User Interface manager
    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    // Choose the Random engine
    // CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

    // Initiate Control Class
    Control::CreateInstance();

//-------------------------------------------------------------------------------
// Evaluate Arguments

    G4String macro;
    G4String OpticalMacro;
    G4String yamlFileName;

    bool gui_mode = false;
    for (G4int i = 1; i < argc; i = i + 2) {
        if (G4String(argv[i]) == "-g" || G4String(argv[i]) == "--gui" ) gui_mode = true;
        else if (G4String(argv[i]) == "-m" || G4String(argv[i]) == "--macro" ) macro = argv[i + 1];
        else if (G4String(argv[i]) == "-o" || G4String(argv[i]) == "--opticalMacro" ) OpticalMacro = argv[i + 1];
        else if (G4String(argv[i]) == "-y" || G4String(argv[i]) == "--yaml" ) yamlFileName = argv[i + 1];
        else if (G4String(argv[i]) == "-s" || G4String(argv[i]) == "--seed") {
            dControl->read_yaml_random_seed = false;
            dControl->random_seed = std::stol(argv[i + 1]);
        }
        else if (G4String(argv[i]) == "--save_geometry" ) {
            dControl->read_yaml_save_geometry = false;
            dControl->save_geometry = std::stoi(argv[i + 1]);
        }
        else if (G4String(argv[i]) == "-f" || G4String(argv[i]) == "--outfile_Name" ) {
            dControl->read_yaml_outfile_Name = false;
            dControl->outfile_Name = G4String(argv[i + 1]);
        }
        else if ( G4String(argv[i]) == "-n" || G4String(argv[i]) == "--Run_Number" ) {
            dControl->read_yaml_Run_Number = false;
            //dControl->Run_Number = *argv[i + 1] - '0';
            dControl->Run_Number = std::stoi(argv[i + 1]);
        }
        else if (G4String(argv[i]) == "-b" || G4String(argv[i]) == "--beam_on" ) {
            dControl->read_yaml_BeamOnNumber = false;
            //dControl->BeamOnNumber = *argv[i + 1] - '0';
            dControl->BeamOnNumber = std::stoi(argv[i + 1]);
        }
        else if (G4String(argv[i]) == "-h" || G4String(argv[i]) == "--help" ) {
            PrintUsage();
            return 1;
        } else if (G4String(argv[i]) == "-v" || G4String(argv[i]) == "--version") {
            PrintVersion();
            return 1;
        } else {
            PrintUsage();
            //return 1;
        }
    }

    if(if_introduction) PrintIntroduction();
    PrintVersion();

//-------------------------------------------------------------------------
    //G4cout << macro << ", " << OpticalMacro << G4endl;

    // Read Configuration from YAML
    auto yaml_valid = dControl->ReadYAML(yamlFileName);
    if (!yaml_valid) {
        std::cerr << "[Read YAML] ==> Reading Error from YAML file: " << std::endl;
        return -1;
    }
    dControl->ConstructG4MaterialTable();
    dControl->AssignG4Material();

    // Initiate RootManager Class
    RootManager::CreateInstance();

    // Initiate Truth Manager
    TruthManager::CreateInstance();

    // Initiate DDetectorIDMaps Class
    DDetectorIDMaps::CreateInstance();

    // Rebuild all dependent variables
    // All the parameters are locked for now
    dControl->RebuildVariables();
    dControl->ReadAndSetRandomSeed();

    G4Random::setTheEngine(new CLHEP::RanecuEngine());
    if (!dControl->random_restore_file.contains(".rndm")) {
        G4Random::setTheSeed(dControl->random_seed);
    } else {
        G4Random::restoreEngineStatus(dControl->random_restore_file.c_str());
    }

    // Initialize all the self-defined Singletons
    FilterManager::CreateInstance();

    if (!OpticalMacro.empty())   // batch mode
    {
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command + OpticalMacro);
    }

    // Construct the default run manager

    auto *runManager = new G4RunManager;

    // Set mandatory initialization classes

    runManager->SetUserInitialization(new DetectorConstruction());

    G4VModularPhysicsList *physicsList = new FTFP_BERT;

    // Dark Physics
    if (dControl->signal_production) physicsList->RegisterPhysics(new DarkMatterPhysics());
    //physicsList->ReplacePhysics(new DarkMatterPhysics());

    physicsList->SetVerboseLevel(0);

    // Optical Physics
    if (dControl->if_optical) {
        std::cout << "[Main] ==> Optical Physics Init... " << std::endl;
        //auto *opticalPhysics = new OpticalPhysics(dControl->Optical_PhysicsVerbose);
        auto *opticalPhysics = new G4OpticalPhysics(dControl->Optical_PhysicsVerbose);
        physicsList->ReplacePhysics(opticalPhysics);
    }
    physicsList->RegisterPhysics(new G4StepLimiterPhysics());
    physicsList->RegisterPhysics(new GammaPhysics());

    // Biasing
    if (dControl->if_bias) dControl->ReadAndSetBias(physicsList);
    //physicsList->RegisterPhysics( new OpticalPhysics( rootMng ) );

    runManager->SetUserInitialization(physicsList);

    // Set user action classes
    auto *run_action = new RunAction();
    runManager->SetUserAction(run_action);

    auto *event_action = new EventAction();
    runManager->SetUserAction(event_action);

    runManager->SetUserAction(new PrimaryGeneratorAction());
    runManager->SetUserAction(new TrackingAction());
    if (dControl->if_filter)
        runManager->SetUserAction(new StackingAction);
    auto *stepping_action = new SteppingAction();
    runManager->SetUserAction(stepping_action);

    dControl->ReadAndSetVerbosity();
    dControl->ReadAndSetGPS();

    // Initialize G4 kernel
    runManager->Initialize();

    // Initialize visualization
    G4VisManager *visManager = new G4VisExecutive;
    visManager->Initialize();

    if (!macro.empty())   // batch mode
    {
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command + macro);
    } else if (OpticalMacro.empty()) {  // interactive mode : define UI session
        std::cout << macro.size() << ", " << OpticalMacro.size() << std::endl;
    }


#ifdef DEBUG
    std::cout << "[INFO] Navigator CheckMode ON" << std::endl;
    UImanager->ApplyCommand("/geometry/navigator/check_mode 1");
#endif

    if (gui_mode) {
        auto *ui = new G4UIExecutive(argc, argv);
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        if (ui->IsGUI())
            UImanager->ApplyCommand("/control/execute gui.mac");
        ui->SessionStart();
        delete ui;
    } else {
        runManager->BeamOn(dControl->BeamOnNumber);
    }



    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted
    // in the main() program !

//#ifdef G4VIS_USE
    delete visManager;
//#endif
    delete runManager;

    return 0;
}

/* test on Pi */
/* test on Git */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
