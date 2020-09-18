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
/// \file SimHit.cc
/// \brief Implementation of the SimHit class

#include "DP_simu/SimHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4Allocator<SimHit> SimHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SimHit::SimHit()
 : G4VHit(),
   fEdep(0.),
   fEdepEM(0.),
   fEdepHad(0.),
   fTrackLength(0.),
   ft(0.),
   fx(0.0),
   fy(0.0),
   fz(0.0),
   fID(-1),
   fPDG(0),
   fDetectorID( G4ThreeVector() ),
   fDetectorRepNo(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SimHit::~SimHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SimHit::SimHit(const SimHit& right)
  : G4VHit()
{
  fEdep        = right.fEdep;
  fTrackLength = right.fTrackLength;
  ft           = right.ft;
  fx           = right.fx;
  fy           = right.fy;
  fz           = right.fz;

  fEdepEM      = right.fEdepEM;
  fEdepHad     = right.fEdepHad;
  fID          = right.fID;
  fPDG         = right.fPDG;
  fDetectorID  = right.fDetectorID;
  fDetectorRepNo = right.fDetectorRepNo;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const SimHit& SimHit::operator=(const SimHit& right)
{
  fEdep        = right.fEdep;
  fTrackLength = right.fTrackLength;
  ft           = right.ft;
  fx           = right.fx;
  fy           = right.fy;
  fz           = right.fz;

  fEdepEM      = right.fEdepEM;
  fEdepHad     = right.fEdepHad;
  fID          = right.fID;
  fPDG         = right.fPDG;
  fDetectorID  = right.fDetectorID;
  fDetectorRepNo = right.fDetectorRepNo;

  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int SimHit::operator==(const SimHit& right) const
{
  return ( this == &right ) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SimHit::Print()
{
  G4cout
     << "Edep: " 
     << std::setw(7) << G4BestUnit(fEdep,"Energy")
     << " track length: " 
     << std::setw(7) << G4BestUnit( fTrackLength,"Length")
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
