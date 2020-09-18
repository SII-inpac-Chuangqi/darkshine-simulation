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
/// \file eventgenerator/HepMC/HepMCEx01/src/HepMCG4Interface.cc
/// \brief Implementation of the HepMCG4Interface class
//
//

#include "DP_simu/HepMCG4Interface.hh"

#include "G4RunManager.hh"
#include "G4LorentzVector.hh"
#include "G4Event.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4TransportationManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <cstdio>

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HepMCG4Interface::HepMCG4Interface()
  : hepmcEvent(0)
{
    RunNumber   = 0;
    EventNumber   = 0;

    MCNumber   = 0;
    for(int i = 0; i<10; i++ ) {
        MC_PDG[i] = 0;
        MC_Energy[i] = 0;
        MC_Px[i] = 0;
        MC_Py[i] = 0;
        MC_Pz[i] = 0;
    }

    tr = new TTree("mc","MCParticle");

    tr->Branch("RunNumber",     &RunNumber,     "RunNumber/I");
    tr->Branch("EventNumber",   &EventNumber,   "EventNumber/I");
    tr->Branch("MCNumber",      &MCNumber,      "MCNumber/I");
    tr->Branch("MC_PDG",        &MC_PDG,        "MC_PDG[MCNumber]/I");
    tr->Branch("MC_Energy",     &MC_Energy,     "MC_Energy[MCNumber]/D");
    tr->Branch("MC_Px",         &MC_Px,         "MC_Px[MCNumber]/D");
    tr->Branch("MC_Py",         &MC_Py,         "MC_Py[MCNumber]/D");
    tr->Branch("MC_Pz",         &MC_Pz,         "MC_Pz[MCNumber]/D");

    if( remove("HepMC.root") == 0 ) G4cout<<"HepMC.root deleted."<<G4endl;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HepMCG4Interface::~HepMCG4Interface()
{

    delete hepmcEvent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool HepMCG4Interface::CheckVertexInsideWorld
                         (const G4ThreeVector& pos) const
{
  G4Navigator* navigator= G4TransportationManager::GetTransportationManager()
                                                 -> GetNavigatorForTracking();

  G4VPhysicalVolume* world= navigator-> GetWorldVolume();
  G4VSolid* solid= world-> GetLogicalVolume()-> GetSolid();
  EInside qinside= solid-> Inside(pos);

  if( qinside != kInside) return false;
  else return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HepMCG4Interface::HepMC2G4(const HepMC::GenEvent* hepmcevt,
                                G4Event* g4event)
{
    rootFile = new TFile("HepMC.root","UPDATE");
        
    if(!rootFile) {
        G4cout << " HepMCG4Interface::Initialization :"
            << " problem creating the ROOT TFile "
            << G4endl;
        return;    
    }

    for(HepMC::GenEvent::vertex_const_iterator vitr= hepmcevt->vertices_begin();
        vitr != hepmcevt->vertices_end(); ++vitr ) { // loop for vertex ...
    
        // real vertex?
        G4bool qvtx=false;
        for (HepMC::GenVertex::particle_iterator
            pitr= (*vitr)->particles_begin(HepMC::children);
            pitr != (*vitr)->particles_end(HepMC::children); ++pitr) {
    
            if (!(*pitr)->end_vertex() && (*pitr)->status()==1) {
            qvtx=true;
            break;
          }
        }
        if (!qvtx) continue;
    

        G4double r = 8000./(0.3*1.5);
        G4double z = 607.5;
        G4double y = r-sqrt(r*r-z*z);
        G4double cosTheta = z/sqrt(z*z+y*y);
        G4double sinTheta = y/sqrt(z*z+y*y);
        // check world boundary
        HepMC::FourVector pos= (*vitr)-> position();
        G4LorentzVector xvtx(pos.x(), pos.y(), pos.z(), pos.t());
        if (! CheckVertexInsideWorld(xvtx.vect()*mm)) continue;
    
        // create G4PrimaryVertex and associated G4PrimaryParticles
        G4PrimaryVertex* g4vtx=
          new G4PrimaryVertex(xvtx.x()*mm, xvtx.y()*mm + y, xvtx.z()*mm,
                              xvtx.t()*mm/c_light);
    
        for (HepMC::GenVertex::particle_iterator
            vpitr= (*vitr)->particles_begin(HepMC::children);
            vpitr != (*vitr)->particles_end(HepMC::children); ++vpitr) {
    
            if( (*vpitr)->status() != 1 ) continue;
    
            G4int pdgcode= (*vpitr)-> pdg_id();
            pos= (*vpitr)-> momentum();
            G4LorentzVector p(pos.px(), pos.py()*cosTheta + pos.pz()*sinTheta, pos.pz()*cosTheta - pos.py()*sinTheta, pos.e());
            
            // filling the root file
            MC_PDG[MCNumber] = (int)pdgcode;
            MC_Energy[MCNumber] = p.t();
            MC_Px[MCNumber] = p.x();
            MC_Py[MCNumber] = p.y();
            MC_Pz[MCNumber] = p.z();

            MCNumber++;
    
            G4PrimaryParticle* g4prim=
              new G4PrimaryParticle(pdgcode, p.x()*MeV, p.y()*MeV, p.z()*MeV);
    
            g4vtx-> SetPrimary(g4prim);
        }
        g4event-> AddPrimaryVertex(g4vtx);
    }
    tr->Fill();
    
    EventNumber++;
    MCNumber   = 0;
    for(int i = 0; i<10; i++ ) {
        MC_PDG[i] = 0;
        MC_Energy[i] = 0;
        MC_Px[i] = 0;
        MC_Py[i] = 0;
        MC_Pz[i] = 0;
    }
    if (rootFile) {
        rootFile->WriteTObject(tr,"","Overwrite");
        rootFile->Close();
        //G4cout << "===> HepMC Tree is saved \n" << G4endl;        
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HepMC::GenEvent* HepMCG4Interface::GenerateHepMCEvent()
{
  HepMC::GenEvent* aevent= new HepMC::GenEvent();
  return aevent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HepMCG4Interface::GeneratePrimaryVertex(G4Event* anEvent)
{
  // delete previous event object
  delete hepmcEvent;

  // generate next event
  hepmcEvent= GenerateHepMCEvent();
  if(! hepmcEvent) {
    G4cout << "HepMCInterface: no generated particles. run terminated..."
           << G4endl;
    G4RunManager::GetRunManager()-> AbortRun();
    return;
  }
  HepMC2G4(hepmcEvent, anEvent);
}
