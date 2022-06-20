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
/// \file RunAction.cc
/// \brief Implementation of the RunAction class

#include "DP_simu/RunAction.hh"
#include "DP_simu/RootManager.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
        : G4UserRunAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction() = default;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run *aRun) {
    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

    // record time information
    time(&start_time);

    //inform the runManager to save random number seed
    G4RunManager::GetRunManager()->SetRandomNumberStore(true);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run *) {
    auto file_size = static_cast<long double>(dRootMng->save());
    auto N_events = static_cast<long double>(dRootMng->getFEvtNRecorded());
    auto event_size = (N_events > 0) ? file_size / 1000. / N_events : 0;

    G4cout << "[Performance Statistics] ==> recorded events: " << Form("%.0Lf", N_events) << G4endl;
    G4cout << "[Performance Statistics] ==> event size [KB/event]: " << Form("%.2Lf", event_size) << G4endl;

    // record time information
    time(&end_time);
    auto event_time = (N_events > 0) ? double(end_time - start_time) * 1000. / N_events : 0;
    G4cout << "[Performance Statistics] ==> event time [ms/event]: " << Form("%.0Lf", event_time) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
