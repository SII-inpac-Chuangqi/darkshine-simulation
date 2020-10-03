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
/// \file eventgenerator/HepMC/HepMCEx01/src/ExN04PrimaryGeneratorAction.cc
/// \brief Implementation of the ExN04PrimaryGeneratorAction class
//
//

#include "DP_simu/PrimaryGeneratorAction.hh"
#include "DP_simu/PrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "DP_simu/HepMCG4AsciiReader.hh"
#include "DP_simu/HepMCG4PythiaInterface.hh"
#include "TRandom.h"
#include "TRandom3.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorAction::PrimaryGeneratorAction()
        : G4VUserPrimaryGeneratorAction() {
    // default generator is particle gun.
    fCurrentGenerator = fGPS = new G4GeneralParticleSource();
    fCurrentGeneratorName = "fParticleGun";

    fParticleGun = new G4ParticleGun();
    fHepmcAscii = new HepMCG4AsciiReader();
#ifdef G4LIB_USE_PYTHIA
    fPythiaGen = new HepMCG4PythiaInterface();
#else
    fPythiaGen = 0;
#endif
    fGentypeMap["particleGun"] = fParticleGun;
    fGentypeMap["GPS"] = fGPS;
    fGentypeMap["hepmcAscii"] = fHepmcAscii;
    fGentypeMap["pythia"] = fPythiaGen;

    fMessenger = new PrimaryGeneratorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fMessenger;
    delete fCurrentGenerator;
    delete fPythiaGen;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
    if (fCurrentGeneratorName == "particleGun" && fDist) {
        double px, py, pz;
        fDist->GetRandom3(px, py, pz);

        fParticleGun->SetParticleMomentum(G4ParticleMomentum(px, py, pz));
    }

    if (fCurrentGenerator)
        fCurrentGenerator->GeneratePrimaryVertex(anEvent);
    else
        G4Exception("PrimaryGeneratorAction::GeneratePrimaries",
                    "PrimaryGeneratorAction001", FatalException,
                    "generator is not instanciated.");
}

void PrimaryGeneratorAction::SetHist(const G4String &a) {
    //gRandom->SetSeed(0);

    G4cout << "Reading distribution from file: " << filename << G4endl;
    fRootFile = new TFile(filename);

    fDist = (TH3D *) fRootFile->Get(a);
}
