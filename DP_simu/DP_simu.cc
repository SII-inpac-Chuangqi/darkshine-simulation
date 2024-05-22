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
#include "Utility/parser.h"

#include "G4Version.hh"

#include "G4StepLimiterPhysics.hh"  // Geant4.10
#include "G4GenericBiasingPhysics.hh"
#include "DP_simu/GammaPhysics.h"
#include "Optical/OpticalPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "DarkPhysics/DarkMatterPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "Animation/AnimationData.h"

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
#ifdef DSIMU_VERSION
        cerr << "DSimu " << (std::string)DSIMU_VERSION << std::endl;
#else
        cerr << "DSIMU " << "v1.5.5" << endl;
#endif
    }
}

int main(int argc, char **argv) {

    // Get the pointer to the User Interface manager
    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    // Choose the Random engine
    // CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

    // Initiate Control Class
    Control::CreateInstance();

    // Initiate Animation Data Class
    AnimationData::CreateInstance();
//-------------------------------------------------------------------------------
// Evaluate Arguments

    std::string macro;
    std::string OpticalMacro;
    std::string yamlFileName;
    bool gui_mode = false;
    long random_seed;
    bool save_geometry = true;
    std::string outfile_Name;
    int Run_Number;
    int beam_on;
    bool print_version = false;
    bool if_animation = false;

    arg_parser::Parser parser;
    parser.Add("m,macro",        macro,        "");
    parser.Add("o,opticalMacro", OpticalMacro, "Optical macro name");
    parser.Add("y,yaml",         yamlFileName, "YAML file name");
    parser.Add("s,seed",         random_seed,  "Random seed, if not set read from YAML");
    parser.Add("f,outfile_Name", outfile_Name, "Output name, if not set read from YAML");
    parser.Add("n,Run_Number",   Run_Number,   "Run number for this job (EventID = id + beam_on * Run_Number), if not set read from YAML");
    parser.Add("b,beam_on",      beam_on,      "Beam on number, if not set read from YAML");
    parser.AddFlag("g,gui",         gui_mode,      false, "GUI mode");
    parser.AddFlag("save_geometry", save_geometry, true,  "Save Geometry, if not set read from YAML");
    parser.AddFlag("v,version",     print_version, false, "Print version");
    parser.AddFlag("a,animation",   if_animation,  false, "Store animation info for DDis");
    parser.Parse(argc, argv);

    if(print_version) {
        PrintVersion();
        return 0;
    }

    if(parser.IfInCommandLine("seed")) {
        dControl->read_yaml_random_seed = false;
        dControl->random_seed = random_seed;
    }
    if(parser.IfInCommandLine("save_geometry")) {
        dControl->read_yaml_save_geometry = false;
        dControl->save_geometry = save_geometry;
    }
    if(parser.IfInCommandLine("outfile_Name")) {
        dControl->read_yaml_outfile_Name = false;
        dControl->outfile_Name = outfile_Name;
    }
    if(parser.IfInCommandLine("Run_Number")) {
        dControl->read_yaml_Run_Number = false;
        dControl->Run_Number = Run_Number;
    }
    if(parser.IfInCommandLine("beam_on")) {
        dControl->read_yaml_BeamOnNumber = false;
        dControl->BeamOnNumber = beam_on;
    }

    if (if_animation) {
            pAniData->setUseAni(true);
            G4cout << "[ Animation ]: Activated." << G4endl;
            pAniData->initialization();
    }

    if (if_introduction) PrintIntroduction();
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
    if (
# if G4VERSION_NUMBER >= 1100
        !G4StrUtil::contains(dControl->random_restore_file, ".rndm")
# else
        !dControl->random_restore_file.contains(".rndm")
# endif
       ) {
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


#ifdef DSIMU_DEBUG
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
