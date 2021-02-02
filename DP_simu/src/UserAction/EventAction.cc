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
/// \file EventAction.cc
/// \brief Implementation of the EventAction class

#include "DP_simu/EventAction.hh"
#include "DP_simu/RootManager.hh"
#include "Bias_Filter/FilterManager.hh"

#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "G4THitsCollection.hh"
#include "G4UnitsTable.hh"

#include <iostream>

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RootManager *rootMng)
        : G4UserEventAction(),
          fPrintModulo(100),
          fStartID(0),
          fEvtNb(1e8) {
    frootMng = rootMng;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
= default;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event *event) {
    fPrintModulo = frootMng->GetNbEvent() / 100;

    G4int eventID = event->GetEventID();
    if (eventID < 100 || eventID % fPrintModulo == 0) {
        G4cout << "\n---> Begin of event: " << eventID << G4endl;
        //CLHEP::HepRandom::showEngineStatus();
    }

    G4RunManager::GetRunManager()->StoreRandomNumberStatusToG4Event(1);

    dFilterManager->Filter_Event_Initialize();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event *event) {
    if (event->IsAborted()) {
        frootMng->initialize();
        return;
    }
    if ( ( dFilterManager->GetifFilter_Process() && !dFilterManager->Filter_Process_Found_Result() )
         ||( dFilterManager->GetifFilter_Particle() && !dFilterManager->Filter_Particle_Found_Result() ) ) {
        frootMng->initialize();
        return;
    }
    const G4String &RndmS = G4RunManager::GetRunManager()->GetRandomNumberStatusForThisEvent();
    const char *rn = RndmS.data();

    long double r1 = 0;
    int Nr = 0;
    double nr = 10000;
    double rndm[4];
    for (int i = 24; i < 100; i++) {
        if (Nr > 3) break;
        if ((int) rn[i] - (int) '0' == -38) {
            rndm[Nr] = r1 * pow(10, -1 - (log(nr) / log(10)));
            nr = 10000;
            r1 = 0;
            Nr++;
            //G4cout<<"RR: "<<rndm[Nr-1]<<G4endl;
            continue;
        }

        r1 = r1 + nr * (double) ((int) rn[i] - (int) '0');
        nr /= 10.0;
    }

    // print per event (modulo n)

    G4int eventID = event->GetEventID();
    if (eventID < 100 || eventID % fPrintModulo == 0) {
        G4cout << "---> End of event: " << eventID << G4endl;
    }
    if(dControl->if_optical) frootMng->FinalizeOptical();

    frootMng->FillSim(eventID, rndm); //refresh event level data, so any fill should before this!
}
